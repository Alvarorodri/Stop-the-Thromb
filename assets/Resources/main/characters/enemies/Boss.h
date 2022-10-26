#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "characters\Character.h"

class Boss : public Character {

public:

	enum BossAnims {STAND_RIGHT, STAND_UP, STAND_DOWN, MOVE_UP, MOVE_DOWN, DOWN_RETURN, UP_RETURN };

public:

    Boss(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos);
    
    void update(int deltaTime);

private:

	void init(const glm::ivec2 &tileMapPos);

};

#endif // _Boss_INCLUDE
