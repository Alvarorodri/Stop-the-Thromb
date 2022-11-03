#include "ProjectileFireball.h"
#include "GeneralDefines.h"

ProjectileFireball::ProjectileFireball(glm::mat4 *project, int id) {
    projection = project;
    idProjectile = id;
    // TODO: the collision type must be set depending of the class that called this method
    collider = new Collision(id, project, Collision::PlayerProjectiles);

    collisionSystem = CollisionSystem::getInstance();
}

void ProjectileFireball::init(Texture *spritesheet, int type) {

    glm::vec2 spriteCuts = glm::vec2(1.0 / 8.0, 1.0 / 8.0);
    sprite = Sprite::createSprite(glm::ivec2(32, 32), spriteCuts, spritesheet, projection);
    sprite->setNumberAnimations(1);

    projectileConfigurator((ProjectileType)type, spriteCuts);
    sprite->changeAnimation(0, false);

    lastMovement = projVelocity.y > 0.0f ? Down : Up;

	mapSpeed = ProjectileFactory::getInstance()->mapSpeed;

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(0.0f,0.0f));
}

void ProjectileFireball::update(int deltaTime) {
    collisionRoutine();

    if (!followMapShape()) return;

    sprite->setPosition(posProjectile);

    sprite->update(deltaTime);
}

void ProjectileFireball::render() {
    sprite->render();

#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES
}

void ProjectileFireball::projectileConfigurator(ProjectileType type, const glm::vec2 &xy) {
    sprite->setAnimationSpeed(0, 8);
    collider->changePositionAbsolute(glm::vec2(0.0f, 0.0f));

    switch (type) {
    case Fireball:
        sprite->addKeyframe(0, glm::vec2(xy.x * 0.0, xy.y * 2.0));
        sprite->addKeyframe(0, glm::vec2(xy.x * 1.0, xy.y * 2.0));
        sprite->addKeyframe(0, glm::vec2(xy.x * 2.0, xy.y * 2.0));
        sprite->addKeyframe(0, glm::vec2(xy.x * 3.0, xy.y * 2.0));

        collider->addCollider(glm::ivec4(0, 0, 12, 13));
        break;
    }

    collisionSystem->addColliderIntoGroup(collider);
    collisionSystem->updateCollider(collider, glm::vec2(0.0f, 0.0f));
}

void ProjectileFireball::collisionRoutine() {
    if (posProjectile.x >= 500.0f || posProjectile.y >= 256.0f || posProjectile.y < 0.0f || posProjectile.x < -50.0f) {
        ProjectileFactory::getInstance()->destroyProjectile(idProjectile);
        return;
    }
}

bool ProjectileFireball::followMapShape() {
    bool movementFound = false;
    int curMovement = lastMovement;
    CollisionSystem::CollisionInfo info;

    int sign = projVelocity.y < 0.0f ? -1 : 1;

    while (!movementFound) {

        int updateMovement = 0;
        switch ((Movements)curMovement) {
            case Down:
                info = CollisionSystem::getInstance()->isColliding(collider, glm::vec2(0.0f + mapSpeed, float(sign) * projVelocity.y));

                if (!info.colliding) {
                    posProjectile += glm::vec2(0.0f + mapSpeed, float(sign) * projVelocity.y);
                    collisionSystem->updateCollider(collider, posProjectile);
                    collider->changePositionAbsolute(posProjectile);
                    movementFound = true;

                    updateMovement = (sign == 1) ? 0 : +1;
                    lastMovement = curMovement + updateMovement;
                }
                break;
            case Right:
                info = CollisionSystem::getInstance()->isColliding(collider, glm::vec2(projVelocity.x + mapSpeed, 0.0f));

                if (!info.colliding) {
                    posProjectile += glm::vec2(projVelocity.x + mapSpeed, 0.0f);
                    collisionSystem->updateCollider(collider, posProjectile);
                    collider->changePositionAbsolute(posProjectile);
                    movementFound = true;

                    updateMovement = (sign == 1) ? -1 : +1;
                    lastMovement = curMovement + updateMovement;
                }
                break;
            case Up:
                info = CollisionSystem::getInstance()->isColliding(collider, glm::vec2(0.0f + mapSpeed, float(sign) * -projVelocity.y));

                if (!info.colliding) {
                    posProjectile += glm::vec2(0.0f + mapSpeed, float(sign) * -projVelocity.y);
                    collisionSystem->updateCollider(collider, posProjectile);
                    collider->changePositionAbsolute(posProjectile);
                    movementFound = true;

                    updateMovement = (sign == 1) ? -1 : 0;
                    lastMovement = curMovement + updateMovement;
                }
                break;
            case Left:
                info = CollisionSystem::getInstance()->isColliding(collider, glm::vec2(-projVelocity.x + mapSpeed, 0.0f));

                if (!info.colliding) {
                    posProjectile += glm::vec2(-projVelocity.x + mapSpeed, 0.0f);
                    collisionSystem->updateCollider(collider, posProjectile);
                    collider->changePositionAbsolute(posProjectile);
                    movementFound = true;

                    updateMovement = (sign == 1) ? -1 : +1;
                    lastMovement = curMovement + updateMovement;
                }
                break;
        }

        curMovement += (sign == 1) ? +1 : -1;
        if (curMovement == 4 && sign == 1) curMovement = 0;
        else if (curMovement == -1 && sign == -1) curMovement = 3;

        // If it returns to the same movement means that it tried all the posibilities
        if (!movementFound && curMovement == lastMovement) {
            ProjectileFactory::getInstance()->destroyProjectile(idProjectile);
            return false;
        }
    }
    return true;
}