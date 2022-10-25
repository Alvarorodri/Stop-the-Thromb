#ifndef _Enemy4_INCLUDE
#define _Enemy4_INCLUDE

#include "characters\Character.h"

class Enemy4 : public Character {

public:

	enum Enemy4Anims {STAND_RIGHT};

public:

	Enemy4(glm::mat4 *project, int id, Collision::CollisionGroups type, const glm::ivec2 &tileMapPos);

    void update(int deltaTime);

private:
	void init(const glm::ivec2 &tileMapPos);

};

#endif // _Enemy4_INCLUDE
