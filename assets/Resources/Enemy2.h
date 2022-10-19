#ifndef _Enemy2_INCLUDE
#define _Enemy2_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Collision.h"
#include "CollisionSystem.h"

#include "GeneralDefines.h"

// Enemy2 is basically a Sprite that represents the Enemy2. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class Enemy2 {

public:

	enum Enemy2Anims {MOVE_LEFT};

public:

    Enemy2(glm::mat4 *project);
    void init(const glm::ivec2 &tileMapPos);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap *tileMap);
    void setPosition(const glm::vec2 &pos);

private:

    bool bJumping;
	glm::ivec2 tileMapDispl;
    glm::vec2 posEnemy2;
    int jumpAngle, startY;
    Texture spritesheet;
    Sprite *sprite;
    TileMap *map;

    glm::mat4 *projection;

    Collision *collider;
    CollisionSystem *collisionSystem;

};

#endif // _Enemy2_INCLUDE
