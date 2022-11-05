#ifndef _PROJECTILE_FACTORY_INCLUDE
#define _PROJECTILE_FACTORY_INCLUDE

#include <map>
#include <set>

#include "Projectile.h"
#include "characters\ExplosionFactory.h"
#include "ProjectileNormal.h"
#include "ProjectileFireball.h"
#include "ProjectileWaves.h"
#include "TextureManager.h"

class ProjectileFactory {

private:

	static ProjectileFactory *instance_;

    ProjectileFactory();
    ~ProjectileFactory();

    void lateDestroyProjectile();

public:

	static ProjectileFactory *getInstance();

    void init();
    void update(int deltaTime);
    void render();

    void setProjection(glm::mat4 *project);

    void spawnProjectile(const glm::vec2 &pos, const glm::vec2 &vel, bool bounce, Projectile::ProjectileType type);
    void destroyProjectile(const int &id);
	void destroyAllProjectiles();

public:

	float mapSpeed;

private:

    glm::mat4 *projection;

    map<int, Projectile *> projectiles;
    int last_id = 0;

    set<int> pendingToBeDestroyed;
};

#endif // _PROJECTILE_FACTORY_INCLUDE
