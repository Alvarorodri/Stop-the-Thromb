#ifndef _COLLISION_INCLUDE
#define _COLLISION_INCLUDE

#include <math.h>
#include <vector>

#include "debug\TrianglesRenderer.h"

#include "GeneralDefines.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.
class Collision {

public:

    enum CollisionGroups {
        Player,
        Force,
        Map,
        Enemy,
        PlayerProjectiles,
        EnemyProjectiles,
        Uknown
    };

    Collision();
    Collision(int id, glm::mat4 *project, CollisionGroups group);
    ~Collision();

    void setProjection(glm::mat4 *project);

    void addCollider(const glm::vec4 &boxCollider);
    void removeCollider(const glm::vec4 &boxCollider);

    void changePositionAbsolute(const glm::vec2 &pos);
    void changePositionRelative(const glm::vec2 &pos);
	void setRotation(const glm::vec3 &rotation);
	void setBox(const glm::vec2 &size);
	int getId();

    glm::vec4 getBoundingBox();

#ifdef SHOW_HIT_BOXES
    void showHitBox();
    void render();
#endif // SHOW_HIT_BOXES

public:

    Collision::CollisionGroups collisionGroup;

    int collidersSize;
    vector<glm::vec4> collisions;

    glm::vec2 position;

private:

	int id;

#ifdef SHOW_HIT_BOXES
    TrianglesRenderer* trianglesRenderer;
#endif // SHOW_HIT_BOXES

};

#endif // _COLLISION_INCLUDE