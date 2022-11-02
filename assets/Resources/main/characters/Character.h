#ifndef _CHARACTER_INCLUDE
#define _CHARACTER_INCLUDE

#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"



#include "GeneralDefines.h"



class Character {

public:

	enum CharacterAnims {STAND_RIGHT};

	Character(glm::mat4 *project, int id, Collision::CollisionGroups type);
	
	virtual void update(int deltaTime);
	virtual void render();

	virtual void setTileMap(TileMap *tileMap);
	virtual void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition() { return pos; };
	void deleteRoutine();

protected:

	virtual void init(const glm::ivec2 &tileMapPos);
	virtual void rotate(const float &angleX, const float &angleY, const float &angleZ);

	bool bJumping;
	bool rot;
	int id;
	bool landed;
	int jumpDelay;
	glm::ivec2 tileMapDispl;
	glm::vec2 pos;
	float jumpAngle;
	float startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *tmap;
	int shootDelay;

	glm::mat4 *projection;

	Collision *collider;
	CollisionSystem *collisionSystem;

};

#endif // _CHARACTER_INCLUDE
