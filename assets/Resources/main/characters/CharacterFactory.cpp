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

void CharacterFactory::init() {

}

void CharacterFactory::setProjection(glm::mat4 *project) {
	projection = project;
}

void CharacterFactory::spawnCharacter(int type, const glm::vec2 &pos) {

	Character *character = nullptr;
	switch (type) {
		case 0:
			//Player
			character = new Enemy1(projection, last_id, Collision::Enemy, tileMapPos);
			character->setPosition(pos);
			break;
		case 1:
			character = new Enemy1(projection, last_id, Collision::Enemy, tileMapPos);
			character->setPosition(pos);
			break;
		case 2:
			character = new Enemy2(projection, last_id, Collision::Enemy, tileMapPos);
			character->setPosition(pos);
			break;
		case 3:
			character = new Enemy3(projection, last_id, Collision::Enemy, tileMapPos);
			character->setPosition(pos);
			break;
		case 4:
			character = new Enemy4(projection, last_id, Collision::Enemy, tileMapPos);
			character->setPosition(pos);
			break;
		default:
			character = new Enemy1(projection, last_id, Collision::Enemy, tileMapPos);
			character->setPosition(pos);
			break;
	}
	characters[last_id] = character;
	++last_id;


}

void CharacterFactory::update(int deltaTime) {
	for (auto it = characters.begin(); it != characters.end(); it++) {
		it->second->update(deltaTime);
	}
}

void CharacterFactory::render() {
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
