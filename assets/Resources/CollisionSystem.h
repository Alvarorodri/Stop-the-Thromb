#ifndef _COLLISIONSYSTEM_INCLUDE
#define _COLLISIONSYSTEM_INCLUDE

#include <glm/glm.hpp>
#include <vector>

#include "Collision.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class CollisionSystem{

public:

	static CollisionSystem *instance_;

	static CollisionSystem *getInstance();

public:

	void addColliderIntoGroup(Collision* a);
	void removeColliderFromGroup(Collision* a);

	bool isColliding(const Collision* a);
	
private:
	CollisionSystem();
	~CollisionSystem();

	static bool searchForCollision(const Collision* a, const Collision* b);

	static bool overlapVertical(const glm::ivec4 &a, const glm::ivec4 &b, const glm::ivec2 &posA, const glm::ivec2 &posB);
	static bool overlapHorizontal(const glm::ivec4 &a, const glm::ivec4 &b, const glm::ivec2 &posA, const glm::ivec2 &posB);

private:

	vector<vector<Collision*>> groups = vector<vector<Collision*>>(5, vector<Collision*>(0));

};


#endif // _COLLISION_INCLUDE


