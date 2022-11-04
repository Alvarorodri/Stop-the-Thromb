#ifndef _PROJECTILE_NORMAL_INCLUDE
#define _PROJECTILE_NORMAL_INCLUDE

#include "projectiles\Projectile.h"
#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"
#include "projectiles\ProjectileFactory.h"

#include "GeneralDefines.h"

class ProjectileNormal : public Projectile {

public:

    ProjectileNormal(glm::mat4 *project, int id, int type);
    void init(Texture *spritesheet, int type);
    void update(int deltaTime);
    void render();

    void projectileConfigurator(ProjectileType type, const glm::vec2 &xy);

    bool collisionRoutine();

private:
	
	ProjectileType projectileType;

};

#endif // _PROJECTILE_NORMAL_INCLUDE
