#include "ProjectileFactory.h"

ProjectileFactory* ProjectileFactory::instance_ = nullptr;

ProjectileFactory *ProjectileFactory::getInstance() {
    if (instance_ == nullptr) instance_ = new ProjectileFactory();
    return instance_;
}

ProjectileFactory::ProjectileFactory() {
}

ProjectileFactory::~ProjectileFactory() {
}

void ProjectileFactory::init() {
    last_id = 0;
    
    spritesheet1.loadFromFile("images/projectiles/projectiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet1.setWrapS(GL_CLAMP_TO_EDGE);
    spritesheet1.setWrapT(GL_CLAMP_TO_EDGE);
    spritesheet1.setMinFilter(GL_NEAREST);
    spritesheet1.setMagFilter(GL_NEAREST);
}

void ProjectileFactory::setProjection(glm::mat4 *project) {
    projection = project;
}

void ProjectileFactory::spawnProjectile(const glm::vec2 &pos, const glm::vec2 &vel, bool bounce, Projectile::ProjectileType type) {
    Projectile *projectile;
    switch (type) {
        case Projectile::BombProjectile:
        case Projectile::ForceProjectile:
        case Projectile::R9mk0:
        case Projectile::R9mk1:
        case Projectile::R9mk2:
        case Projectile::R9mk3:
        case Projectile::EnemyProjectile:
            projectile = new ProjectileNormal(projection, last_id);
            projectile->init(&spritesheet1, type);
            projectile->setPosition(pos);
            projectile->setVelocity(vel);
            projectile->setBounciness(bounce);
            break;
        case Projectile::Fireball:
            projectile = new ProjectileFireball(projection, last_id);
            projectile->init(&spritesheet1, type);
            projectile->setPosition(pos);
            projectile->setVelocity(vel);
            projectile->setBounciness(bounce);
            break;
    }

    projectiles.insert({ last_id, projectile });
    ++last_id;
}

void ProjectileFactory::destroyProjectile(const int &id) {
    pendingToBeDestroyed.insert(id);
}

void ProjectileFactory::lateDestroyProjectile() {
    for (auto it = pendingToBeDestroyed.begin(); it != pendingToBeDestroyed.end(); ++it) {
        projectiles[*it]->deleteRoutine();
        delete projectiles[*it];
        projectiles.erase(*it);
    }
    pendingToBeDestroyed.clear();
}

void ProjectileFactory::update(int deltaTime) {
    map<int, Projectile*>::iterator it = projectiles.begin();

    while (it != projectiles.end()) {
        it->second->update(deltaTime);
        if(it != projectiles.end()) ++it;
    }

    lateDestroyProjectile();
}

void ProjectileFactory::render() {
    for (auto it = projectiles.begin(); it != projectiles.end(); it++) {
        it->second->render();
    }
}