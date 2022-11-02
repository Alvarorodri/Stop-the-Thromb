#ifndef _WORM_INCLUDE
#define _WORM_INCLUDE

#include "characters\Character.h"
#include <vector>

class Part : public Character {
public:

	Part(glm::mat4 *project, int id, int idBody, const glm::ivec2 &tileMapPos);

	void update(int deltaTime);
	

private:

	void init(const glm::ivec2 &tileMapPos);
	int idBody;
};

class Worm : public Character {

public:

	Worm(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos);

	void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2 &pos);

private:

	void init(const glm::ivec2 &tileMapPos);
	std::vector<Part*> parts;
};

#endif // _WORM_INCLUDE

