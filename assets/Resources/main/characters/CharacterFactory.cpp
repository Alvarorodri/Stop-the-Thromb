#include "CharacterFactory.h"
#include "Game.h"

CharacterFactory* CharacterFactory::instance_ = nullptr;

CharacterFactory *CharacterFactory::getInstance() {
	if (instance_ == nullptr) instance_ = new CharacterFactory();
	return instance_;
}

void CharacterFactory::deleteReference() {
	if (instance_ != nullptr) delete instance_;
	instance_ = nullptr;
}

CharacterFactory::CharacterFactory() {
	
}

CharacterFactory::~CharacterFactory() {
}

void CharacterFactory::lateDestroyCharacter() {
	for (auto it = pendingToBeDestroyed.begin(); it != pendingToBeDestroyed.end(); ++it) {
		characters[*it]->deleteRoutine();

		if (characters[*it] == player) player = nullptr;

		delete characters[*it];
		characters.erase(*it);
	}
	pendingToBeDestroyed.clear();

	for (auto it = pendingToBeKilled.begin(); it != pendingToBeKilled.end(); ++it) {
		if (characters.find(*it) != characters.end()) {
			characters[*it]->deleteRoutine();

			if (characters[*it] == player) { 
				player = nullptr; 
			}

			delete characters[*it];
			characters.erase(*it);
		}
	}
	pendingToBeKilled.clear();
	if (player == nullptr && alive) {
		alive = false;
		timer = 100;
		nextSpawn = 0;
	}
}

void CharacterFactory::init() {

}

void CharacterFactory::update(int deltaTime) {
	for (auto it = characters.begin(); it != characters.end(); it++) {
		it->second->update(deltaTime);
	}

	spawnRoutine();
	lateDestroyCharacter();
	if (timer != -1) {
		timer -= 1;
	}
	if (timer == 0) {
		timer = -1;
		nextSpawn = 0;

		Game::instance().changeToGame(true);
	}
}

void CharacterFactory::render() {
	map<int, Character *>::reverse_iterator it = characters.rbegin();
	while (it != characters.rend()) {
		it->second->render();
		++it;
	}
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

void CharacterFactory::setTileMapPos(const glm::vec2 &pos) {
	tileMapPos = pos;
}

void CharacterFactory::setMap(TileMap *map) {
	mapa = map;
}

bool CharacterFactory::getPlayerPos(glm::vec2 &pos) {
	if (player == nullptr) return false;

	pos = player->getPosition();
	return true;
}

int CharacterFactory::getHealthCharacter(int id) {
	auto it = characters.find(id);
	if (it != characters.end()) return it->second->getHealth();
	return 0;
}

void CharacterFactory::spawnCharacter(int type, const glm::vec2 &pos) {

	Character *character = nullptr;
	int aux = last_id;
	switch (type) {
	case cPlayer:
		if (player == nullptr) {
			alive = true;
			player = new Player(projection, last_id, tileMapPos);
			player->setPosition(pos);
			character = player;
		}
		break;
	case cEnemy1:
		character = new BloodEnemy1(projection, last_id, tileMapPos);
		character->setPosition(pos);
		break;
	case cEnemy2:
		character = new BloodEnemy2(projection, last_id, tileMapPos);
		character->setPosition(pos);
		break;
	default:
		character = new BloodEnemy1(projection, last_id, tileMapPos);
		character->setPosition(pos);
		break;
	}
	if (character != nullptr)characters[aux] = character;
	++last_id;


}

void CharacterFactory::spawnRoutine() {
    if (rand() % 100 == 1) {
		if ((rand() % 100 < 80)) spawnCharacter(cEnemy1, glm::vec2(470.0f, 240 - (rand() % 200)));
		else spawnCharacter(cEnemy2, glm::vec2(470.0f, 128.0f));
    }
}

void CharacterFactory::destroyCharacter(const int &id) {
	pendingToBeDestroyed.insert(id);
}

void CharacterFactory::destroyAllCharactersToTeleport() {
	map<int, Character *>::iterator it = characters.begin();

	while (it != characters.end()) {
		if (player != nullptr && it->first != player->getId()) pendingToBeDestroyed.insert(it->first);
		++it;
	}

	nextSpawn = 0;
}

void CharacterFactory::destroyAllCharactersToEnd() {
	map<int, Character *>::iterator it = characters.begin();
	enemies.clear();
	while (it != characters.end()) {
		pendingToBeDestroyed.insert(it->first);
		++it;
	}

	nextSpawn = 0;
}

void CharacterFactory::killCharacter(const int &id) {
	pendingToBeKilled.insert(id);
	auto it = characters.find(id);
	AudioManager::getInstance()->playSoundEffect(AudioManager::EnemyKilled, 128);
	if (it != characters.end()) {
		if(player != nullptr && player->getId() == id){
			ExplosionFactory::getInstance()->spawnExplosion(Explosion::Explosions::ExplosionPlayer, projection, it->second->getPosition(), it->second->getBoundingBox());
		}
		else ExplosionFactory::getInstance()->spawnExplosion(Explosion::Explosions::ExplosionEnemy,projection,it->second->getPosition(),it->second->getBoundingBox());
	}
}

void CharacterFactory::damageCharacter(const int &id, int dmg) {
	auto search = characters.find(id);
	if (search != characters.end()) search->second->damage(dmg,id);

}

void CharacterFactory::increasePlayerForce(int power) {
	if (player != nullptr) player->increaseForce(power);
}

void CharacterFactory::wormRetun(int idSource, int idDest, bool upOrDown) {
	auto it = characters.find(idDest);
	if (it != characters.end()) it->second->wormReturn(idSource, upOrDown);
}

void CharacterFactory::bossIsDead(bool dead) {
	bossdead = dead;
}

bool CharacterFactory::isBossDead() {
	return bossdead;
}

void CharacterFactory::exterminateWorms() {

	ProjectileFactory::getInstance()->destroyAllProjectiles();
}
