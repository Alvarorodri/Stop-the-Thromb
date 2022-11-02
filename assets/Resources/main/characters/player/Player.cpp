#include "Player.h"
#include "Game.h"
#include "GeneralDefines.h"

Player::Player(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos) :Character(project, id, Collision::Player){
	init(tileMapPos);
}

void Player::init(const glm::ivec2 &tileMapPos) {
    bJumping = false;
	rot = false;
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Player);

    sprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(1/16.0, 1/16.0), &spritesheet, projection);
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

    collider->addCollider(glm::ivec4(3, 3, 30, 14));
	collisionSystem->addColliderIntoGroup(collider);
	collisionSystem->updateCollider(collider, glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
    collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));

    forceDevice = new ForceDevice(projection);
    forceDevice->init(collider);
    forceDevice->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 32.0f), float(tileMapDispl.y + pos.y)));
}

void Player::update(int deltaTime)
{
    sprite->update(deltaTime);
    inputController();

    if(Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(-3, 0));
        CollisionSystem::CollisionInfo info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(-3, 0));

        if (info.colliding || (forceDevice->isAttached() && info2.colliding)) {
            sprite->changeAnimation(STAND_RIGHT, false);
        }
        else {
            pos.x -= 3;
			collisionSystem->updateCollider(collider, pos);
            collider->changePositionRelative(glm::vec2(-3, 0));
        }
	}
    else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Player::collider, glm::ivec2(3, 0));
        CollisionSystem::CollisionInfo info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(3, 0));

        if (info.colliding || (forceDevice->isAttached() && info2.colliding)) {
            sprite->changeAnimation(STAND_RIGHT, false);
        }
        else {
            pos.x += 3;
			collisionSystem->updateCollider(collider, pos);
            collider->changePositionRelative(glm::vec2(3, 0));
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
        CollisionSystem::CollisionInfo info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(0, 2));

        if (!(info.colliding || (forceDevice->isAttached() && info2.colliding))) {
            pos.y += 2;
			collisionSystem->updateCollider(collider, pos);
            collider->changePositionRelative(glm::vec2(0, 2));
        }
    } else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
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
        CollisionSystem::CollisionInfo info2 = collisionSystem->isColliding(forceDevice->getCollider(), glm::ivec2(0, -2));

        if (!(info.colliding || (forceDevice->isAttached() && info2.colliding))) {
            pos.y -= 2;
			collisionSystem->updateCollider(collider, pos);
            collider->changePositionRelative(glm::vec2(0, -2));
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

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
    forceDevice->update(deltaTime);
}

void Player::inputController() {
    if (Game::instance().getKey('x') && !latchKeys['x']) {
        latchKeys['x'] = true;

		AudioManager::getInstance()->playSoundEffect(AudioManager::LaserGun, 128);
        ProjectileFactory::getInstance()->spawnProjectile(pos + glm::vec2(32.0f, 6.0f), glm::vec2(3.0f,0.0f), false, Projectile::R9mk0);
    }
    else if (Game::instance().getKey('c') && !latchKeys['c']) {
        latchKeys['c'] = true;
		AudioManager::getInstance()->playSoundEffect(AudioManager::RunningInThe90s, 42);
    }

    if (!Game::instance().getKey('x') && latchKeys['x']) latchKeys['x'] = false;
    else if (!Game::instance().getKey('c') && latchKeys['c']) latchKeys['c'] = false;
}

void Player::setPosition(const glm::vec2 &pos) {
	Character::setPosition(pos);
    forceDevice->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 32.0f), float(tileMapDispl.y + pos.y)));
}

void Player::render() {
	Character::render();
	forceDevice->render();
}

void Player::damage() {
	if (!godmode) {
		live -= 1;
		if (live <= 0)CharacterFactory::getInstance()->killPlayer();
	}
}
