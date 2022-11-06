#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "sound\AudioManager.h"
#include "characters\Character.h"
#include "ForceDevice.h"
#include "ui\Text.h"

class Player : public Character {

private:

	enum PlayerAnims {STAND_RIGHT, STAND_UP, STAND_DOWN, MOVE_UP, MOVE_DOWN, DOWN_RETURN, UP_RETURN };

public:

    Player(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos);
    void update(int deltaTime);
	void render();

	bool isPlayer() { return true; };

	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition() { return pos; };

	void inputController();

	void damage(int dmg) override;

	void spawnForce();
	void destroyForce();
	void increaseForce();

private:

	void init(const glm::ivec2 &tileMapPos);

	void initAnimation();
    

private:

	Sprite *chargeProjectile;
	Sprite *boost;
	bool isInitAnimation = false;
	bool soundcharge = false;
	bool soundBoost = false;

    ForceDevice *forceDevice;
	bool forceSpawned = false;

	Text text;
	bool godmode = false;

	int contGod = 0;
						   /*  0,  1,  2,  3  mkLevels*/
	float timestampMks[4] = { 15, 30, 45, 60 };
	float currentCharge = 0;

	bool latchKeys[256] = { false };

};

#endif // _PLAYER_INCLUDE
