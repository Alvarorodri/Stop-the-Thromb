#ifndef _CHARACTER_FACTORY_INCLUDE
#define _CHARACTER_FACTORY_INCLUDE

#include <map>

#include "Character.h"
#include "enemies\Enemy1.h"
#include "enemies\Enemy2.h"
#include "enemies\Enemy3.h"
#include "enemies\Enemy4.h"
#include "collisions\Collision.h"
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