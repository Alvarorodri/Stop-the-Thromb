#ifndef _Enemy3_INCLUDE
#define _Enemy3_INCLUDE

#include "Character.h"

class Enemy3 : public Character {

public:

    Enemy3(glm::mat4 *project, int id, Collision::CollisionGroups type, const glm::ivec2 &tileMapPos);
    void update(int deltaTime);


private:
	enum Enemy3Anims { POS_0, POS_1, POS_2, POS_3, POS_4 };
	void init(const glm::ivec2 &tileMapPos);

};

#endif // _Enemy3_INCLUDE
