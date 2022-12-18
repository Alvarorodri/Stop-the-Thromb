#ifndef _COLLISION_SYSTEM_INCLUDE
#define _COLLISION_SYSTEM_INCLUDE

#include "SpatialHashmap.h"

class CollisionSystem {

public:

    static CollisionSystem *instance_;

    static CollisionSystem *getInstance();
	static void deleteReference();

public:

    struct CollisionInfo {
        bool colliding;
        Collision *collider;
        bool triggered;
    };

	void setTileMapPos(float pos) { tileMapPos = pos; spatialHashmap->setTileMapPos(pos); };

    void addColliderIntoGroup(Collision* a);
    void removeColliderFromGroup(Collision* a);
	void updateCollider(Collision* a, const glm::vec2 &newPos);

    CollisionInfo isColliding(Collision* a, const glm::vec2 &offset);
    CollisionInfo isTriggering(Collision* a, const glm::vec2 &offset);

private:

    const bool collisionMatrix[10][10] = {
        /*                          Player Force  Map    Enemy  PlProj EnProj Uknown Object  EneSt  Enemy2                            */
        /* Player               */ {  0   , false, true , true , false, true , false, false, true , true  }, /* Player             */
        /* Force                */ { false,  0   , true , true , false, true , false, false, true , true  }, /* Force              */
        /* Map                  */ { true , true ,  0   , true , true , true , true , false, true , true  }, /* Map                */
        /* Enemy                */ { true , false, true ,  0   , false, false, false, false, true , false }, /* Enemy              */
        /* PlayerProjectiles    */ { false, false, true , true ,  0   , false, false, false, true , true  }, /* PlayerProjectiles  */
        /* EnemyProjectiles     */ { true , true , true , false, false,  0   , false, false, false, false }, /* EnemyProjectiles  */
        /* Uknown               */ { false, false, false, false, false, false,  0   , false, false, false }, /* Uknown             */
        /* Object               */ { true , true , false, false, false, false, false,  0   , false, false }, /* Object             */
        /* EnemyStatic          */ { true , false, true , true , false, false, false, false,  0   , false }, /* EnemyStatic                */
        /* Enemy2               */ { true , false, true , false, false, false, false, false, false,  0    }, /* Enemy2                */
        /*                          Player Force  Map    Enemy  PlProj EnProj Uknown Object  EneSt  Enemy2                            */
    };

    const bool triggersMatrix[10][10] = {
        /*                         Player Force  Map    Enemy  PlProj EnProj Uknown PowUp ---    ---                              */
        /* Player               */{  0   , true , false, false, false, false, false, false, false, false }, /* Player             */
        /* Force                */{ true ,  0   , false, false, false, false, false, false, false, false }, /* Force              */
        /* Map                  */{ false, false,  0   , false, false, false, false, false, false, false }, /* Map                */
        /* Enemy                */{ false, false, false,  0   , false, false, false, false, false, false }, /* Enemy              */
        /* PlayerProjectiles    */{ false, false, false, false,  0   , false, false, false, false, false }, /* PlayerProjectiles  */
        /* EnemyProjectiles     */{ false, false, false, false, false,  0   , false, false, false, false }, /* EnemyProjectiles   */
        /* Uknown               */{ false, false, false, false, false, false,  0   , false, false, false }, /* Uknown             */
        /* PowUp                */{ true , true , false, false, false, false, false,  0   , false, false }, /* PowUp              */
        /* ---                  */{ false, false, false, false, false, false, false, false,  0   , false }, /* ---                */
        /* ---                  */{ false, false, false, false, false, false, false, false, false,  0    }, /* ---                */
        /*                         Player Force  Map    Enemy  PlProj EnProj Uknown PowUp    ---    ---                           */
    };

    CollisionSystem();
    ~CollisionSystem();

    bool isValidCollision(const Collision* a, const Collision* b);
    bool isTriggerCollision(const Collision* a, const Collision* b);
    static bool searchForCollision(const Collision* a, const Collision* b, const glm::vec2 &offset);

    static bool overlapHorizontal(const glm::vec4 &a, const glm::vec4 &b, const glm::vec2 &posA, const glm::vec2 &posB);

private:

    //vector<vector<Collision*>> groups = vector<vector<Collision*>>(10, vector<Collision*>(0));
	SpatialHashmap *spatialHashmap;
	float tileMapPos = 0;

};

#endif // _COLLISION_SYSTEM_INCLUDE
