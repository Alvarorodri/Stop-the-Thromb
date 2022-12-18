#include "Player.h"
#include "Game.h"
#include "GeneralDefines.h"

Player::Player(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos) :Character(project, id, Collision::Player){
	init(tileMapPos);
}

void Player::init(const glm::ivec2 &tileMapPos) {
    bJumping = false;
	rot = false;

#pragma region Player
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::PlayerStt);
	sprite = Sprite::createSprite(glm::ivec2(64, 32), glm::vec2(1 / 2.0, 1 / 4.0), spritesheet, projection);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0, false);
	tileMapDispl = tileMapPos;
#pragma endregion

#pragma region ChargeProjectile
	Texture* tmpSpriteSheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Projectiles);

	chargeProjectile = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1 / 16.0, 1 / 16.0), tmpSpriteSheet, projection);
	chargeProjectile->setNumberAnimations(1);

	chargeProjectile->setAnimationSpeed(0, 8);
	chargeProjectile->addKeyframe(0, glm::vec2((1 / 16.0) * 0.0, (1 / 16.0) * 12.0f));
	chargeProjectile->addKeyframe(0, glm::vec2((1 / 16.0) * 2.0, (1 / 16.0) * 12.0f));
	chargeProjectile->addKeyframe(0, glm::vec2((1 / 16.0) * 4.0, (1 / 16.0) * 12.0f));
	chargeProjectile->addKeyframe(0, glm::vec2((1 / 16.0) * 6.0, (1 / 16.0) * 12.0f));
	chargeProjectile->addKeyframe(0, glm::vec2((1 / 16.0) * 8.0, (1 / 16.0) * 12.0f));

	chargeProjectile->changeAnimation(0, false);
#pragma endregion


	collider->addCollider(glm::ivec4(3, 6, 62, 21));
	collisionSystem->addColliderIntoGroup(collider);
	collisionSystem->updateCollider(collider, glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
	collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	chargeProjectile->setPosition(pos + projectileOffset);

	if (!text.init("fonts/OpenSans-Bold.ttf"))
		cout << "Could not load font!!!" << endl;

	initAnimation();
}

void Player::update(int deltaTime)
{
    sprite->update(deltaTime);

	if (!isInitAnimation) {
		chargeProjectile->update(deltaTime);

		inputController();

#pragma region Player movement and Animation

		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && ((pos.x-3)>=0)) {
			if (rot == false) {
				rot = true;
				rotate(0.f, 180.f, 0.f);
			}
			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(-3, 0));
			CollisionSystem::CollisionInfo info2;
			if (forceSpawned) info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(-3, 0));

			if (info.colliding && info.collider->collisionGroup != Collision::CollisionGroups::Force) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
				else if(info.collider->collisionGroup == Collision::CollisionGroups::Enemy || info.collider->collisionGroup == Collision::CollisionGroups::EnemyStatic){
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					if (!godmode)CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
			}
			else if (info.colliding || (forceSpawned && forceDevice->isAttached() && info2.colliding)) {
				sprite->changeAnimation(STAND_RIGHT, false);
			}
			else {
				pos.x -= 3;
				collider->changePositionAbsolute(pos);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)&&((pos.x + 3) <= 415)) {
			if (rot == true) {
				rot = false;
				rotate(0.f, 0.f, 0.f);
			}
			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(3, 0));
			CollisionSystem::CollisionInfo info2;
			if (forceSpawned) info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(3, 0));
			if (info.colliding && info.collider->collisionGroup != Collision::CollisionGroups::Force) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
				else if (info.collider->collisionGroup == Collision::CollisionGroups::Enemy || info.collider->collisionGroup == Collision::CollisionGroups::EnemyStatic) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					if (!godmode)CharacterFactory::getInstance()->damageCharacter(id, 1);
				}

			}
			else if (info.colliding || (forceSpawned && forceDevice->isAttached() && info2.colliding)) {
				sprite->changeAnimation(STAND_RIGHT, false);
			}
			else {
				pos.x += 3;
				collider->changePositionAbsolute(pos);
			}
		}

		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {

			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(0, 2));
			CollisionSystem::CollisionInfo info2;
			if (forceSpawned) info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(0, 2));

			if (info.colliding && info.collider->collisionGroup != Collision::CollisionGroups::Force) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
				else if (info.collider->collisionGroup == Collision::CollisionGroups::Enemy || info.collider->collisionGroup == Collision::CollisionGroups::EnemyStatic) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					if (!godmode)CharacterFactory::getInstance()->damageCharacter(id, 1);
				}

			}
			else if (!(info.colliding || (forceSpawned && forceDevice->isAttached() && info2.colliding))) {
				pos.y += 2;
				collider->changePositionAbsolute(pos);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {

			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(0, -2));
			CollisionSystem::CollisionInfo info2;
			if (forceSpawned) info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(0, -2));

			if (info.colliding && info.collider->collisionGroup != Collision::CollisionGroups::Force) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
				else if (info.collider->collisionGroup == Collision::CollisionGroups::Enemy || info.collider->collisionGroup == Collision::CollisionGroups::EnemyStatic) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					if(!godmode)CharacterFactory::getInstance()->damageCharacter(id, 1);
				}

			}
			else if (!(info.colliding || (forceSpawned && forceDevice->isAttached() && info2.colliding))) {
				pos.y -= 2;
				collider->changePositionAbsolute(pos);
			}
		}
		else {
			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(0, 0));
			CollisionSystem::CollisionInfo info2;
			if (forceSpawned) info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(0, 0));

			if (info.colliding && info.collider->collisionGroup != Collision::CollisionGroups::Force) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
				else if (info.collider->collisionGroup == Collision::CollisionGroups::Enemy || info.collider->collisionGroup == Collision::CollisionGroups::EnemyStatic) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					if (!godmode)CharacterFactory::getInstance()->damageCharacter(id, 1);
				}

			}
		}
		


#pragma endregion

		chargeProjectile->setPosition(pos + projectileOffset);
	}
	else {
		if (pos.x < 150.0f) {
			pos.x += 3;
			collider->changePositionAbsolute(pos);
		}
		else {
			godmode = false;
			isInitAnimation = false;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	Character::update(deltaTime);
}

void Player::inputController() {
    if (Game::instance().getKey('x')) {
		if (!soundcharge) {
			AudioManager::getInstance()->playSoundEffectLooped(AudioManager::ChargeAttack, 128);
			soundcharge = true;
		}
		latchKeys['x'] = true;
		currentCharge += 1.0f;
    }
    else if (Game::instance().getKey('c') && !latchKeys['c']) {
        latchKeys['c'] = true;
    }
	if (Game::instance().getKey('g') && !latchKeys['g']) {
		latchKeys['g'] = true;

		godmode = !godmode;
		contGod = 60;
	}
	else if (Game::instance().getKey('f') && !latchKeys['f']) {
		if (!forceSpawned) spawnForce();	
		latchKeys['f'] = !latchKeys['f'];
	}

	if (!Game::instance().getKey('x') && latchKeys['x']) {
		latchKeys['x'] = false;

		AudioManager::getInstance()->playSoundEffect(AudioManager::LaserGun, 128);

		int mk = 0;
		for (int i = 0; i < 4; ++i) {
			mk = i;
			if (timestampMks[i] > currentCharge) break;
		}
		if (soundcharge) {
			AudioManager::getInstance()->stopSoundEffectLooped(AudioManager::ChargeAttack);
			soundcharge = false;
		}

		ProjectileFactory::getInstance()->spawnProjectile(pos + glm::vec2(rotated ? 0.0f : 62.0f, rotated ? 13.0f : 10.0f), glm::vec2(rotated ? -3.0f : 3.0f, 0.0f), false, (Projectile::ProjectileType)((int)Projectile::R9mk0 + mk));
		currentCharge = 0.0f;
	}
	else if (!Game::instance().getKey('c') && latchKeys['c']) latchKeys['c'] = false;
	else if (!Game::instance().getKey('g') && latchKeys['g']) latchKeys['g'] = false;
	else if (!Game::instance().getKey('f') && latchKeys['f']) latchKeys['f'] = false;
}

void Player::rotate(const float& angleX, const float& angleY, const float& angleZ) {
	Character::rotate(angleX, angleY, angleZ);

	if (angleY != 0.0f) {
		rotated = true;
		projectileOffset = glm::vec2(-12.0f, 6.0f);
	} else {
		rotated = false;
		projectileOffset = glm::vec2(62.0f, 6.0f);
	}

	chargeProjectile->setRotation(glm::vec3(angleX, angleY, angleZ));
}

void Player::setPosition(const glm::vec2 &pos) {
	Character::setPosition(pos);
	chargeProjectile->setPosition(pos + projectileOffset);
}

void Player::render() {
	string word = "";
	if (contGod > 0) {
		if (godmode) word = "Invulnerability activated";
		else word = "Invulnerability desactivated";
		text.render(word, glm::vec2(0,32), 32, glm::vec4(1, 1, 1, 1), Text::Left); 
		contGod-=1;
	}

	Character::render();
	if (currentCharge != 0) chargeProjectile->render();
	
}

void Player::damage(int dmg, int id) {
	if (!godmode) {
		Character::damage(dmg, id);
		AudioManager::getInstance()->stopSoundEffectLooped(AudioManager::ChargeAttack);
	}
}

void Player::spawnForce() {
}

void Player::destroyForce() {
}

void Player::increaseForce(int power) {
}

void Player::initAnimation() {
	isInitAnimation = true;
	AudioManager::getInstance()->playSoundEffect(AudioManager::Boost, 60);
	godmode = true;
}
