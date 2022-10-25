#ifndef _POWER_UP_INCLUDE
#define _POWER_UP_INCLUDE

#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"

#include "GeneralDefines.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class PowerUp {

public:

    enum PowerUpType {  PowerUpBlue, PowerUpGreen, PowerUpRed,
                        PowerUpYellow, PowerUpBlack, 
                        ModifierS, ModifierMRed, ModifierMBlue};

public:

    PowerUp(glm::mat4 *project);
    void init(const PowerUpType type);
    void update(int deltaTime);
    void render();

    void setPosition(const glm::vec2 &pos);
    void setType(const PowerUpType type);

private:

    int size = 10;

    glm::vec2 posPowerUp;
    Texture spritesheet;
    Sprite *sprite;

    glm::mat4 *projection;

    Collision *collider;
    CollisionSystem *collisionSystem;

};

#endif // _POWER_UP_INCLUDE
