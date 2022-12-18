#include "ObjectFactory.h"

ObjectFactory* ObjectFactory::instance_ = nullptr;

ObjectFactory *ObjectFactory::getInstance() {
    if (instance_ == nullptr) instance_ = new ObjectFactory();
    return instance_;
}

void ObjectFactory::deleteReference() {
	if (instance_ != nullptr) delete instance_;
	instance_ = nullptr;
}

ObjectFactory::ObjectFactory() {
}

ObjectFactory::~ObjectFactory() {
}

void ObjectFactory::init() {
    last_id = 0;
}

void ObjectFactory::setProjection(glm::mat4 *project) {
    projection = project;
}

void ObjectFactory::spawnObject(const glm::vec2 &pos, Object::ObjectType type) {
    Object *object;
    switch (type) {
        case Object::PowerUpBlue:
		case Object::PowerUpGreen:
		case Object::PowerUpRed:
		case Object::PowerUpYellow:
		case Object::PowerUpBlack:
		case Object::ModifierS:
		case Object::ModifierMRed:
		case Object::ModifierMBlue:
            object = new PowerUp(last_id, projection, type);
			object->setSize(14);
			object->init();
			object->setPosition(pos);
			object->setType(type);
            break;
    }

    objects.insert({ last_id, object });
    ++last_id;
}

void ObjectFactory::destroyObject(const int &id) {
    pendingToBeDestroyed.insert(id);
}

void ObjectFactory::destroyAllObjects() {
	map<int, Object *>::iterator it = objects.begin();

	while (it != objects.end()) {
		pendingToBeDestroyed.insert(it->first);
		++it;
	}
	last_id = 0;
}

void ObjectFactory::lateDestroyObject() {
    for (auto it = pendingToBeDestroyed.begin(); it != pendingToBeDestroyed.end(); ++it) {
		objects[*it]->deleteRoutine();
        delete objects[*it];
		objects.erase(*it);
    }
    pendingToBeDestroyed.clear();
}

void ObjectFactory::update(int deltaTime) {
    map<int, Object*>::iterator it = objects.begin();

    while (it != objects.end()) {
        it->second->update(deltaTime);
        if(it != objects.end()) ++it;
    }

    lateDestroyObject();
}

void ObjectFactory::render() {
    for (auto it = objects.begin(); it != objects.end(); it++) {
        it->second->render();
    }
}