#ifndef _EXPLOSION_INCLUDE
#define _EXPLOSION_INCLUDE


#include "TextureManager.h"
#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "GeneralDefines.h"
#include "sound\AudioManager.h"


class Explosion {

public:
	//Trying this shit
	enum Explosions { ExplosionPlayer, ExplosionEnemy, ExplosionProyectileR9mk0, ExplosionProyectileR9mk1, ExplosionProyectileR9mk2, ExplosionProyectileR9mk3, ExplosionProyectileWaves, ExplosionProyectileEnemy, ExplosionProyectileMisil, Portal, PortalPlayer};
	Explosion(Explosions type, glm::mat4 &project, const glm::vec2 &pos, const glm::vec4 &box);
	~Explosion();

	void update(int deltaTime);
	void render();
	bool isEnded();
	void setMap(TileMap* map);

private:
	bool ended;
	glm::mat4 *projection;
	glm::vec2 pos;
	Texture *spritesheet;
	Sprite *sprite;
	TileMap* map;
};
#endif // _EXPLOSIONFACTORY_INCLUDE
