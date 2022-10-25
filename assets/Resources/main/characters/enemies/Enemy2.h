#ifndef _Enemy2_INCLUDE
#define _Enemy2_INCLUDE

#include "characters\Character.h"

class Enemy2 : public Character {

public:

    Enemy2(glm::mat4 *project, int id, Collision::CollisionGroups type, const glm::ivec2 &tileMapPos);
    
    void update(int deltaTime);

private:
	enum Enemy2Anims { MOVE_LEFT };

	void init(const glm::ivec2 &tileMapPos);

};

#endif // _Enemy2_INCLUDE
