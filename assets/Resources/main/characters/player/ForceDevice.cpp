#include "ForceDevice.h"
#include "Game.h"
#include "GeneralDefines.h"

ForceDevice::ForceDevice(glm::mat4 *project) {
    projection = project;

    collider = new Collision(1,project, Collision::Force);

    collisionSystem = CollisionSystem::getInstance();

    forceLevel = 0;
}

void ForceDevice::init(Collision *sCollider) {
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Force);

    sprite = Sprite::createSprite(glm::ivec2(20, 20), glm::vec2(1.0f/10.0f, 1.0f/16.0f), spritesheet, projection);
    sprite->setNumberAnimations(3);

        int offset = 1;
        float subX = 1.0f / 10.0f;
        float subY = 1.0f / 16.0f;
        sprite->setAnimationSpeed(ForceMK1, 8);
        sprite->addKeyframe(ForceMK1, glm::vec2(subX*0, subY*offset));
        sprite->addKeyframe(ForceMK1, glm::vec2(subX*1, subY*offset));
        sprite->addKeyframe(ForceMK1, glm::vec2(subX*2, subY*offset));
        sprite->addKeyframe(ForceMK1, glm::vec2(subX*3, subY*offset));
        sprite->addKeyframe(ForceMK1, glm::vec2(subX*4, subY*offset));
        sprite->addKeyframe(ForceMK1, glm::vec2(subX*5, subY*offset));

        offset = 2;
        sprite->setAnimationSpeed(ForceMK2, 8);
        sprite->addKeyframe(ForceMK2, glm::vec2(subX * 0, subY*offset));
        sprite->addKeyframe(ForceMK2, glm::vec2(subX * 1, subY*offset));
        sprite->addKeyframe(ForceMK2, glm::vec2(subX * 2, subY*offset));
        sprite->addKeyframe(ForceMK2, glm::vec2(subX * 3, subY*offset));

        offset = 3;
        sprite->setAnimationSpeed(ForceMK3, 8);
        sprite->addKeyframe(ForceMK3, glm::vec2(subX * 0, subY*offset));
        sprite->addKeyframe(ForceMK3, glm::vec2(subX * 1, subY*offset));
        sprite->addKeyframe(ForceMK3, glm::vec2(subX * 2, subY*offset));
        sprite->addKeyframe(ForceMK3, glm::vec2(subX * 3, subY*offset));

    sprite->changeAnimation(forceLevel, false);

    collider->addCollider(glm::vec4(0, 4, 10, 20-4));
    collisionSystem->addColliderIntoGroup(collider);
    collisionSystem->updateCollider(collider, posForce);
    collider->changePositionAbsolute(posForce);

    sprite->setBox(glm::vec2(collider->getBoundingBox().z, collider->getBoundingBox().w));
    collider->setBox(glm::vec2(collider->getBoundingBox().z, collider->getBoundingBox().w));

	isAtached = false;
    targetPosition = glm::vec2(isAtached ? posForce.x : rightLimit, posForce.y);

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(posForce.x, posForce.y));

	shipCollider = sCollider;

	if (!text.init("fonts/OpenSans-Bold.ttf"))
		cout << "Could not load font!!!" << endl;
}

void ForceDevice::update(int deltaTime) {
    inputController();
    collisionRoutine();
	collisionSystem->updateCollider(collider, posForce);

    sprite->setPosition(glm::vec2(posForce.x, posForce.y));
    sprite->update(deltaTime);
}

void ForceDevice::render() {
    sprite->render();


#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES

	string word = "";
	if (cont > 0) {
		if (levelup) word = "Increased";
		else word = "Decreased";
		word += " the level of the Force, actual level: " + to_string(forceLevel);
		text.render(word, glm::vec2(0, 64), 32, glm::vec4(1, 1, 1, 1), Text::Left);
		cont -= 1;
	}
}

void ForceDevice::setPosition(const glm::vec2 &pos) {
    posForce = pos;
    sprite->setPosition(posForce);
    collider->changePositionAbsolute(posForce);
}

Collision* ForceDevice::getCollider() {
    return collider;
}

void ForceDevice::setForceLevel(int level) {
	if (level > 2) level = 2;
	else if (level < 0) level = 0;
	forceLevel = level;
    sprite->changeAnimation(level,false);
}

void ForceDevice::deleteRoutine() {
	collisionSystem->removeColliderFromGroup(collider);
	delete collider;
}

void ForceDevice::inputController() {
    // Press State
    if (Game::instance().getKey('j') && !latchKeys['j']) {
        latchKeys['j'] = true;

		forceLevel++;
		setForceLevel(forceLevel);
		cont = 60;
		levelup = true;
    }
    else if (Game::instance().getKey('h') && !latchKeys['h']) {
        latchKeys['h'] = true;

		forceLevel--;
		setForceLevel(forceLevel);
		cont = 60;
		levelup = false;
    }
    else if (Game::instance().getKey('z') && !latchKeys['z']) {
        latchKeys['z'] = true;

        if (isAtached && isLeft) targetPosition.x = leftLimit;
        else if (isAtached && !isLeft) targetPosition.x = rightLimit;
        else if (!isAtached && isLeft) targetPosition.x = rightLimit;
        else targetPosition.x = leftLimit;

        if (!isAtached) isLeft = !isLeft;

        isAtached = false;

        rotateForce(glm::vec3(0.0f, isLeft && isAtached ? 180.0f:0.0f, 0.0f));

    }
    else if (Game::instance().getKey('x') && !latchKeys['x']) {
        latchKeys['x'] = true;

		int offsetAttach = isAtached ? 3 : 0;
		int tmp = offsetAttach + (int)forceLevel;

		if (currentShotCountdown < 0) {
			spawnProjectiles();
			currentShotCountdown = shotDelay[tmp];
		}
    }

    // Release State
    if (!Game::instance().getKey('j') && latchKeys['j']) latchKeys['j'] = false;
    else if (!Game::instance().getKey('h') && latchKeys['h']) latchKeys['h'] = false;
    else if (!Game::instance().getKey('z') && latchKeys['z']) latchKeys['z'] = false;
	else if (!Game::instance().getKey('x') && latchKeys['x']) {
		latchKeys['x'] = false;
	}

	currentShotCountdown -= 1;
	if (currentShotCountdown > 10000) currentShotCountdown = 0;
}

void ForceDevice::collisionRoutine() {
    if (isAtached) targetPosition = shipCollider->position + getOffsetofColliders(isLeft);
    else targetPosition.y = shipCollider->position.y + getOffsetofColliders(isLeft).y;

	bool moved = false;

    if (abs(targetPosition.x - posForce.x) >= forceVelocity.x) {
        int sign = (targetPosition.x >= posForce.x) ? ((targetPosition.x - posForce.x > 0.0f) ? 1 : -1) : ((posForce.x - targetPosition.x > 0.0f) ? -1 : 1);
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::ivec2(sign * forceVelocity.x, 0));

		if (!info.colliding) {
			setPosition(posForce + glm::vec2(sign * forceVelocity.x, 0.0f));
			moved |= true;
		}
		else collisionHelper(info);

    }

    if (abs(targetPosition.y - posForce.y) >= forceVelocity.y) {
        int sign = (targetPosition.y >= posForce.y) ? ((targetPosition.y - posForce.y > 0.0f) ? 1 : -1) : ((posForce.y - targetPosition.y > 0.0f) ? -1 : 1);
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::ivec2(0, sign * forceVelocity.y));

		if (!info.colliding) {
			setPosition(posForce + glm::vec2(0.0f, sign * forceVelocity.y));
			moved |= true;
		}
		else collisionHelper(info);

    }

	if (!moved) {
		CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::ivec2(0, 0));
		if (info.colliding) collisionHelper(info);
	}

    CollisionSystem::CollisionInfo infoAttach = collisionSystem->isTriggering(collider, glm::ivec2(0, 0));

    if (infoAttach.triggered && infoAttach.collider->collisionGroup == Collision::Player) {
        attachToASide();
        rotateForce(glm::vec3(0.0f, isLeft && isAtached ? 180.0f : 0.0f, 0.0f));
    }

    if (isAtached && abs(targetPosition.y - posForce.y) <= 10.0f && 
        abs(targetPosition.x - posForce.x) <= 10.0f) setPosition(targetPosition);

}

glm::vec2 ForceDevice::getOffsetofColliders(bool left) {
    glm::vec2 offset = glm::vec2(0.0f, 0.0f);

    glm::vec4 shipBoundingBox = shipCollider->getBoundingBox();
    glm::vec4 forceBoundingBox = collider->getBoundingBox();

    //  horizontal
    if (left) offset -= glm::vec2((forceBoundingBox.z - forceBoundingBox.x) + forceBoundingBox.x, 0.0f);
    else offset += glm::vec2((shipBoundingBox.z - shipBoundingBox.x) + shipBoundingBox.x, 0.0f);

    // vertical
    float shipCenterY = ((shipBoundingBox.w + shipBoundingBox.y) / 2.0f) + shipBoundingBox.y;
    float forceCenterY = ((forceBoundingBox.w + forceBoundingBox.y) / 2.0f) + forceBoundingBox.y;
	float sign = (forceCenterY > shipCenterY) ? -1.0f : 1.0f;

    offset += glm::vec2(0.0f, sign * abs(shipCenterY - forceCenterY)/2.0f);

    return offset;
}

void ForceDevice::attachToASide() {
    glm::vec4 shipBoundingBox = shipCollider->getBoundingBox();
    glm::vec4 forceBoundingBox = collider->getBoundingBox();

    isLeft = !(abs((collider->position.x + forceBoundingBox.x) - (shipCollider->position.x + shipBoundingBox.z)) 
                < abs((collider->position.x + forceBoundingBox.z) - (shipCollider->position.x + shipBoundingBox.x)));
    isAtached = true;

    targetPosition = shipCollider->position + getOffsetofColliders(isLeft);
    posForce = targetPosition;
    collisionSystem->updateCollider(collider, targetPosition);
    collider->changePositionAbsolute(targetPosition);
    sprite->setPosition(targetPosition);
    
}

void ForceDevice::rotateForce(const glm::vec3 &rotation) {
    sprite->setRotation(rotation);
    collider->setRotation(rotation);
}

void ForceDevice::spawnProjectiles() {
    glm::vec4 boundingBox = collider->getBoundingBox();
    glm::vec2 spawnOffset = glm::vec2((((boundingBox.z - boundingBox.x) * 0.2f),(boundingBox.w - boundingBox.y)/2.0f));

    float sign = isLeft && isAtached ? -1.0f : 1.0f;

    switch (forceLevel) {
    case ForceMK1:
        if (!isAtached) ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 5.0f, 0.0f), true, Projectile::ForceProjectile);
        break;
    case ForceMK2:
        if (!isAtached) {
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 5.0f, -5.0f), true, Projectile::ForceProjectile);
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 5.0f, 0.0f), true, Projectile::ForceProjectile);
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 5.0f, 5.0f), true, Projectile::ForceProjectile);
        }
        else {
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 5.0f, -5.0f), true, Projectile::Fireball);
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 5.0f, 5.0f), true, Projectile::Fireball);
        }
        break;
    case ForceMK3:
        if (!isAtached) {
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 0.0f, -5.0f), true, Projectile::ForceProjectile);
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 5.0f, -5.0f), true, Projectile::ForceProjectile);
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 5.0f, 0.0f), true, Projectile::ForceProjectile);
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 5.0f, 5.0f), true, Projectile::ForceProjectile);
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 0.0f, 5.0f), true, Projectile::ForceProjectile);
        }
        else {
            ProjectileFactory::getInstance()->spawnProjectile(posForce + spawnOffset, glm::vec2(sign * 5.0f, 0.0f), true, Projectile::Waves);
        }
        break;
    }
}

bool ForceDevice::collisionHelper(const CollisionSystem::CollisionInfo &info) {
	switch (info.collider->collisionGroup) {
	case Collision::EnemyProjectiles:
		ProjectileFactory::getInstance()->destroyProjectile(info.collider->getId());
		break;
	case Collision::Enemy:
		CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
		break;
	}
	return true;
}
