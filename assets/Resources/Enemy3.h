#ifndef _Enemy3_INCLUDE
#define _Enemy3_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Collision.h"
#include "CollisionSystem.h"

#include "GeneralDefines.h"

// Enemy3 is basically a Sprite that represents the Enemy3. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class Enemy3 {

public:

	enum Enemy3Anims {POS_0, POS_1, POS_2, POS_3, POS_4};

public:

    Enemy3(glm::mat4 *project);
    void init(const glm::ivec2 &tileMapPos);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap *tileMap);
    void setPosition(const glm::vec2 &pos);

private:
    bool landed;
    glm::ivec2 tileMapDispl;
    glm::vec2 posEnemy3;
    int jumpAngle, startY;
    Texture spritesheet;
    Sprite *sprite;
    TileMap *map;

    glm::mat4 *projection;

    Collision *collider;
    CollisionSystem *collisionSystem;

};

#endif // _Enemy3_INCLUDE
