#ifndef _COLLISION_INCLUDE
#define _COLLISION_INCLUDE

#include "TrianglesRenderer.h"

#include "GeneralDefines.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.
class Collision {

public:

    enum CollisionGroups {
        Player,
        Map,
        Enemy,
        PlayerProjectiles,
        EnemyProjectiles
    };

    Collision();
    Collision(const glm::mat4 &project, CollisionGroups group);
    ~Collision();

    void setProjection(const glm::mat4 &project);

    void addCollider(const glm::ivec4 &boxCollider);
    void removeCollider(const glm::ivec4 &boxCollider);

    void changePositionAbsolute(const glm::ivec2 &pos);
    void changePositionRelative(const glm::ivec2 &pos);

#ifdef SHOW_HIT_BOXES
    void showHitBox();
    void render();
#endif // SHOW_HIT_BOXES

public:

    Collision::CollisionGroups collisionGroup;

    int collidersSize;
    glm::ivec4 *collisions;

    glm::ivec2 position;

private:

    TrianglesRenderer* trianglesRenderer;

};

#endif // _COLLISION_INCLUDE
