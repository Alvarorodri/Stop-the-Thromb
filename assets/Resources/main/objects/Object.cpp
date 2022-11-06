#include "Object.h"
#include "Game.h"
#include "GeneralDefines.h"

Object::Object(int id, glm::mat4 *project, const Object::ObjectType type) {
	this->id = id;
    projection = project;
    collider = new Collision(id,project, Collision::Object);
	this->type = type;

    collisionSystem = CollisionSystem::getInstance();
}

void Object::init() {

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(pos);
}

void Object::update(int deltaTime) {
    sprite->update(deltaTime);

	vel = glm::vec2(ObjectFactory::getInstance()->mapSpeed, 0.0f);
	setPosition(pos + vel);
}

void Object::render() {
    sprite->render();

#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES
}

void Object::setPosition(const glm::vec2 &pos) {
    this->pos = pos;
    sprite->setPosition(pos);
	collisionSystem->updateCollider(collider, pos);
    collider->changePositionAbsolute(pos);
}

void Object::setType(const Object::ObjectType type) {
	this->type = type;
	sprite->changeAnimation(type, false);
}

void Object::setSize(int size) {
	this->size = size;
}

bool Object::collisionRoutine() {
	return true;
}

void Object::deleteRoutine() {
	collisionSystem->removeColliderFromGroup(collider);
	delete collider;
}
