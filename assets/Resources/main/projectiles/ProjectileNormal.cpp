#include "ProjectileNormal.h"
#include "GeneralDefines.h"
#include "Game.h"

ProjectileNormal::ProjectileNormal(glm::mat4 *project, int id, int type) {
    projection = project;
    idProjectile = id;
    // TODO: the collision type must be set depending of the class that called this method
    collider = new Collision(id, project, (ProjectileType)type == ProjectileType::EnemyProjectile ? Collision::EnemyProjectiles : Collision::PlayerProjectiles);

    collisionSystem = CollisionSystem::getInstance();
}

void ProjectileNormal::init(Texture *spritesheet, int type) {
	projectileType = (ProjectileType)type;

    glm::vec2 spriteCuts = glm::vec2(1.0 / 8.0, 1.0 / 32.0);
    sprite = Sprite::createSprite(glm::ivec2(32, 8), spriteCuts, spritesheet, projection);
    sprite->setNumberAnimations(1);

    projectileConfigurator(projectileType, spriteCuts);
    sprite->changeAnimation(0, false);

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

	posProjectile = glm::vec2(0.0f);
    sprite->setPosition(glm::vec2(0.0f,0.0f));
}

void ProjectileNormal::update(int deltaTime) {
    posProjectile += projVelocity;
    collider->changePositionAbsolute(posProjectile);

	if (!collisionRoutine()) return;

    sprite->setPosition(posProjectile);

    sprite->update(deltaTime);

	Projectile::update(deltaTime);
}

void ProjectileNormal::render() {
    sprite->render();

#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES
}

void ProjectileNormal::projectileConfigurator(ProjectileType type, const glm::vec2 &xy) {
    sprite->setAnimationSpeed(0, 8);
    collider->changePositionAbsolute(glm::vec2(0.0f, 0.0f));

    switch (type) {
    case BombProjectile:
        sprite->addKeyframe(0, glm::vec2(xy.x * 0.0, xy.y * 0.0));
        collider->addCollider(glm::ivec4(0, 0, 12, 6));
        break;
    case ForceProjectile:
        sprite->addKeyframe(0, glm::vec2(xy.x * 1.0, xy.y * 0.0));
        sprite->addKeyframe(0, glm::vec2(xy.x * 2.0, xy.y * 0.0));
        collider->addCollider(glm::ivec4(0, 0, 16, 8));
        break;
    case R9mk0:
        sprite->addKeyframe(0, glm::vec2(xy.x * 3.0, xy.y * 0.0));
        collider->addCollider(glm::ivec4(0, 3, 8, 4));
        break;
    case R9mk1:
        sprite->addKeyframe(0, glm::vec2(xy.x * 4.0, xy.y * 0.0));
        collider->addCollider(glm::ivec4(0, 1, 12, 6));
        break;
    case R9mk2:
        sprite->addKeyframe(0, glm::vec2(xy.x * 5.0, xy.y * 0.0));
        collider->addCollider(glm::ivec4(0, 0, 15, 6));
        break;
    case R9mk3:
        sprite->addKeyframe(0, glm::vec2(xy.x * 6.0, xy.y * 0.0));
        collider->addCollider(glm::ivec4(0, 0, 27, 6));
        break;
    case EnemyProjectile:
        sprite->addKeyframe(0, glm::vec2(xy.x * 0.0, xy.y * 4.0));
        sprite->addKeyframe(0, glm::vec2(xy.x * 1.0, xy.y * 4.0));
        collider->addCollider(glm::ivec4(1, 1, 7, 7));
        break;
    }

    collisionSystem->addColliderIntoGroup(collider);
    collisionSystem->updateCollider(collider, glm::vec2(0.0f, 0.0f));
}

bool ProjectileNormal::collisionRoutine() {
	if (!Projectile::collisionRoutine()) return false;

	collisionWait--;
    if (collisionWait <= 0) {
        collisionWait = 0;
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, projVelocity);
		
        if (info.colliding) {
			switch (info.collider->collisionGroup) {
			case Collision::Map:
				ProjectileFactory::getInstance()->destroyProjectile(idProjectile);
				return false;
				break;
			case Collision::Enemy:
				if (projectileType != ProjectileType::EnemyProjectile) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
				}
				break;
			case Collision::Player:
				if (projectileType == ProjectileType::EnemyProjectile) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					
				}
				break;
			}
        }
    }
	return true;
}