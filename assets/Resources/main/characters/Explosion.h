#ifndef _EXPLOSION_INCLUDE
#define _EXPLOSION_INCLUDE


#include "TextureManager.h"
#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "GeneralDefines.h"


class Explosion {

public:
	//Trying this shit
	enum Explosions { ExplosionPlayer, ExplosionEnemy, ExplosionProyectilePlayer, ExplosionProyectileEnemy };
	Explosion(Explosions type, glm::mat4 &project, const glm::vec2 &pos, const glm::vec4 &box);
	~Explosion();

	void update(int deltaTime);
	void render();
	bool isEnded();

private:
	bool ended;
	glm::mat4 *projection;
	glm::vec2 pos;
	Texture *spritesheet;
	Sprite *sprite;
};
#endif // _EXPLOSIONFACTORY_INCLUDE
