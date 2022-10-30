#include "ForceDevice.h"
#include "Game.h"
#include "GeneralDefines.h"

ForceDevice::ForceDevice(glm::mat4 *project) {
    projection = project;

    collider = new Collision(1,project, Collision::Force);

    collisionSystem = CollisionSystem::getInstance();
    collisionSystem->addColliderIntoGroup(collider);

    forceLevel = 0;
}

void ForceDevice::init(Collision *sCollider) {
    spritesheet.loadFromFile("images/player/force-pit-beam.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
    spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(20, 20), glm::vec2(1.0f/10.0f, 1.0f/16.0f), &spritesheet, projection);
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
    collider->changePositionAbsolute(glm::vec2(posForce.x, posForce.y));

    if (isAtached) targetPosition = posForce;
    else {
        targetPosition.x = rightLimit;
        targetPosition.y = posForce.y;
    }

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(posForce.x, posForce.y));

    shipCollider = sCollider;
}

void ForceDevice::update(int deltaTime) {

    if (Game::instance().getKey('h') && !latchKeys['h']) {
        latchKeys['h'] = true;
        if (forceLevel < 2) forceLevel++;
        setForceLevel(forceLevel);
    } else if (Game::instance().getKey('g') && !latchKeys['g']) {
        latchKeys['g'] = true;
        if (forceLevel > 0) forceLevel--;
        setForceLevel(forceLevel);
    }
    else if (Game::instance().getKey('z') && !latchKeys['z']) {
        latchKeys['z'] = true;
        
        if (isAtached) {
            if (isLeft) targetPosition.x = leftLimit;
            else targetPosition.x = rightLimit;
        }
        else {
            if (isLeft) targetPosition.x = rightLimit;
            else targetPosition.x = leftLimit;
        }

        if (!isAtached) isLeft = !isLeft;
        isAtached = false;
    }
    else if (Game::instance().getKey('x') && !latchKeys['x']) {
        latchKeys['x'] = true;
    }

    if (isAtached) {
        targetPosition = shipCollider->position + getOffsetofColliders(isLeft);
    }
    else {
        targetPosition.y = shipCollider->position.y + getOffsetofColliders(isLeft).y;
    }

    if (abs(targetPosition.x - posForce.x) >= horizontalVelocity ) {
        int sign = (targetPosition.x >= posForce.x) ? ((targetPosition.x - posForce.x > 0.0f) ? 1 : -1) : ((posForce.x - targetPosition.x > 0.0f) ? -1 : 1);
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::ivec2(sign * horizontalVelocity, 0));

        if (!info.colliding) {
            posForce.x += sign * horizontalVelocity;
            collider->changePositionRelative(glm::ivec2(sign * horizontalVelocity, 0));
        } 
    }

    if (abs(targetPosition.y - posForce.y) >= verticalVelocity) {
        int sign = (targetPosition.y >= posForce.y) ? ((targetPosition.y - posForce.y > 0.0f) ? 1 : -1) : ((posForce.y - targetPosition.y > 0.0f) ? -1 : 1);
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::ivec2(0, sign * verticalVelocity));

        if (!info.colliding) {
            posForce.y += sign * verticalVelocity;
            collider->changePositionRelative(glm::ivec2(0, sign * verticalVelocity));
        } 
    }

    CollisionSystem::CollisionInfo infoAttach = collisionSystem->isTriggering(collider, glm::ivec2(0, 0));
    if (infoAttach.triggered && infoAttach.collider->collisionGroup == Collision::Player) attachToASide();

    if (isAtached && abs(targetPosition.y - posForce.y) <= 10.0f && abs(targetPosition.x - posForce.x) <= 10.0f) {
        posForce = targetPosition;
        collider->changePositionAbsolute(posForce);
    }

    sprite->setPosition(glm::vec2(posForce.x, posForce.y));
    sprite->update(deltaTime);

    if (!Game::instance().getKey('h') && latchKeys['h']) latchKeys['h'] = false;
    else if (!Game::instance().getKey('g') && latchKeys['g']) latchKeys['g'] = false;
    else if (!Game::instance().getKey('z') && latchKeys['z']) latchKeys['z'] = false;
    else if (!Game::instance().getKey('x') && latchKeys['x']) latchKeys['x'] = false;
}

void ForceDevice::render() {
    sprite->render();

#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES
}

void ForceDevice::setPosition(const glm::vec2 &pos) {
    posForce = pos;
    sprite->setPosition(glm::vec2(posForce.x, posForce.y));
    collider->changePositionAbsolute(glm::vec2(posForce.x, posForce.y));
    targetPosition = pos;
}

Collision* ForceDevice::getCollider() {
    return collider;
}

void ForceDevice::setForceLevel(int level) {
    sprite->changeAnimation(level,false);
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
    collider->changePositionAbsolute(targetPosition);
    sprite->setPosition(targetPosition);
    
}
