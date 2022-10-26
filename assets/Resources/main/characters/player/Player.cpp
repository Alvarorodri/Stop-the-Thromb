#include "Player.h"
#include "Game.h"
#include "GeneralDefines.h"

Player::Player(glm::mat4 *project) {
    projection = project;
    collider = new Collision(project, Collision::Player);

    collisionSystem = CollisionSystem::getInstance();
    collisionSystem->addColliderIntoGroup(collider);
}

void Player::init(const glm::ivec2 &tileMapPos) {
    bJumping = false;
	rot = false;
    spritesheet.loadFromFile("images/player/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
    spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);
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
    collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + posPlayer.x, tileMapDispl.y + posPlayer.y));

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

    forceDevice = new ForceDevice(projection);
    forceDevice->init(collider);
    forceDevice->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + 32.0f), float(tileMapDispl.y + posPlayer.y)));
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
            posPlayer.x -= 3;
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
            posPlayer.x += 3;
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
            posPlayer.y += 2;
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
            posPlayer.y -= 2;
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

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    forceDevice->update(deltaTime);
}

void Player::inputController() {
    if (Game::instance().getKey('x') && !latchKeys['x']) {
        latchKeys['x'] = true;

        ProjectileFactory::getInstance()->spawnProjectile(posPlayer + glm::vec2(32.0f, 6.0f), glm::vec2(3.0f,-3.0f), false, Projectile::R9mk0);
        ProjectileFactory::getInstance()->spawnProjectile(posPlayer + glm::vec2(32.0f, 6.0f), glm::vec2(3.0f, 3.0f), false, Projectile::R9mk0);

    }
    else if (Game::instance().getKey('c') && !latchKeys['c']) {
        latchKeys['c'] = true;
    }

    if (!Game::instance().getKey('x') && latchKeys['x']) latchKeys['x'] = false;
    else if (!Game::instance().getKey('c') && latchKeys['c']) latchKeys['c'] = false;
}

void Player::render() {
    sprite->render();

#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES

    forceDevice->render();
}

void Player::setTileMap(TileMap *tileMap) {
    map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos) {
    posPlayer = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + posPlayer.x, tileMapDispl.y + posPlayer.y));
    forceDevice->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + 32.0f), float(tileMapDispl.y + posPlayer.y)));
}

void Player::rotate(const float &angleX, const float &angleY, const float &angleZ) {
	sprite->setRotation( glm::vec3(angleX, angleY, angleZ));
	collider->setBox(sprite->getQuadsize());
	collider->setRotation(glm::vec3(angleX, angleY, angleZ));
}
