#ifndef _Enemy1_INCLUDE
#define _Enemy1_INCLUDE

#include "Character.h"

class Enemy1 : public Character {
	
public:

	Enemy1(glm::mat4 *project, int id, Collision::CollisionGroups type, const glm::ivec2 &tileMapPos);
    void update(int deltaTime);

private:

	enum Enemy1Anims {STAND_LEFT, MOVE_LEFT, FLY_LEFT, LANDING_LEFT };

	void init(const glm::ivec2 &tileMapPos);

};

#endif // _Enemy1_INCLUDE
