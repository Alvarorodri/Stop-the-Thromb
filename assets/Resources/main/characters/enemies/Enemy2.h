#ifndef _Enemy2_INCLUDE
#define _Enemy2_INCLUDE

#include "characters\Character.h"
#include <cstdlib>

class Enemy2 : public Character {

public:

    Enemy2(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos);
    
    void update(int deltaTime);

	void deleteRoutine();

private:
	enum Enemy2Anims { MOVE_LEFT };

	void init(const glm::ivec2 &tileMapPos);
	void shoot();

};

#endif // _Enemy2_INCLUDE
