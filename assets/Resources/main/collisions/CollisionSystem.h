#ifndef _COLLISIONSYSTEM_INCLUDE
#define _COLLISIONSYSTEM_INCLUDE

#include <Set>

#include "Collision.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.
class CollisionSystem {

public:

    static CollisionSystem *instance_;

    static CollisionSystem *getInstance();

public:

    struct CollisionInfo {
        bool colliding;
        Collision *collider;
        bool triggered;
    };

    void addColliderIntoGroup(Collision* a);
    void removeColliderFromGroup(Collision* a);

    CollisionInfo isColliding(const Collision* a, const glm::vec2 &offset);
    CollisionInfo isTriggering(const Collision* a, const glm::vec2 &offset);

private:

    const vector<vector<bool>> collisionMatrix = {
        /*                          Player Force  Map    Enemy  PlProj EnProj Uknown ---    ---    ---                             */
        /* Player               */ {  0   , false, true , true , false, true , false, false, false, false }, /* Player             */
        /* Force                */ { false,  0   , true , true , false, true , false, false, false, false }, /* Force              */
        /* Map                  */ { true , true ,  0   , true , true , true , false, false, false, false }, /* Map                */
        /* Enemy                */ { true , true , true ,  0   , true , false, false, false, false, false }, /* Enemy              */
        /* PlayerProjectiles    */ { false, false, true , true ,  0   , true , false, false, false, false }, /* PlayerProjectiles  */
        /* EnemyProjectiles     */ { true , true , true , false, true ,  0   , false, false, false, false }, /* EnemyProjectiles   */
        /* Uknown               */ { false, false, false, false, false, false,  0   , false, false, false }, /* Uknown             */
        /* ---                  */ { false, false, false, false, false, false, false,  0   , false, false }, /* ---                */
        /* ---                  */ { false, false, false, false, false, false, false, false,  0   , false }, /* ---                */
        /* ---                  */ { false, false, false, false, false, false, false, false, false,  0    }, /* ---                */
        /*                          Player Force  Map    Enemy  PlProj EnProj Uknown ---    ---    ---                             */
    };

    const vector<vector<bool>> triggersMatrix = {
        /*                          Player Force  Map    Enemy  PlProj EnProj Uknown ---    ---    ---                             */
        /* Player               */{  0   , true , true , true , false, true , false, false, false, false }, /* Player             */
        /* Force                */{ true ,  0   , true , true , false, true , false, false, false, false }, /* Force              */
        /* Map                  */{ true , true ,  0   , true , true , true , false, false, false, false }, /* Map                */
        /* Enemy                */{ true , true , true ,  0   , true , false, false, false, false, false }, /* Enemy              */
        /* PlayerProjectiles    */{ false, false, true , true ,  0   , true , false, false, false, false }, /* PlayerProjectiles  */
        /* EnemyProjectiles     */{ true , true , true , false, true ,  0   , false, false, false, false }, /* EnemyProjectiles   */
        /* Uknown               */{ false, false, false, false, false, false,  0   , false, false, false }, /* Uknown             */
        /* ---                  */{ false, false, false, false, false, false, false,  0   , false, false }, /* ---                */
        /* ---                  */{ false, false, false, false, false, false, false, false,  0   , false }, /* ---                */
        /* ---                  */{ false, false, false, false, false, false, false, false, false,  0 }, /* ---                */
                                                                                                         /*                          Player Force  Map    Enemy  PlProj EnProj Uknown ---    ---    ---                             */
    };

    CollisionSystem();
    ~CollisionSystem();

    bool isValidCollision(const Collision* a, const Collision* b);
    bool isTriggerCollision(const Collision* a, const Collision* b);
    static bool searchForCollision(const Collision* a, const Collision* b, const glm::vec2 &offset);

    static bool overlapVertical(const glm::vec4 &a, const glm::vec4 &b, const glm::vec2 &posA, const glm::vec2 &posB);
    static bool overlapHorizontal(const glm::vec4 &a, const glm::vec4 &b, const glm::vec2 &posA, const glm::vec2 &posB);

private:

    vector<vector<Collision*>> groups = vector<vector<Collision*>>(10, vector<Collision*>(0));

};

#endif // _COLLISION_INCLUDE
