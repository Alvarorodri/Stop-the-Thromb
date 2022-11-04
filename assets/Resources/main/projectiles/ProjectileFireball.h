#ifndef _PROJECTILE_FIREBALL_INCLUDE
#define _PROJECTILE_FIREBALL_INCLUDE

#include "projectiles\Projectile.h"
#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"
#include "projectiles\ProjectileFactory.h"

#include "GeneralDefines.h"

class ProjectileFireball : public Projectile {

private:

    enum Movements { Down, Right, Up, Left};

public:

    ProjectileFireball(glm::mat4 *project, int id);
    void init(Texture *spritesheet, int type);
    void update(int deltaTime);
    void render();

    void projectileConfigurator(ProjectileType type, const glm::vec2 &xy);

    bool collisionRoutine();

    bool followMapShape();
	bool collisionHelper(const CollisionSystem::CollisionInfo &info);

private:

    int lastMovement;
	float mapSpeed;

};

#endif // _PROJECTILE_FIREBALL_INCLUDE
