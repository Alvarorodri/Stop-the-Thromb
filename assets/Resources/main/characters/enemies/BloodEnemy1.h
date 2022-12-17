#pragma once
#ifndef _BloodEnemy1_INCLUDE
#define _BloodEnemy1_INCLUDE

#include "characters\Character.h"

class BloodEnemy1 : public Character {

public:

	BloodEnemy1(glm::mat4* project, int id, const glm::ivec2& tileMapPos);
	void update(int deltaTime);

	void deleteRoutine();

private:

	enum BloodEnemy1Anims {NORMAL, COAGULATED};

	void init(const glm::ivec2& tileMapPos);


	int jumpDelay;
	bool staystatick;
	bool coagulated;
	bool up;
	bool right;

};

#endif // _BloodEnemy1_INCLUDE