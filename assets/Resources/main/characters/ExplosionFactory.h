#ifndef _EXPLOSIONFACTORY_INCLUDE
#define _EXPLOSIONFACTORY_INCLUDE

#include "TextureManager.h"
#include "textures\Sprite.h"
#include "textures\TileMap.h"
#include "GeneralDefines.h"
#include "TextureManager.h"
#include "Explosion.h"

#include <map>
#include <set>



class ExplosionFactory {
private:

	static ExplosionFactory *instance_;
	ExplosionFactory();
	~ExplosionFactory();
	TileMap* map;

public:
	

	static ExplosionFactory *getInstance();
	static void deleteReference();
	void spawnExplosion(Explosion::Explosions type, glm::mat4 *project, const glm::vec2 &pos, const glm::vec4 &box);
	void update(int deltaTime);
	void render();
	void setMap(TileMap* map);
	void deleteAll();
	std::map<int, Explosion*> explosions;
	int lastId = 0;
	set<int> explosionsToDelete;

};


#endif // _EXPLOSIONFACTORY_INCLUDE
