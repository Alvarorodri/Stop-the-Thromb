#ifndef _POWER_UP_INCLUDE
#define _POWER_UP_INCLUDE

#include "Object.h"
#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"

#include "GeneralDefines.h"


class PowerUp : public Object {

public:

    PowerUp(int id, glm::mat4 *project, const ObjectType type);
    void init();
	void update(int deltaTime) override;

	bool collisionRoutine() override;

};

#endif // _POWER_UP_INCLUDE
