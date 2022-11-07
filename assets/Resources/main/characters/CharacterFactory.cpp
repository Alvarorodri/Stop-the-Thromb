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

void CharacterFactory::lateDestroyCharacter() {
	for (auto it = pendingToBeDestroyed.begin(); it != pendingToBeDestroyed.end(); ++it) {
		if (!IdreservedBoss.empty() && *it == (IdreservedBoss[0] - 1))IdreservedBoss.clear();
		else if (!IdreservedWorm1.empty() && *it == (IdreservedWorm1[0] - 1))IdreservedWorm1.clear();
		else if (!IdreservedWorm2.empty() && *it == (IdreservedWorm2[0] - 1))IdreservedWorm2.clear();
		characters[*it]->deleteRoutine();

		if (characters[*it] == player) player = nullptr;

		delete characters[*it];
		characters.erase(*it);
	}
	pendingToBeDestroyed.clear();

	for (auto it = pendingToBeKilled.begin(); it != pendingToBeKilled.end(); ++it) {
		if (characters.find(*it) != characters.end()) {
			characters[*it]->deleteRoutine();

			if (characters[*it] == player) player = nullptr;

			delete characters[*it];
			characters.erase(*it);
		}
	}
	pendingToBeKilled.clear();
}

void CharacterFactory::init() {

}

void CharacterFactory::update(int deltaTime) {
	for (auto it = characters.begin(); it != characters.end(); it++) {
		it->second->update(deltaTime);
	}

	spawnRoutine();
	lateDestroyCharacter();
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
			player = new Player(projection, last_id, tileMapPos);
			player->setPosition(pos);
			character = player;
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
		++last_id;
		IdreservedBoss.push_back(last_id);
		++last_id;
		IdreservedBoss.push_back(last_id);
		break;
	case cWormDown:
	case cWormUp:
		character = new Worm(projection, last_id, (type == cWormUp) ? true : false, IdreservedBoss[0]-1);
		if (IdreservedWorm1.empty()) {
			for (int i = 0; i < 9; ++i) {
				last_id += 1;
				IdreservedWorm1.push_back(last_id);
			}
		}
		else {
			for (int i = 0; i < 9; ++i) {
				last_id += 1;
				IdreservedWorm2.push_back(last_id);
			}
		}
		break;
	default:
		character = new Enemy1(projection, last_id, tileMapPos);
		character->setPosition(pos);
		break;
	}
	if (character != nullptr)characters[aux] = character;
	++last_id;


}

void CharacterFactory::spawnRoutine() {
	float x1 = COORD_VIEW_LIMIT_X;
	float x2 = x1 + 25.0f;
	float mapOffset = mapa->getPosition();

	if (nextSpawn >= (int)enemies.size()) return;

	glm::vec2 tempPos = enemies[nextSpawn].second;
	tempPos.x += mapOffset;

	// Loop to discard all the enemies that can not spawn because its position
	while (nextSpawn < (int)enemies.size() && tempPos.x < x1) {
		++nextSpawn;

		if (nextSpawn < (int)enemies.size()) {
			tempPos = enemies[nextSpawn].second;
			tempPos.x += mapOffset;
		}
	}

	// Actual loop that spawn the following enemy
	while (nextSpawn < (int)enemies.size() && tempPos.x >= x1 && tempPos.x <= x2) {
		spawnCharacter(enemies[nextSpawn].first, tempPos);

		++nextSpawn;
		if (nextSpawn < (int)enemies.size()) {
			tempPos = enemies[nextSpawn].second;
			tempPos.x += mapOffset;
		}
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
	IdreservedBoss.clear();
	IdreservedWorm1.clear();
	IdreservedWorm2.clear();
	nextSpawn = 0;
}

void CharacterFactory::destroyAllCharactersToEnd() {
	map<int, Character *>::iterator it = characters.begin();

	while (it != characters.end()) {
		pendingToBeDestroyed.insert(it->first);
		++it;
	}
	IdreservedBoss.clear();
	IdreservedWorm1.clear();
	IdreservedWorm2.clear();
	nextSpawn = 0;
}

void CharacterFactory::killCharacter(const int &id) {
	pendingToBeKilled.insert(id);
	auto it = characters.find(id);
	AudioManager::getInstance()->playSoundEffect(AudioManager::Explode, 128);
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
	else{
		bool found = false;
		for (int i = 0; i < IdreservedBoss.size();++i) {
			if (id == IdreservedBoss[i]) {
				if (i == 0) {
					auto search = characters.find((id - 1));
					if (search != characters.end()) search->second->damage(dmg,id);
					found = true;
				}
				if (i == 1 && !found) {
					auto search = characters.find(id - 2);
					if (search != characters.end()) search->second->damage(dmg, id);
					found = true;
				}
			}
		}
		if (!found) {
			for (int i = 0; i < IdreservedWorm1.size(); ++i) {
				if (id == IdreservedWorm1[i] && !found) {
					auto search = characters.find((id -i -1));
					if (search != characters.end()) search->second->damage(dmg, id);
					found = true;
				}
			}
			if (!found) {
				for (int i = 0; i < IdreservedWorm2.size(); ++i) {
					if (id == IdreservedWorm2[i] && !found) {
						auto search = characters.find((id - i - 1));
						if (search != characters.end()) search->second->damage(dmg, id);
						found = true;
					}
				}
			}
		}
			
	}
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

	if (!IdreservedWorm1.empty())destroyCharacter(IdreservedWorm1[0] - 1);
	if (!IdreservedWorm2.empty())destroyCharacter(IdreservedWorm2[0] - 1);
	ProjectileFactory::getInstance()->destroyAllProjectiles();
}
