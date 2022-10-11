#include "CollisionSystem.h"

CollisionSystem* CollisionSystem::instance_ = nullptr;

CollisionSystem *CollisionSystem::getInstance() {
    if (instance_ == nullptr) instance_ = new CollisionSystem();
    return instance_;
}

CollisionSystem::CollisionSystem() {
}

CollisionSystem::~CollisionSystem() {
}

void CollisionSystem::addColliderIntoGroup(Collision* a) {
    groups[int(a->collisionGroup)].push_back(a);
}

void CollisionSystem::removeColliderFromGroup(Collision* a) {
    // Unused
    /*int group = int(a->collisionGroup);
    for (int i = 0; i < groups[group].size(); ++i) {
        //Do this with iterator
        //if (groups[group][i] == a) groups.erase(i);
    }*/
}

bool CollisionSystem::isColliding(const Collision* a) {
    for (int i = 0; i < groups.size(); ++i) {
        if (i != int(a->collisionGroup)) {
            for (int j = 0; j < groups[i].size(); ++j) {
                if (searchForCollision(a, groups[i][j])) return true;
            }
        }
    }

    return false;
}

bool CollisionSystem::searchForCollision(const Collision* a, const Collision* b) {
    for (int i = 0; i < a->collidersSize; ++i) {
        for (int j = 0; j < b->collidersSize; ++j) {
            if (overlapHorizontal(a->collisions[i], b->collisions[j], a->position, b->position)) {
                return true;
            }
        }
    }

    return false;
}

bool CollisionSystem::overlapVertical(const glm::ivec4 &a, const glm::ivec4 &b, const glm::ivec2 &posA, const glm::ivec2 &posB) {
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

bool CollisionSystem::overlapHorizontal(const glm::ivec4 &a, const glm::ivec4 &b, const glm::ivec2 &posA, const glm::ivec2 &posB) {
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
