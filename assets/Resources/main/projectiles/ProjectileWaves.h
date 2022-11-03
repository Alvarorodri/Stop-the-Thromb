#ifndef _PROJECTILE_LASERS_INCLUDE
#define _PROJECTILE_LASERS_INCLUDE

#include "projectiles\Projectile.h"
#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"
#include "projectiles\ProjectileFactory.h"

#include "GeneralDefines.h"

class ProjectileWaves : public Projectile {

public:

    ProjectileWaves(glm::mat4 *project, int id);
    void init(Texture *spritesheet, int type);
    void update(int deltaTime);
    void render();

	void setPosition(const glm::vec2 &pos) override;

    void projectileConfigurator(ProjectileType type, const glm::vec2 &xy);

    void collisionRoutine();

private:

    Sprite *auxSprite1, *auxSprite2;
    enum wavesState {  SpawningInitial, SpawningSections
    };
};

#endif // _PROJECTILE_LASERS_INCLUDE
