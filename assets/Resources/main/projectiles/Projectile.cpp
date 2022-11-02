#include "Projectile.h"
#include "Game.h"
#include "GeneralDefines.h"

Projectile::Projectile() {
}

void Projectile::init(Texture *spritesheet, int type) {
    throw exception("Not Implemented Method");
}

void Projectile::update(int deltaTime) {
    throw exception("Not Implemented Method");
}

void Projectile::render() {
    throw exception("Not Implemented Method");
}

void Projectile::setPosition(const glm::vec2 &pos) {
    posProjectile = pos;
    sprite->setPosition(posProjectile);
    collisionSystem->updateCollider(collider, posProjectile);
    collider->changePositionAbsolute(posProjectile);
}

void Projectile::setVelocity(const glm::vec2 &vel) {
    projVelocity = vel;

    sprite->setBox(glm::vec2(collider->getBoundingBox().z, collider->getBoundingBox().w));
    collider->setBox(glm::vec2(collider->getBoundingBox().z, collider->getBoundingBox().w));
    sprite->setRotation(glm::vec3(0.0f,0.0f,atan2(vel.y, vel.x) * (180 / PI)));
    collider->setRotation(glm::vec3(0.0f, 0.0f, atan2(vel.y, vel.x) * (180 / PI)));
}

void Projectile::setType(const ProjectileType type) {
    projType = type;
}

void Projectile::setBounciness(const bool bounce) {
    projBounce = bounce;
}

void Projectile::collisionRoutine() {
    throw exception("Not Implemented Method");
}

void Projectile::deleteRoutine() {
    collisionSystem->removeColliderFromGroup(collider);
	sprite->free();
	delete sprite;
	delete collider;
}