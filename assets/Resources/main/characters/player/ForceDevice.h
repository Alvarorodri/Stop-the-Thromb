#ifndef _FORCE_DEVICE_INCLUDE
#define _FORCE_DEVICE_INCLUDE

#include <vector>
#include <set>

#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"

#include "GeneralDefines.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class ForceDevice {

public:

    enum ForceDeviceMk { ForceMK1, ForceMK2, ForceMK3 };

public:

    ForceDevice(glm::mat4 *project);
    void init(Collision *sCollider);
    void update(int deltaTime);
    void render();

    void setPosition(const glm::vec2 &pos);

    Collision* getCollider();
	bool isAttached() { return isAtached; };

    void setForceLevel(int level);

private:

    glm::vec2 getOffsetofColliders(bool left);

    void attachToASide();

private:

    int forceLevel;

    glm::vec2 posForce;
    Texture spritesheet;
    Sprite *sprite;
    TileMap *map;

    glm::mat4 *projection;

    Collision *collider;
    Collision *shipCollider;
    CollisionSystem *collisionSystem;

    bool latchKeys[256];

    bool isAtached = true;
    bool isLeft = false;
    float shipOffset = 32.0f; // TODO: hacerlo de alguna manera dinamica
    float leftLimit = 30;
    float rightLimit = 400;
    glm::vec2 targetPosition;

    float horizontalVelocity = 3.0;
    float verticalVelocity = 1.0;

};

#endif // _FORCE_DEVICE_INCLUDE
