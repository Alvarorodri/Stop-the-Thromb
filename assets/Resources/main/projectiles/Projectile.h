#ifndef _PROJECTILE_INCLUDE
#define _PROJECTILE_INCLUDE

#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"
#include "characters\player\ForceDevice.h"

#include "GeneralDefines.h"

class Projectile {

public:

	enum ProjectileType {   BombProjectile, ForceProjectile, R9mk0, R9mk1, R9mk2, R9mk3, EnemyProjectile, 
                            Fireball,
                            Waves,
    };

public:

    Projectile();

    virtual void init(Texture *spritesheet, int type);
    virtual void update(int deltaTime);
    virtual void render();

    virtual void setPosition(const glm::vec2 &pos);
    virtual void setVelocity(const glm::vec2 &vel);
    virtual void setType(const ProjectileType type);
    virtual void setBounciness(const bool bounce);

    virtual int getId() { return idProjectile; };

    virtual bool collisionRoutine();
    virtual void deleteRoutine();

protected:

    int idProjectile;
    glm::vec2 posProjectile;
    Texture spritesheet;
    Sprite *sprite;

    glm::mat4 *projection;

    Collision *collider;
    CollisionSystem *collisionSystem;
    int collisionWait = 3;

    glm::vec2 projVelocity;
    Projectile::ProjectileType projType;
    bool projBounce;
};

#endif // _PROJECTILE_INCLUDE
