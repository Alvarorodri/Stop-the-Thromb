#include "CollisionSystem.h"

CollisionSystem* CollisionSystem::instance_ = nullptr;

CollisionSystem *CollisionSystem::getInstance() {
    if (instance_ == nullptr) instance_ = new CollisionSystem();
    return instance_;
}

CollisionSystem::CollisionSystem() {
	spatialHashmap = new SpatialHashmap(32);
}

CollisionSystem::~CollisionSystem() {
}

void CollisionSystem::addColliderIntoGroup(Collision* a) {
	spatialHashmap->insertObject(a);
}

void CollisionSystem::removeColliderFromGroup(Collision* a) {
	spatialHashmap->removeObject(a);
}

void CollisionSystem::updateCollider(Collision* a, const glm::vec2 &newPos) {
	spatialHashmap->updateObject(a, newPos);
}

bool CollisionSystem::isValidCollision(const Collision* a, const Collision* b) {
    return collisionMatrix[a->collisionGroup][b->collisionGroup];
}

bool CollisionSystem::isTriggerCollision(const Collision* a, const Collision* b) {
    return triggersMatrix[a->collisionGroup][b->collisionGroup];
}

CollisionSystem::CollisionInfo CollisionSystem::isColliding(Collision* a, const glm::vec2 &offset) {
	glm::vec4 box = a->getBoundingBox();

	glm::vec2 pos = glm::vec2((box.x+box.z)/2.0f, (box.y+box.w)/2.0f);
	int radius = (int)glm::distance(pos, glm::vec2(box.x,box.y)) + 1;

	pos.x += a->position.x;
	pos.y += a->position.y;

	int coll = a->collisionGroup;
    bool collidersGroup[10] = {false, false, false, false, false, false, false, false, false, false};

	for (int i = 0; i < 10/*Collision Matrix Size*/; ++i) {
		if (collisionMatrix[coll][i]) collidersGroup[i] = true;
	}

	vector<Collision*> objects = spatialHashmap->getNearByObjects(pos, radius, collidersGroup);

    for (int i = 0; i < (int)objects.size(); ++i) {
        if (searchForCollision(a, objects[i], offset)) {
            return CollisionInfo{ true, objects[i], false };
        }
    }

    return CollisionInfo{ false, NULL, false};
}

CollisionSystem::CollisionInfo CollisionSystem::isTriggering(Collision* a, const glm::vec2 &offset) {
	glm::vec4 box = a->getBoundingBox();

	glm::vec2 pos = glm::vec2((box.x + box.z) / 2.0f, (box.y + box.w) / 2.0f);
	int radius = (int)glm::distance(pos, glm::vec2(box.x, box.y)) + 1;

	pos.x += a->position.x;
	pos.y += a->position.y;

	int coll = a->collisionGroup;

    bool collidersGroup[10] = { false, false, false, false, false, false, false, false, false, false };

	for (int i = 0; i < 10/*Collision Matrix Size*/; ++i) {
		if (triggersMatrix[coll][i]) collidersGroup[i] = true;
	}

    vector<Collision*> objects = spatialHashmap->getNearByObjects(pos, radius, collidersGroup);

    for (int i = 0; i < (int)objects.size(); ++i) {
        if (searchForCollision(a, objects[i], offset)) {
            return CollisionInfo{ false, objects[i], true };
        }
    }
	
    return CollisionInfo{ false, NULL, false };
}

bool CollisionSystem::searchForCollision(const Collision* a, const Collision* b, const glm::vec2 &offset) {
    glm::vec2 posA = a->position + offset;

    if ((posA.x + a->colliderBox.x) >= 500.0f) {
        return false;
    } else if ((b->position.x + b->colliderBox.x) >= 500.0f) {
        return false;
    }

    if (overlapHorizontal(a->colliderBox, b->colliderBox, posA, b->position)) {
        return true;
    }

    return false;
}

bool CollisionSystem::overlapHorizontal(const glm::vec4 &a, const glm::vec4 &b, const glm::vec2 &posA, const glm::vec2 &posB) {
    // if one rectangle is on left side of other
    if (a[0] + posA.x >= b[2] + posB.x || b[0] + posB.x >= a[2] + posA.x) {
        return false;
    }

    // if one rectangle is above other
    if (a[3] + posA.y <= b[1] + posB.y || a[1] + posA.y >= b[3] + posB.y) {
        return false;
    }

    return true;
}
