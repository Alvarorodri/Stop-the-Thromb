#include "Projectile.h"
#include "Game.h"
#include "GeneralDefines.h"

Projectile::Projectile() {
}

void Projectile::init(Texture *spritesheet, int type) {
    throw exception("Not Implemented Method");
}

void Projectile::update(int deltaTime) {
	collisionSystem->updateCollider(collider, posProjectile);
}

void Projectile::render() {
    throw exception("Not Implemented Method");
}

Projectile::ProjectileType Projectile::getType() {
	return projType;
}

glm::vec2  Projectile::getPos() {
	return posProjectile;
}

glm::vec4  Projectile::getBox(){
	return collider->getBoundingBox();
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

bool Projectile::collisionRoutine() {
	if (posProjectile.x >= 500.0f || posProjectile.y >= 300.0f || posProjectile.y < -50.0f || posProjectile.y < -50.0f) {
		ProjectileFactory::getInstance()->destroyProjectile(idProjectile);
		return false;
	}

	return true;
}

void Projectile::deleteRoutine() {
    collisionSystem->removeColliderFromGroup(collider);
	delete sprite;
	delete collider;
}