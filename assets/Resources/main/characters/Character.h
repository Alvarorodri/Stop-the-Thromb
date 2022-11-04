#ifndef _CHARACTER_INCLUDE
#define _CHARACTER_INCLUDE

#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"
#include "TextureManager.h"
#include "projectiles\ProjectileFactory.h"


#include "GeneralDefines.h"



class Character {

public:

	enum CharacterAnims {STAND_RIGHT};

	Character(glm::mat4 *project, int id, Collision::CollisionGroups type);
	
	virtual void update(int deltaTime);
	virtual void render();
	virtual void damage(int dmg);

	virtual void setTileMap(TileMap *tileMap);
	virtual void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition() { return pos; };
	glm::vec4 getBoundingBox();
	void deleteRoutine();

	void clippingAvoidance();

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
	Texture *spritesheet;
	Sprite *sprite;
	TileMap *tmap;
	int shootDelay;
	int live = 1;

	glm::mat4 *projection;

	Collision *collider;
	CollisionSystem *collisionSystem;

};

#endif // _CHARACTER_INCLUDE
