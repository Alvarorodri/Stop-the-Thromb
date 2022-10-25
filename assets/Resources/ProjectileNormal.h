#ifndef _PROJECTILE_NORMAL_INCLUDE
#define _PROJECTILE_NORMAL_INCLUDE

#include "Projectile.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Collision.h"
#include "CollisionSystem.h"
#include "ProjectileFactory.h"

#include "GeneralDefines.h"

class ProjectileNormal : public Projectile {

public:

    ProjectileNormal(glm::mat4 *project, int id);
    void init(Texture *spritesheet, int type);
    void update(int deltaTime);
    void render();

    void projectileConfigurator(ProjectileType type, const glm::vec2 &xy);

    void collisionRoutine();

};

#endif // _PROJECTILE_NORMAL_INCLUDE
