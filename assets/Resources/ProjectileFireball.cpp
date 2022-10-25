#include "ProjectileFireball.h"
#include "GeneralDefines.h"

ProjectileFireball::ProjectileFireball(glm::mat4 *project, int id) {
    projection = project;
    idProjectile = id;
    // TODO: the collision type must be set depending of the class that called this method
    collider = new Collision(project, Collision::PlayerProjectiles);

    collisionSystem = CollisionSystem::getInstance();
    collisionSystem->addColliderIntoGroup(collider);
}

void ProjectileFireball::init(Texture *spritesheet, int type) {

    glm::vec2 spriteCuts = glm::vec2(1.0 / 8.0, 1.0 / 8.0);
    sprite = Sprite::createSprite(glm::ivec2(32, 32), spriteCuts, spritesheet, projection);
    sprite->setNumberAnimations(1);

    projectileConfigurator((ProjectileType)type, spriteCuts);
    sprite->changeAnimation(0, false);

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(0.0f,0.0f));
}

void ProjectileFireball::update(int deltaTime) {
    if (posProjectile.x >= 500.0f || posProjectile.y >= 300.0f || posProjectile.y < 0.0f) {
        ProjectileFactory::getInstance()->destroyProjectile(idProjectile);
        return;
    }

    collisionRoutine();

    followMapShape();

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
}

void ProjectileFireball::collisionRoutine() {
    // RECORDER: it only destroy it self with gets out the scene
    /*
    collisionWait--;
    if (collisionWait == 0) {
        collisionWait = 3;
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, projVelocity);

        if (info.colliding) {
            if (info.collider->collisionGroup == Collision::Map) {
                ProjectileFactory::getInstance()->destroyProjectile(idProjectile);
            }
        }
    }*/
}

void ProjectileFireball::followMapShape() {
    CollisionSystem::CollisionInfo info = CollisionSystem::getInstance()->isColliding(collider, glm::vec2(0.0f,2.0f));

    if (!info.colliding) {
        posProjectile += glm::vec2(0.0f, 2.0f);
        collider->changePositionRelative(glm::vec2(0.0f, 2.0f));
    }
    else {
        info = CollisionSystem::getInstance()->isColliding(collider, glm::vec2(2.0f, 0.0f));

        if (!info.colliding) {
            posProjectile += glm::vec2(2.0f, 0.0f);
            collider->changePositionRelative(glm::vec2(2.0f, 0.0f));
        }
        else {
            info = CollisionSystem::getInstance()->isColliding(collider, glm::vec2(0.0f, -2.0f));

            if (!info.colliding) {
                posProjectile += glm::vec2(0.0f, -2.0f);
                collider->changePositionRelative(glm::vec2(0.0f, -2.0f));
            }
            else {
                ProjectileFactory::getInstance()->destroyProjectile(idProjectile);
            }
        }
    }
}