#include "PowerUp.h"
#include "Game.h"
#include "GeneralDefines.h"

PowerUp::PowerUp(glm::mat4 *project) {
    projection = project;
    collider = new Collision(project, Collision::Uknown);

    collisionSystem = CollisionSystem::getInstance();
    collisionSystem->addColliderIntoGroup(collider);
}

void PowerUp::init(const PowerUp::PowerUpType type) {
    spritesheet.loadFromFile("images/misc/power-ups.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
    spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(PowerUp::size, PowerUp::size), glm::vec2(1.0/8.0, 1.0), &spritesheet, projection);
    sprite->setNumberAnimations(8);

        for (int i = 0; i < 8; ++i) {
            sprite->setAnimationSpeed(i, 8);
            sprite->addKeyframe(i, glm::vec2((1.0 / 8.0) * i, 0.f));
        }

    sprite->changeAnimation(type, false);

    collider->addCollider(glm::ivec4(0, 0, PowerUp::size, PowerUp::size));
    collider->changePositionAbsolute(glm::ivec2(posPowerUp.x, posPowerUp.y));

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(posPowerUp);
}

void PowerUp::update(int deltaTime) {
    sprite->update(deltaTime);
}

void PowerUp::setType(const PowerUp::PowerUpType type) {
    sprite->changeAnimation(type, false);
}

void PowerUp::render() {
    sprite->render();

#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES
}

void PowerUp::setPosition(const glm::vec2 &pos) {
    posPowerUp = pos;
    sprite->setPosition(posPowerUp);
    collider->changePositionAbsolute(posPowerUp);
}
