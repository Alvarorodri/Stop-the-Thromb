#include "ForceDevice.h"
#include "Game.h"
#include "GeneralDefines.h"

ForceDevice::ForceDevice(glm::mat4 *project) {
    projection = project;

    collider = new Collision(project, Collision::Player);

    collisionSystem = CollisionSystem::getInstance();
    collisionSystem->addColliderIntoGroup(collider);

    forceLevel = 0;
}

void ForceDevice::init(const glm::ivec2 &tileMapPos) {
    spritesheet.loadFromFile("images/player/force-pit-beam.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
    spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.0f/10.0f, 1.0f/16.0f), &spritesheet, projection);
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

    collider->addCollider(glm::vec4(0, 4, 16, 32-4));
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
            if (targetPosition.x > shipPosition.x) targetPosition.x = rightLimit;
            else targetPosition.x = leftLimit;
        }
        else {
            targetPosition.x = shipPosition.x;
            if (targetPosition.x > shipPosition.x) targetPosition.x += shipOffset;
            else targetPosition.x -= shipOffset;
        }

        isAtached = !isAtached;
    }

    if ((targetPosition.x > posForce.x) || (targetPosition.x < posForce.x)) {
        int sign = (targetPosition.x >= posForce.x) ? ((targetPosition.x - posForce.x > 0.0f) ? 1 : -1) : ((posForce.x - targetPosition.x > 0.0f) ? -1 : 1);
        if (!collisionSystem->isColliding(collider, glm::ivec2(sign * horizontalVelocity, 0))) {
            posForce.x += sign * horizontalVelocity;
            collider->changePositionRelative(glm::ivec2(sign * horizontalVelocity, 0));
        }
    }

    if (targetPosition.y > posForce.y || targetPosition.y < posForce.y) {
        int sign = (targetPosition.y >= posForce.y) ? ((targetPosition.y - posForce.y > 0.0f) ? 1 : -1) : ((posForce.y - targetPosition.y > 0.0f) ? -1 : 1);
        if (!collisionSystem->isColliding(collider, glm::ivec2(0, sign * verticalVelocity))) {
            posForce.y += sign * verticalVelocity;
            collider->changePositionRelative(glm::ivec2(0, sign * verticalVelocity));
        }
    }


    sprite->setPosition(glm::vec2(posForce.x, posForce.y));
    sprite->update(deltaTime);

    if (!Game::instance().getKey('h') && latchKeys['h']) latchKeys['h'] = false;
    else if (!Game::instance().getKey('g') && latchKeys['g']) latchKeys['g'] = false;
    else if (!Game::instance().getKey('z') && latchKeys['z']) latchKeys['z'] = false;
}

void ForceDevice::render() {
    sprite->render();

#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES
}

void ForceDevice::setPosition(const glm::vec2 &pos, bool initial) {
    shipPosition = pos;

    if (initial) {
        posForce = pos;
        sprite->setPosition(glm::vec2(posForce.x, posForce.y));
        collider->changePositionAbsolute(glm::vec2(posForce.x, posForce.y));
        targetPosition = pos;
    } else {
        targetPosition.y = pos.y;
    }
}

Collision* ForceDevice::getCollider() {
    return collider;
}

void ForceDevice::setForceLevel(int level) {
    sprite->changeAnimation(level,false);
}
