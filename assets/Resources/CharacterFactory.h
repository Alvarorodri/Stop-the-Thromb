#ifndef _CHARACTER_FACTORY_INCLUDE
#define _CHARACTER_FACTORY_INCLUDE

#include <map>

#include "Character.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "Enemy4.h"
#include "Collision.h"
class CharacterFactory {

private:

	static CharacterFactory *instance_;
	CharacterFactory();
	~CharacterFactory();

public:

	static CharacterFactory *getInstance();

	void init();
	void update(int deltaTime);
	void render();
	void setProjection(glm::mat4 *project);
	void spawnCharacter(int type, const glm::vec2 &pos);
	void setTileMapPos(const glm::vec2 &pos);
	void setMap(TileMap *map);

private:

	glm::mat4 *projection;
	glm::vec2 tileMapPos;
	TileMap *mapa;

	map<int, Character *> characters;
	int last_id;

};

#endif // _CHARACTER_FACTORY_INCLUDE