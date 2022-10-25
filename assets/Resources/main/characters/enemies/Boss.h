#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"

#include "GeneralDefines.h"

// Boss is basically a Sprite that represents the Boss. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class Boss {

public:

	enum BossAnims {STAND_RIGHT, STAND_UP, STAND_DOWN, MOVE_UP, MOVE_DOWN, DOWN_RETURN, UP_RETURN };

public:

    Boss(glm::mat4 *project);
    void init(const glm::ivec2 &tileMapPos);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap *tileMap);
    void setPosition(const glm::vec2 &pos);

private:

    bool bJumping;
    glm::ivec2 tileMapDispl;
    glm::vec2 posBoss;
    int jumpAngle, startY;
    Texture spritesheet;
    Sprite *sprite;
    TileMap *map;

    glm::mat4 *projection;

    Collision *collider;
    CollisionSystem *collisionSystem;

};

#endif // _Boss_INCLUDE
