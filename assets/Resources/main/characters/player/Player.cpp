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
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Player);
	sprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(1 / 16.0, 1 / 16.0), spritesheet, projection);
	sprite->setNumberAnimations(7);

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0625*8.0, 0.f));

	sprite->setAnimationSpeed(STAND_UP, 8);
	sprite->addKeyframe(STAND_UP, glm::vec2(0.0625*3.0, 0.0625*2.0f));

	sprite->setAnimationSpeed(MOVE_UP, 8);
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.0625*0.0, 0.0625*2.0f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.0625*1.0, 0.0625*2.0f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.0625*2.0, 0.0625*2.0f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.0625*3.0, 0.0625*2.0f));


	sprite->setAnimationSpeed(STAND_DOWN, 8);
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.0625*3.0, 0.0625*4.0f));

	sprite->setAnimationSpeed(MOVE_DOWN, 8);
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.0625*5.0, 0.0625*4.0f));
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.0625*4.0, 0.0625*4.0f));
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.0625*3.0, 0.0625*4.0f));

	sprite->setAnimationSpeed(UP_RETURN, 8);
	sprite->addKeyframe(UP_RETURN, glm::vec2(0.0625*2.0, 0.0625*2.0f));
	sprite->addKeyframe(UP_RETURN, glm::vec2(0.0625*1.0, 0.0625*2.0f));
	sprite->addKeyframe(UP_RETURN, glm::vec2(0.0625*0.0, 0.0625*2.0f));
	//sprite->addKeyframe(UP_RETURN, glm::vec2(0.0625*2.0, 0.0625*2.0f));


	sprite->setAnimationSpeed(DOWN_RETURN, 8);
	sprite->addKeyframe(DOWN_RETURN, glm::vec2(0.0625*3.0, 0.0625*4.0f));
	sprite->addKeyframe(DOWN_RETURN, glm::vec2(0.0625*4.0, 0.0625*4.0f));
	sprite->addKeyframe(DOWN_RETURN, glm::vec2(0.0625*5.0, 0.0625*4.0f));
	//sprite->addKeyframe(DOWN_RETURN, glm::vec2(0.0625*2.0, 0.0625*2.0f));

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

#pragma region Boost
	Texture* tmpSpriteSheet2 = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Player);
	boost = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1 / 16.0, 1 / 8.0), tmpSpriteSheet2, projection);
	boost->setNumberAnimations(1);

	boost->setAnimationSpeed(0, 30);
	boost->addKeyframe(0, glm::vec2((1 / 16.0) * 1.0, (1 / 8.0) * 3.0f));
	boost->addKeyframe(0, glm::vec2((1 / 16.0) * 2.0, (1 / 8.0) * 3.0f));


	boost->changeAnimation(0, false);
#pragma endregion

	collider->addCollider(glm::ivec4(3, 3, 30, 14));
	collisionSystem->addColliderIntoGroup(collider);
	collisionSystem->updateCollider(collider, glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	chargeProjectile->setPosition(glm::vec2(pos.x + 30.0f, pos.y + 4.0f));
	boost->setPosition(glm::vec2(pos.x - 32.0f, pos.y + 2.0f));

	if (!text.init("fonts/OpenSans-Bold.ttf"))
		cout << "Could not load font!!!" << endl;

	initAnimation();
}

void Player::update(int deltaTime)
{
    sprite->update(deltaTime);
	if (isInitAnimation) boost->update(deltaTime);

	if (!isInitAnimation) {
		chargeProjectile->update(deltaTime);

		inputController();

#pragma region Player movement and Animation

		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && ((pos.x-3)>=0)) {
			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(-3, 0));
			CollisionSystem::CollisionInfo info2;
			if (forceSpawned) info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(-3, 0));

			if (info.colliding && info.collider->collisionGroup != Collision::CollisionGroups::Force) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
				else if(info.collider->collisionGroup == Collision::CollisionGroups::Enemy && !godmode){
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					CharacterFactory::getInstance()->damageCharacter(id, 1);
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
			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(3, 0));
			CollisionSystem::CollisionInfo info2;
			if (forceSpawned) info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(3, 0));
			if (info.colliding && info.collider->collisionGroup != Collision::CollisionGroups::Force) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
				else if (info.collider->collisionGroup == Collision::CollisionGroups::Enemy && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					CharacterFactory::getInstance()->damageCharacter(id, 1);
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
			if (sprite->animation() == STAND_RIGHT) {
				sprite->changeAnimation(MOVE_DOWN, false);
			}
			else if (sprite->animation() == STAND_UP) {
				sprite->changeAnimation(UP_RETURN, false);
			}
			else if (sprite->animation() == STAND_UP || sprite->animation() == MOVE_UP) {
				sprite->changeAnimation(UP_RETURN, false);
			}

			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(0, 2));
			CollisionSystem::CollisionInfo info2;
			if (forceSpawned) info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(0, 2));

			if (info.colliding && info.collider->collisionGroup != Collision::CollisionGroups::Force) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
				else if (info.collider->collisionGroup == Collision::CollisionGroups::Enemy && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}

			}
			else if (!(info.colliding || (forceSpawned && forceDevice->isAttached() && info2.colliding))) {
				pos.y += 2;
				collider->changePositionAbsolute(pos);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			if (sprite->animation() == STAND_RIGHT) {
				sprite->changeAnimation(MOVE_UP, false);
			}
			else if (sprite->animation() == STAND_DOWN) {
				sprite->changeAnimation(DOWN_RETURN, false);
			}
			else if (sprite->animation() == STAND_DOWN || sprite->animation() == MOVE_DOWN) {
				sprite->changeAnimation(DOWN_RETURN, false);
			}

			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(0, -2));
			CollisionSystem::CollisionInfo info2;
			if (forceSpawned) info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(0, -2));

			if (info.colliding && info.collider->collisionGroup != Collision::CollisionGroups::Force) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
				else if (info.collider->collisionGroup == Collision::CollisionGroups::Enemy && !godmode) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}

			}
			else if (!(info.colliding || (forceSpawned && forceDevice->isAttached() && info2.colliding))) {
				pos.y -= 2;
				collider->changePositionAbsolute(pos);
			}
		}

		if (sprite->animation() == MOVE_UP && sprite->isFinidhedAnimation() == true) {
			sprite->changeAnimation(STAND_UP, false);
		}
		else if (sprite->animation() == STAND_UP && !Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			sprite->changeAnimation(UP_RETURN, false);
		}
		else if (sprite->animation() == UP_RETURN && sprite->isFinidhedAnimation() == true) {
			sprite->changeAnimation(STAND_RIGHT, false);
		}
		else if (sprite->animation() == DOWN_RETURN && sprite->isFinidhedAnimation() == true) {
			sprite->changeAnimation(STAND_RIGHT, false);
		}
		else if (sprite->animation() == MOVE_DOWN && sprite->isFinidhedAnimation() == true) {
			sprite->changeAnimation(STAND_DOWN, false);
		}
		else if (sprite->animation() == STAND_DOWN && !Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			sprite->changeAnimation(DOWN_RETURN, false);
		}

#pragma endregion

		if (!forceSpawned || (forceSpawned && !forceDevice->isAttached())) chargeProjectile->setPosition(glm::vec2(pos.x + 30.0f, pos.y + 4.0f));
		else chargeProjectile->setPosition(glm::vec2(pos.x + 40.0f, pos.y + 2.0f));

		if (forceSpawned) forceDevice->update(deltaTime);
	}
	else {
		if (pos.x < 350.0f) {
			pos.x += 3;
			collider->changePositionAbsolute(pos);
		}
		else {
			godmode = false;
			isInitAnimation = false;
		}

		boost->setPosition(glm::vec2(pos.x - 23.0f, pos.y - 6.0f));
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
		else destroyForce();
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

		ProjectileFactory::getInstance()->spawnProjectile(pos + glm::vec2(32.0f, 6.0f), glm::vec2(3.0f, 0.0f), false, (Projectile::ProjectileType)((int)Projectile::R9mk0 + mk));
		currentCharge = 0.0f;
	}
	else if (!Game::instance().getKey('c') && latchKeys['c']) latchKeys['c'] = false;
	else if (!Game::instance().getKey('g') && latchKeys['g']) latchKeys['g'] = false;
	else if (!Game::instance().getKey('f') && latchKeys['f']) latchKeys['f'] = false;
}

void Player::setPosition(const glm::vec2 &pos) {
	Character::setPosition(pos);
	chargeProjectile->setPosition(glm::vec2(pos.x + 30.0f, pos.y + 4.0f));
	if (isInitAnimation) boost->setPosition(glm::vec2(pos.x - 23.0f, pos.y - 6.0f));
	if (forceSpawned) forceDevice->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 32.0f), float(tileMapDispl.y + pos.y)));
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
	if (forceSpawned) forceDevice->render();
	if (currentCharge != 0) chargeProjectile->render();
	if (isInitAnimation) boost->render();
}

void Player::damage(int dmg, int id) {
	if (!godmode) {
		Character::damage(dmg, id);
	}
}

void Player::spawnForce() {
	if (!forceSpawned) {
		forceDevice = new ForceDevice(projection);
		forceDevice->init(collider);
		forceDevice->setPosition(glm::vec2(-20.0f, 256.0f / 2.0f));
		forceSpawned = true;
	}
}

void Player::destroyForce() {
	if (forceSpawned) {
		forceDevice->deleteRoutine();
		delete forceDevice;
		forceSpawned = false;
	}
}

void Player::increaseForce() {
	if (!forceSpawned) spawnForce();
	else forceDevice->setForceLevel(forceDevice->getForceLevel()+1);
}

void Player::initAnimation() {
	isInitAnimation = true;
	AudioManager::getInstance()->playSoundEffect(AudioManager::Boost, 128);
	godmode = true;
}
