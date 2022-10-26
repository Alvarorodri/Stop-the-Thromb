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
    int group = int(a->collisionGroup);
    for (auto it = groups[group].begin(); it != groups[group].end(); ++it) {
        if (*it == a) {
            groups[group].erase(it);
            return;
        }
    }
}

bool CollisionSystem::isValidCollision(const Collision* a, const Collision* b) {
    return collisionMatrix[a->collisionGroup][b->collisionGroup];
}

bool CollisionSystem::isTriggerCollision(const Collision* a, const Collision* b) {
    return triggersMatrix[a->collisionGroup][b->collisionGroup];
}

CollisionSystem::CollisionInfo CollisionSystem::isColliding(const Collision* a, const glm::vec2 &offset) {
    for (int i = 0; i < (int)groups.size(); ++i) {
        for (int j = 0; j < (int)groups[i].size(); ++j) {
            if (isValidCollision(a, groups[i][j])) {
                if (searchForCollision(a, groups[i][j], offset)) {
                    return CollisionInfo{ 
                                isValidCollision(a, groups[i][j]), 
                                groups[i][j], 
                                false };
                }
            }
        }
    }

    return CollisionInfo{ false, NULL, false};
}

CollisionSystem::CollisionInfo CollisionSystem::isTriggering(const Collision* a, const glm::vec2 &offset) {
    for (int i = 0; i < (int)groups.size(); ++i) {
        for (int j = 0; j < (int)groups[i].size(); ++j) {
            if (isTriggerCollision(a, groups[i][j])) {
                if (searchForCollision(a, groups[i][j], offset)) {
                    return CollisionInfo{ 
                                false, 
                                groups[i][j], 
                                isTriggerCollision(a, groups[i][j]) };
                }
            }
        }
    }

    return CollisionInfo{ false, NULL, false };
}

bool CollisionSystem::searchForCollision(const Collision* a, const Collision* b, const glm::vec2 &offset) {
    for (int i = 0; i < a->collidersSize; ++i) {
        for (int j = 0; j < b->collidersSize; ++j) {
            glm::vec4 colliderA = a->collisions[i];
            glm::vec4 colliderB = b->collisions[j];
            glm::vec2 posA = a->position + offset;

            if ((posA.x + colliderA.x) >= 500.0f) {
                return false;
            } else if ((b->position.x + colliderB.x) >= 500.0f) {
                break;
            }

            if (overlapHorizontal(colliderA, colliderB, posA, b->position)) {
                return true;
            }
        }
    }

    return false;
}

bool CollisionSystem::overlapVertical(const glm::vec4 &a, const glm::vec4 &b, const glm::vec2 &posA, const glm::vec2 &posB) {
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
