#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "characters\Character.h"
#include "Worm.h"

class Boss : public Character {

public:

	enum BossAnims {STAND_RIGHT, STAND_UP, STAND_DOWN, MOVE_UP, MOVE_DOWN, DOWN_RETURN, UP_RETURN };

public:

    Boss(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos);
    
    void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2 &pos);

private:

	void init(const glm::ivec2 &tileMapPos);
	int delay = 100;
	Sprite *spriteHead;
	Sprite *spriteTail;
	Sprite *spriteRightPart;
	Sprite *spriteLeftPart;

};

#endif // _Boss_INCLUDE
