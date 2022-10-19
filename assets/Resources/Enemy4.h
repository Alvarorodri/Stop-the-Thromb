#ifndef _Enemy4_INCLUDE
#define _Enemy4_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Collision.h"
#include "CollisionSystem.h"

#include "GeneralDefines.h"

// Enemy4 is basically a Sprite that represents the Enemy4. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class Enemy4 {

public:

	enum Enemy4Anims {STAND_RIGHT};

public:

    Enemy4(glm::mat4 *project);
    void init(const glm::ivec2 &tileMapPos);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap *tileMap);
    void setPosition(const glm::vec2 &pos);

private:

    bool bJumping;
	bool landed;
    glm::ivec2 tileMapDispl;
    glm::vec2 posEnemy4;
    float jumpAngle, startY;
    Texture spritesheet;
    Sprite *sprite;
    TileMap *map;

    glm::mat4 *projection;

    Collision *collider;
    CollisionSystem *collisionSystem;

};

#endif // _Enemy4_INCLUDE
