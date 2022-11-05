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
            projectile = new ProjectileNormal(projection, last_id, type);
            projectile->init(TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Projectiles), type);
			projectile->setType(type);
            projectile->setPosition(pos);
            projectile->setVelocity(vel);
            projectile->setBounciness(bounce);
            break;
        case Projectile::Fireball:
            projectile = new ProjectileFireball(projection, last_id);
            projectile->init(TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Projectiles), type);
			projectile->setType(type);
            projectile->setPosition(pos);
            projectile->setVelocity(vel);
            projectile->setBounciness(bounce);
            break;
        case Projectile::Waves:
            projectile = new ProjectileWaves(projection, last_id);
            projectile->init(TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Waves), type);
			projectile->setType(type);
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
	if(projectiles[id]->getType()== Projectile::EnemyProjectile){
		ExplosionFactory::getInstance()->spawnExplosion(Explosion::Explosions::ExplosionProyectileEnemy, projection, projectiles[id]->getPos(), projectiles[id]->getBox());

	}
	else {
		ExplosionFactory::getInstance()->spawnExplosion(Explosion::Explosions::ExplosionProyectilePlayer, projection, projectiles[id]->getPos(), projectiles[id]->getBox());
	}
}

void ProjectileFactory::destroyAllProjectiles() {
	map<int, Projectile *>::iterator it = projectiles.begin();

	while (it != projectiles.end()) {
		pendingToBeDestroyed.insert(it->first);
		++it;
	}
	last_id = 0;
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