#ifndef _WORM_INCLUDE
#define _WORM_INCLUDE

#include "characters\Character.h"
#include "characters\CharacterFactory.h"
#include <vector>
#include <time.h> 

class Part : public Character {
public:

	Part(glm::mat4 *project, int id, int idBody);

	void update(int deltaTime);
	
	void rotateSprite(glm::vec3 rotation);
	glm::vec2 getPosition() { return pos; };
	void setPosition(const glm::vec2 &pos);
	bool isdamaged();
	void damage(int dmg, int id);

	void shoot();

public:

	glm::vec2 anchorPoint;

private:

	void init();
	int idBody;
	bool damaged;
	
};

class Worm : public Character {

public:

	Worm(glm::mat4 *project, int id, bool upOrDown, int bossID);

	void update(int deltaTime);
	void render();

	void rotateSprite(Part *part, const glm::vec2 &vector);
	void setPosition(const glm::vec2 &movement);
	void damage(int dmg, int id);

private:

	glm::vec2 targetPosition;
	int currentTarget = 0;

	enum routesEnum { RouteUp, CircleUL, CircleUR, RouteDown, CircleDR, CircleDL, SpawnPoint };

#pragma region Routes
	/*Routes: RouteUp, RouteDown, CircleUL, CircleUR, CircleDR, CircleDL*/
	glm::vec2 Routes[7][48] = { { glm::vec2(220.0f,23),glm::vec2(306.0,25),glm::vec2(318.0,28),glm::vec2(327.0,32),glm::vec2(335.0,39),glm::vec2(342.0,48),glm::vec2(343.0,68),glm::vec2(344.0,79),glm::vec2(344.0,58),glm::vec2(349.0,90),glm::vec2(356.0,99),glm::vec2(366.0,106),glm::vec2(378.0,110),glm::vec2(389.0,110),glm::vec2(397.0,108),glm::vec2(408.0,103),glm::vec2(417.0,96),glm::vec2(423.0,85),glm::vec2(426.0,74),glm::vec2(426.0,62),glm::vec2(423.0,51),glm::vec2(416.0,41),glm::vec2(407.0,33),glm::vec2(397.0,28),glm::vec2(385.0,26),glm::vec2(60.0,23),glm::vec2(43.0,27),glm::vec2(33.0,33),glm::vec2(25.0,42),glm::vec2(20.0,53),glm::vec2(18.0,65),glm::vec2(19.0,76),glm::vec2(24.0,87),glm::vec2(31.0,96),glm::vec2(41.0,103),glm::vec2(53.0,107),glm::vec2(64.0,107),glm::vec2(72.0,105),glm::vec2(83.0,100),glm::vec2(92.0,93),glm::vec2(98.0,82),glm::vec2(101.0,71),glm::vec2(110.0,55),glm::vec2(120.0,44),glm::vec2(132.0,35),glm::vec2(146.0,29),glm::vec2(162.0,25),glm::vec2(220.0f,23) }
		,{ glm::vec2(18.0,65.0),glm::vec2(20.0,53.0),glm::vec2(25.0,42.0),glm::vec2(33.0,33.0),glm::vec2(43.0,27.0),glm::vec2(60.0,23.0),glm::vec2(72.0,25.0),glm::vec2(82.0,30.0),glm::vec2(91.0,38.0),glm::vec2(98.0,48.0),glm::vec2(101.0,59.0),glm::vec2(101.0,71.0),glm::vec2(98.0,82.0),glm::vec2(92.0,93.0),glm::vec2(83.0,100.0),glm::vec2(72.0,105.0),glm::vec2(64.0,107.0),glm::vec2(53.0,107.0),glm::vec2(41.0,103.0),glm::vec2(31.0,96.0),glm::vec2(24.0,87.0),glm::vec2(19.0,76.0) }
		,{ glm::vec2(343.0,68.0),glm::vec2(345.0,56.0),glm::vec2(350.0,45.0),glm::vec2(358.0,36.0),glm::vec2(368.0,30.0),glm::vec2(385.0,26.0),glm::vec2(397.0,28.0),glm::vec2(407.0,33.0),glm::vec2(416.0,41.0),glm::vec2(423.0,51.0),glm::vec2(426.0,62.0),glm::vec2(426.0,74.0),glm::vec2(423.0,85.0),glm::vec2(417.0,96.0),glm::vec2(408.0,103.0),glm::vec2(397.0,108.0),glm::vec2(389.0,110.0),glm::vec2(378.0,110.0),glm::vec2(366.0,106.0),glm::vec2(356.0,99.0),glm::vec2(349.0,90.0),glm::vec2(344.0,79.0) }
		,{ glm::vec2(220.0f,229.0),glm::vec2(306.0,227.0),glm::vec2(318.0,224.0),glm::vec2(327.0,220.0),glm::vec2(335.0,213.0),glm::vec2(342.0,204.0),glm::vec2(343.0,184.0),glm::vec2(344.0,194.0),glm::vec2(344.0,173.0),glm::vec2(349.0,162.0),glm::vec2(356.0,153.0),glm::vec2(366.0,146.0),glm::vec2(378.0,142.0),glm::vec2(389.0,142.0),glm::vec2(397.0,144.0),glm::vec2(408.0,149.0),glm::vec2(417.0,156.0),glm::vec2(423.0,167.0),glm::vec2(426.0,178.0),glm::vec2(426.0,190.0),glm::vec2(423.0,201.0),glm::vec2(416.0,211.0),glm::vec2(407.0,219.0),glm::vec2(397.0,224.0),glm::vec2(385.0,226.0),glm::vec2(60.0,229.0),glm::vec2(43.0,225.0),glm::vec2(33.0,219.0),glm::vec2(25.0,210.0),glm::vec2(20.0,199.0),glm::vec2(18.0,187.0),glm::vec2(19.0,176.0),glm::vec2(24.0,165.0),glm::vec2(31.0,156.0),glm::vec2(41.0,149.0),glm::vec2(53.0,145.0),glm::vec2(64.0,145.0),glm::vec2(72.0,147.0),glm::vec2(83.0,152.0),glm::vec2(92.0,159.0),glm::vec2(98.0,170.0),glm::vec2(101.0,181.0),glm::vec2(110.0,197.0),glm::vec2(120.0,208.0),glm::vec2(132.0,217.0),glm::vec2(146.0,223.0),glm::vec2(162.0,227.0),glm::vec2(220.0f,229.0) }
		,{ glm::vec2(345.0,186.0),glm::vec2(347.0,174.0),glm::vec2(352.0,163.0),glm::vec2(360.0,154.0),glm::vec2(370.0,148.0),glm::vec2(387.0,144.0),glm::vec2(399.0,146.0),glm::vec2(409.0,151.0),glm::vec2(418.0,159.0),glm::vec2(425.0,169.0),glm::vec2(428.0,180.0),glm::vec2(428.0,192.0),glm::vec2(425.0,203.0),glm::vec2(419.0,214.0),glm::vec2(410.0,221.0),glm::vec2(399.0,226.0),glm::vec2(391.0,228.0),glm::vec2(380.0,228.0),glm::vec2(368.0,224.0),glm::vec2(358.0,217.0),glm::vec2(351.0,208.0),glm::vec2(346.0,197.0) }
		,{ glm::vec2(18.0,186.0),glm::vec2(25.0,163.0),glm::vec2(33.0,154.0),glm::vec2(43.0,148.0),glm::vec2(60.0,144.0),glm::vec2(72.0,146.0),glm::vec2(82.0,151.0),glm::vec2(91.0,159.0),glm::vec2(98.0,169.0),glm::vec2(101.0,180.0),glm::vec2(101.0,192.0),glm::vec2(98.0,203.0),glm::vec2(92.0,214.0),glm::vec2(83.0,221.0),glm::vec2(72.0,226.0),glm::vec2(64.0,228.0),glm::vec2(53.0,228.0),glm::vec2(41.0,224.0),glm::vec2(31.0,217.0),glm::vec2(24.0,208.0) }
		,{ glm::vec2(220.0f, 128.0f) } };

	int routesSize[7] = { 48, 22, 22, 48, 22, 20, 1 };
#pragma endregion

#pragma region IA Paramters
	/* Circuit, CircleL, CircleR*/
	vector<vector<int>> IAUp = { { CircleUL, CircleUR, RouteUp, SpawnPoint },{ CircleUL, CircleUR, RouteUp },{ CircleUL, CircleUR, RouteUp } };
	vector<vector<int>> IADown = { { CircleDL, CircleDR, RouteDown, SpawnPoint },{ CircleDL, CircleDR, RouteDown },{ CircleDL, CircleDR, RouteDown } };
	routesEnum currentRute = RouteUp;
	bool upOrDown = false;
	bool ableToChange = false;
	bool goingToSpawn = false;
#pragma endregion

	void changeTarget();
	routesEnum nextRoute();
	glm::vec2 getDir(const glm::vec2 &posA, const glm::vec2 &posB);

private:

	void init();
	std::vector<Part*> parts;
	int bossID;

	glm::vec2 spawnPoint = glm::vec2(220.0f, 128.0f);

	bool latchKeys[256] = { false };
};

#endif // _WORM_INCLUDE

