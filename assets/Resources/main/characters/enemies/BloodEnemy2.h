#pragma once
#ifndef _BloodEnemy2_INCLUDE
#define _BloodEnemy2_INCLUDE

#include "characters\Character.h"

class BloodEnemy2 : public Character {

public:

	BloodEnemy2(glm::mat4* project, int id, const glm::ivec2& tileMapPos);
	void update(int deltaTime);

    void normalRoutine();

	void deleteRoutine();

private:

	enum BloodEnemy1Anims {NORMAL, COAGULATED};

	void init(const glm::ivec2& tileMapPos);

	bool coagulated;

};

#endif // _BloodEnemy2_INCLUDE