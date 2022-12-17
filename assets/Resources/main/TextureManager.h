#ifndef _TEXTURE_MANAGER_INCLUDE
#define _TEXTURE_MANAGER_INCLUDE

#include <map>
#include "utilities\Texture.h"
class TextureManager {

private:

	static TextureManager *instance_;
	TextureManager();
	~TextureManager();

public:
	enum Textures { Map, Background, Planets, Player, PlayerStt, Force, Projectiles, Waves, BloodEnemies, PoweUp, ExplosionPlayer, ExplosionEnemy, ExplosionProyectilePlayer, ExplosionProyectileEnemy};
	static TextureManager *getInstance();
	Texture* getSpriteSheet(Textures type);

private:
	map<Textures, Texture> textures;
};

#endif // _TEXTURE_MANAGER_INCLUDE
