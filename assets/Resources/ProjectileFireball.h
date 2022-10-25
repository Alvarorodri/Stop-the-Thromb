#ifndef _PROJECTILE_FIREBALL_INCLUDE
#define _PROJECTILE_FIREBALL_INCLUDE

#include "Projectile.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Collision.h"
#include "CollisionSystem.h"
#include "ProjectileFactory.h"

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
