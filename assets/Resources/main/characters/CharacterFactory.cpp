#include "CharacterFactory.h"

CharacterFactory* CharacterFactory::instance_ = nullptr;

CharacterFactory *CharacterFactory::getInstance() {
	if (instance_ == nullptr) instance_ = new CharacterFactory();
	return instance_;
}

CharacterFactory::CharacterFactory() {
	
}

CharacterFactory::~CharacterFactory() {
}

void CharacterFactory::destroyCharacter(const int &id) {
	pendingToBeDestroyed.insert(id);
}
void CharacterFactory::lateDestroyCharacter() {
	for (auto it = pendingToBeDestroyed.begin(); it != pendingToBeDestroyed.end(); ++it) {
		characters[*it]->deleteRoutine();
		delete characters[*it];
		characters.erase(*it);
	}
	pendingToBeDestroyed.clear();
}

void CharacterFactory::init() {

}

void CharacterFactory::setProjection(glm::mat4 *project) {
	projection = project;
}

void CharacterFactory::spawnCharacter(int type, const glm::vec2 &pos) {

	Character *character = nullptr;
	switch (type) {
		case cPlayer:
			if (player==nullptr) {
				player = new Player(projection, last_id, tileMapPos);
				player->setPosition(pos);
			}
			break;
		case cEnemy1:
			character = new Enemy1(projection, last_id, tileMapPos);
			character->setPosition(pos);
			break;
		case cEnemy2:
			character = new Enemy2(projection, last_id, tileMapPos);
			character->setPosition(pos);
			break;
		case cEnemy3:
			character = new Enemy3(projection, last_id, tileMapPos);
			character->setPosition(pos);
			break;
		case cEnemy4:
			character = new Enemy4(projection, last_id, tileMapPos);
			character->setPosition(pos);
			break;
		default:
			character = new Enemy1(projection, last_id, tileMapPos);
			character->setPosition(pos);
			break;
	}
	if(character!=nullptr)characters[last_id] = character;
	++last_id;


}

void CharacterFactory::update(int deltaTime) {
	if(player != nullptr)player->update(deltaTime);
	for (auto it = characters.begin(); it != characters.end(); it++) {
		it->second->update(deltaTime);
	}
	lateDestroyCharacter();
}

void CharacterFactory::render() {
	if (player != nullptr)player->render();
	for (auto it = characters.begin(); it != characters.end(); it++) {
		it->second->render();
	}
}

void CharacterFactory::setTileMapPos(const glm::vec2 &pos){
	tileMapPos = pos;
}

void CharacterFactory::setMap(TileMap *map) {
	mapa = map;
}
