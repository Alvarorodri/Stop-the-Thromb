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
    collider->changePositionAbsolute(posProjectile);
}

void Projectile::setVelocity(const glm::vec2 &vel) {
    projVelocity = vel;
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
    delete collider;
}