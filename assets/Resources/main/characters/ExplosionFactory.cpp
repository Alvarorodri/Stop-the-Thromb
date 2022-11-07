#include "ExplosionFactory.h"
#include "Game.h"
#include "GeneralDefines.h"

ExplosionFactory* ExplosionFactory::instance_ = nullptr;

ExplosionFactory *ExplosionFactory::getInstance() {
	if (instance_ == nullptr) instance_ = new ExplosionFactory();
	return instance_;
}

void ExplosionFactory::deleteReference() {
	if (instance_ != nullptr) delete instance_;
	instance_ = nullptr;
}

ExplosionFactory::ExplosionFactory() {

}

ExplosionFactory::~ExplosionFactory() {
}

void ExplosionFactory::spawnExplosion(Explosion::Explosions type, glm::mat4 *project, const glm::vec2 &pos, const glm::vec4 &box) {
	Explosion* explosion = new Explosion(type, *project, pos, box);
	explosion->setMap(map);
	explosions.insert({ lastId, explosion });
	lastId++;
}

void ExplosionFactory::update(int deltaTime)
{
	for (auto it = explosions.begin(); it != explosions.end(); ++it) {
		if (it->second->isEnded()) {
			explosionsToDelete.insert(it->first);
		}
		else it->second->update(deltaTime);
	}

	for (auto it = explosionsToDelete.begin(); it != explosionsToDelete.end(); ++it) {
		explosions.erase(*it);
	}
	explosionsToDelete.clear();
}

void ExplosionFactory::render() {
	for (auto it = explosions.begin(); it != explosions.end(); ++it) {
		it->second->render();
	}

}

void ExplosionFactory::setMap(TileMap* map) {
	this->map = map;
}

void ExplosionFactory::deleteAll() {
	std::map<int, Explosion*>::iterator it = explosions.begin();

	while (it != explosions.end()) {
		explosionsToDelete.insert(it->first);
		++it;
	}
	
}
