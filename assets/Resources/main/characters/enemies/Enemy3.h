#ifndef _Enemy3_INCLUDE
#define _Enemy3_INCLUDE

#include <map>
#include "characters\Character.h"


class Enemy3 : public Character {

public:

    Enemy3(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos);
    void update(int deltaTime);
	void shoot();

private:
	enum Enemy3Anims { POS_0, POS_1, POS_2, POS_3, POS_4 };
	void init(const glm::ivec2 &tileMapPos);
	
	
	std::map<int, glm::vec2> poscanon;


};

#endif // _Enemy3_INCLUDE
