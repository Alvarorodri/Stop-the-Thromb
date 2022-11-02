#ifndef _CHARACTER_FACTORY_INCLUDE
#define _CHARACTER_FACTORY_INCLUDE

#include <map>

#include "Character.h"
#include "enemies\Enemy1.h"
#include "enemies\Enemy2.h"
#include "enemies\Enemy3.h"
#include "enemies\Enemy4.h"
#include "enemies\Boss.h"
#include "enemies\Worm.h"
#include "player\Player.h"
#include "ExplosionFactory.h"

class CharacterFactory {

private:

	static CharacterFactory *instance_;
	CharacterFactory();
	~CharacterFactory();
	void lateDestroyCharacter();

public:
	enum CharacterAvailable{cPlayer, cEnemy1, cEnemy2, cEnemy3, cEnemy4, cBoss, cWorm};
	Player *player = nullptr;

	static CharacterFactory *getInstance();

	void init();
	void update(int deltaTime);
	void render();

	void setProjection(glm::mat4 *project);
	void setSpawnFiles(string file);
	void spawnCharacter(int type, const glm::vec2 &pos);
	void setTileMapPos(const glm::vec2 &pos);
	void setMap(TileMap *map);

	void spawnRoutine();

	void destroyCharacter(const int &id);
	void killCharacter(const int &id);
	void damageEnemy(const int &id);
	void damagePlayer();
	void killPlayer();



private:

	
	glm::mat4 *projection;
	glm::vec2 tileMapPos;
	TileMap *mapa;

	map<int, Character *> characters;
	int last_id = 10;

	set<int> pendingToBeDestroyed;
	set<int> pendingToBeKilled;

	vector<pair<CharacterAvailable, glm::vec2>> enemies;

};

#endif // _CHARACTER_FACTORY_INCLUDE