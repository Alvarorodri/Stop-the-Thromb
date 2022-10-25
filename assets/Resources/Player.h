#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Collision.h"
#include "CollisionSystem.h"
#include "ForceDevice.h"

#include "GeneralDefines.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class Player {

public:

	enum PlayerAnims {STAND_RIGHT, STAND_UP, STAND_DOWN, MOVE_UP, MOVE_DOWN, DOWN_RETURN, UP_RETURN };

public:

    Player(glm::mat4 *project);
    void init(const glm::ivec2 &tileMapPos);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap *tileMap);
    void setPosition(const glm::vec2 &pos);
    glm::vec2 getPosition() { return posPlayer; };

    void inputController();

private:

    void rotate(const float &angleX, const float &angleY, const float &angleZ);

private:

    bool bJumping;
    bool rot;
    glm::ivec2 tileMapDispl;
    glm::vec2 posPlayer;
    int jumpAngle, startY;
    Texture spritesheet;
    Sprite *sprite;
    TileMap *map;

    glm::mat4 *projection;

    Collision *collider;
    CollisionSystem *collisionSystem;

    ForceDevice *forceDevice;

    bool latchKeys[256];

};

#endif // _PLAYER_INCLUDE
