#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE

#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"

#include "GeneralDefines.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class Object {

public:

    enum ObjectType {   PowerUpBlue, PowerUpGreen, PowerUpRed,
                        PowerUpYellow, PowerUpBlack, 
                        ModifierS, ModifierMRed, ModifierMBlue};

public:

    Object(int id, glm::mat4 *project, const Object::ObjectType type);
    virtual void init();
    virtual void update(int deltaTime);
    virtual void render();

    void setPosition(const glm::vec2 &pos);
    void setType(const ObjectType type);
	void setSize(int size);

	virtual bool collisionRoutine();
	virtual void deleteRoutine();

protected:

	int id;

    int size = 10;
	ObjectType type;

    glm::vec2 pos;
	glm::vec2 vel;
    Texture *spritesheet;
    Sprite *sprite;

    glm::mat4 *projection;

    Collision *collider;
    CollisionSystem *collisionSystem;

};

#endif // _OBJECT_INCLUDE
