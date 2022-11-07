#ifndef _Enemy4_INCLUDE
#define _Enemy4_INCLUDE

#include "characters\Character.h"

class Enemy4 : public Character {

public:

	enum Enemy4Anims {STAND_RIGHT};

public:

	Enemy4(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos);

    void update(int deltaTime);

	void deleteRoutine();

private:
	void init(const glm::ivec2 &tileMapPos);
	void shoot();

	float jumpAngle2;
	float startY2;


};

#endif // _Enemy4_INCLUDE
