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

public:

    ProjectileFireball(glm::mat4 *project, int id);
    void init(Texture *spritesheet, int type);
    void update(int deltaTime);
    void render();

    void projectileConfigurator(ProjectileType type, const glm::vec2 &xy);

    void collisionRoutine();

    void followMapShape();

};

#endif // _PROJECTILE_FIREBALL_INCLUDE
