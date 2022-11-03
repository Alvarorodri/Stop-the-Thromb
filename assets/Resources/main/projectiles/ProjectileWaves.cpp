#include "ProjectileWaves.h"
#include "GeneralDefines.h"

ProjectileWaves::ProjectileWaves(glm::mat4 *project, int id) {
    projection = project;
    idProjectile = id;
    // TODO: the collision type must be set depending of the class that called this method
    collider = new Collision(id, project, Collision::PlayerProjectiles);

    collisionSystem = CollisionSystem::getInstance();
}

void ProjectileWaves::init(Texture *spritesheet, int type) {

    glm::vec2 spriteCuts = glm::vec2(1.0 / 9.0, 1.0 / 5.0);
    sprite = Sprite::createSprite(glm::ivec2(64, 64), spriteCuts, spritesheet, projection);
    sprite->setNumberAnimations(2);

    auxSprite1 = Sprite::createSprite(glm::ivec2(64, 64), spriteCuts, spritesheet, projection);
    auxSprite1->setNumberAnimations(2);

    auxSprite2 = Sprite::createSprite(glm::ivec2(64, 64), spriteCuts, spritesheet, projection);
    auxSprite2->setNumberAnimations(2);

    projectileConfigurator((ProjectileType)type, spriteCuts);
    sprite->changeAnimation(SpawningInitial, false);
    auxSprite1->changeAnimation(0, false);
    auxSprite2->changeAnimation(0, false);

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(posProjectile);
    auxSprite1->setPosition(posProjectile);
    auxSprite2->setPosition(posProjectile);
}

void ProjectileWaves::update(int deltaTime) {
    if (posProjectile.x >= 500.0f || posProjectile.y >= 300.0f || posProjectile.y < 0.0f) {
        ProjectileFactory::getInstance()->destroyProjectile(idProjectile);
        return;
    }

    if (sprite != NULL && (wavesState)sprite->animation() == SpawningInitial && sprite->isHalfFinidhedAnimation()) {
        posProjectile.x += 16.0f;
        collisionSystem->updateCollider(collider, posProjectile);
        collider->changePositionAbsolute(posProjectile);

        auxSprite1->changeAnimation(SpawningSections, false);
        auxSprite1->setPosition(posProjectile);
    }

    if (sprite == NULL) {
        if (auxSprite1->isHalfFinidhedAnimation()) {
            posProjectile.x += 12.0f;
            collisionSystem->updateCollider(collider, posProjectile);
            collider->changePositionAbsolute(posProjectile);
			posProjectile.x += 12.0f;

            auxSprite2->changeAnimation(SpawningSections, false);
            auxSprite2->setPosition(posProjectile);
        }
        if (auxSprite2->isHalfFinidhedAnimation()) {
			posProjectile.x += 12.0f;
            collisionSystem->updateCollider(collider, posProjectile);
            collider->changePositionAbsolute(posProjectile);
			posProjectile.x += 12.0f;

            auxSprite1->changeAnimation(SpawningSections, false);
            auxSprite1->setPosition(posProjectile);
        }

		if (auxSprite1->isFinidhedAnimation()) {
			auxSprite1->changeAnimation(0, false);
			collisionSystem->updateCollider(collider, posProjectile);
			collider->changePositionAbsolute(posProjectile);
		}
		if (auxSprite2->isFinidhedAnimation()) {
			auxSprite2->changeAnimation(0, false);
			collisionSystem->updateCollider(collider, posProjectile);
			collider->changePositionAbsolute(posProjectile);
		}
    }


    if (sprite != NULL && sprite->isFinidhedAnimation()) sprite = NULL;
    if (sprite != NULL) {
        sprite->update(deltaTime);
    }

    auxSprite1->update(deltaTime);
    auxSprite2->update(deltaTime);
}

void ProjectileWaves::render() {
    if (sprite != NULL) sprite->render();

    auxSprite1->render();
    auxSprite2->render();

#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES
}

void ProjectileWaves::setPosition(const glm::vec2 &pos) {
	posProjectile = pos;
	posProjectile.y -= 28.0f;
	sprite->setPosition(posProjectile);
	collisionSystem->updateCollider(collider, posProjectile);
	collider->changePositionAbsolute(posProjectile);
}

void ProjectileWaves::projectileConfigurator(ProjectileType type, const glm::vec2 &xy) {
    collider->changePositionAbsolute(glm::vec2(0.0f, 0.0f));

    sprite->setAnimationSpeed(SpawningInitial, 32);
    for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < 9; ++i) {
            sprite->addKeyframe(SpawningInitial, glm::vec2(xy.x * float(i), xy.y * float(j)));
        }
    }

    auxSprite1->setAnimationSpeed(0, 1);
    auxSprite1->addKeyframe(0, glm::vec2(xy.x * 8.0, xy.y * 1.0));
    auxSprite1->setAnimationSpeed(SpawningSections, 32);
    for (int j = 2; j < 5; ++j) {
        for (int i = 0; i < 9; ++i) {
            if (j == 4 && i == 8) break;
            auxSprite1->addKeyframe(SpawningSections, glm::vec2(xy.x * float(i), xy.y * float(j)));
        }
    }

    auxSprite2->setAnimationSpeed(0, 1);
    auxSprite2->addKeyframe(0, glm::vec2(xy.x * 8.0, xy.y * 1.0));
    auxSprite2->setAnimationSpeed(SpawningSections, 32);
    for (int j = 2; j < 5; ++j) {
        for (int i = 0; i < 9; ++i) {
            if (j == 4 && i == 8) break;
            auxSprite2->addKeyframe(SpawningSections, glm::vec2(xy.x * float(i), xy.y * float(j)));
        }
    }

    collider->addCollider(glm::ivec4(0, 16, 52, 48));

    collisionSystem->addColliderIntoGroup(collider);
    collisionSystem->updateCollider(collider, glm::vec2(0.0f, 0.0f));
}

void ProjectileWaves::collisionRoutine() {
    collisionWait--;
    if (collisionWait == 0) {
        collisionWait = 0;
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, projVelocity);

        if (info.colliding) {
            if (info.collider->collisionGroup == Collision::Map) {
                ProjectileFactory::getInstance()->destroyProjectile(idProjectile);
            }
        }
    }
}