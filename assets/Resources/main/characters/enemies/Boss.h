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
	void damage(int dmg, int id) override;

private:

	void init(const glm::ivec2 &tileMapPos);
	void updateBoxBalls();
	void spawnWorm();
	void spawnGreenBalls();
	void warmReturn(int id);
	void updateColliders();


	int delay = 100;
	//Worms
	int delaySpawnWorm = 600;
	int delaySpawnWormAnimation = 0;
	int isGoingtoSpawnWorm = 0;
	bool spawnedUp = false;
	bool spawnedDown = false;
	//GreenBalls
	int delaySpawnGreenBall = 600;
	int delaySpawnGreenBallAnimation = 0;
	int isGoingtoSpawnGreenBall = 0;
	bool spawnedLeft = false;
	bool spawnedRight = false;
	int lifeLeft = 0;
	int lifeRight = 0;
	
	//Sprites
	Sprite *spriteHead;
	Sprite *spriteTail;
	Sprite *spriteRightPart;
	Sprite *spriteLeftPart;
	//Colliders
	vector<Collision*>collidersBody;
	vector<Collision*>collidersGreenBalls;
	glm::ivec4 boxcoordenates[19] = {
		//central
		glm::ivec4(15, 3, 95, 80),
		glm::ivec4(37,0,72,3),
		glm::ivec4(45,80,65,91),
		//quad1
		glm::ivec4(1, 21, 15, 37),
		glm::ivec4(3, 15, 15, 21),
		glm::ivec4(10, 6, 15, 15),
		glm::ivec4(6, 10, 10, 15),
		glm::ivec4(5, 50, 15, 57),
		//GreenBall glm::ivec4(-1, 40, 15, 55),
		//quad2
		glm::ivec4(95, 21, 109, 37),
		glm::ivec4(95, 15, 107, 21),
		glm::ivec4(95, 6, 100, 15),
		glm::ivec4(100, 10, 103, 15),
		glm::ivec4(95, 50, 105, 57),
		//GreenBall glm::ivec4(95, 40, 112, 54),
		//quad3
		glm::ivec4(11, 73, 15, 85),
		glm::ivec4(9, 83, 12, 91),
		glm::ivec4(21, 80, 29, 84),
		//quad4
		glm::ivec4(95, 73, 99, 85),
		glm::ivec4(97, 83, 101, 91),
		glm::ivec4(81, 80, 89, 84),
		//special
	};

};

#endif // _Boss_INCLUDE
