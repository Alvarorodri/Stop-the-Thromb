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

void CharacterFactory::killCharacter(const int &id) {
	pendingToBeKilled.insert(id);
	auto it = characters.find(id);
	if (it != characters.end()) {
		ExplosionFactory::getInstance()->spawnExplosion(Explosion::Explosions::ExplosionNormal,projection,it->second->getPosition(),it->second->getBoundingBox());
	}
}

void CharacterFactory::lateDestroyCharacter() {
	for (auto it = pendingToBeDestroyed.begin(); it != pendingToBeDestroyed.end(); ++it) {
		characters[*it]->deleteRoutine();
		delete characters[*it];
		characters.erase(*it);
	}
	pendingToBeDestroyed.clear();
	for (auto it = pendingToBeKilled.begin(); it != pendingToBeKilled.end(); ++it) {
		if (characters.find(*it) != characters.end()) {
			characters[*it]->deleteRoutine();
			delete characters[*it];
			characters.erase(*it);
		}
	}
	pendingToBeKilled.clear();
}

void CharacterFactory::init() {

}

void CharacterFactory::setProjection(glm::mat4 *project) {
	projection = project;
}

void CharacterFactory::setSpawnFiles(string file) {
	ifstream fin;
	string line;
	stringstream sstream;
	int nrEntities;

	fin.open(file.c_str());
	if (!fin.is_open()) return;
	getline(fin, line);
	sstream.str(line);
	sstream >> nrEntities;

	for (int i = 0; i < nrEntities; ++i) {
		stringstream sstream1;
		pair<CharacterAvailable, glm::vec2> enemy;
		glm::vec2 coord;
		int type;

		getline(fin, line);
		sstream1.str(line);
		sstream1 >> type >> coord.x >> coord.y;

		enemy.first = (CharacterAvailable)type;
		enemy.second = coord;

		enemies.push_back(enemy);
	}

	fin.close();
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
		case cBoss:
			character = new Boss(projection, last_id, tileMapPos);
			character->setPosition(pos);
			break;
		case cWorm:
			character = new Worm(projection, last_id, tileMapPos);
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

	spawnRoutine();
	lateDestroyCharacter();
}

void CharacterFactory::render() {
	if (player != nullptr)player->render();
	for (auto it = characters.begin(); it != characters.end(); it++) {
		it->second->render();
	}
}

void CharacterFactory::damageEnemy(const int &id) {
	auto search = characters.find(id);
	if (search != characters.end()) search->second->damage();
}

void CharacterFactory::damagePlayer() {
	if(player != nullptr)player->damage();
}

void CharacterFactory::killPlayer() {
	delete player;
	player = nullptr;
}

void CharacterFactory::setTileMapPos(const glm::vec2 &pos){
	tileMapPos = pos;
}

void CharacterFactory::setMap(TileMap *map) {
	mapa = map;
}

void CharacterFactory::spawnRoutine() {
	float x1 = COORD_VIEW_LIMIT_X;
	float x2 = x1 + 25.0f;
	float mapOffset = mapa->getPosition();

	vector<pair<CharacterAvailable, glm::vec2>>::iterator it = enemies.begin();
	while (it != enemies.end()) {
		glm::vec2 tempPos = it->second;
		tempPos.x += mapOffset;

		if (tempPos.x >= x1 && tempPos.x <= x2) {
			spawnCharacter(2, tempPos);

			enemies.erase(it);
			it = enemies.begin();
		}
		if (it != enemies.end() && it->second.x > x2) return;

		if (it != enemies.end()) ++it;
	}
}
