#pragma once
#ifndef _BloodEnemy1_INCLUDE
#define _BloodEnemy1_INCLUDE

#include "characters\Character.h"

class BloodEnemy1 : public Character {

public:

	BloodEnemy1(glm::mat4* project, int id, const glm::ivec2& tileMapPos);
	void update(int deltaTime);

    void normalRoutine();
    void attachRoutine();

	void deleteRoutine();

private:

	enum BloodEnemy1Anims {NORMAL, COAGULATED};

	void init(const glm::ivec2& tileMapPos);

    glm::vec2 upPosition = glm::vec2(220.0f, 0.0f);
    glm::vec2 downPosition = glm::vec2(220.0f, 256.0f);

	int jumpDelay;
	bool staystatic;
	bool coagulated;
	bool up;
	bool right;

};

#endif // _BloodEnemy1_INCLUDE