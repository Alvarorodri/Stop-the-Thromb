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
	enum Textures { Map, Stars, Planets, Player, Force, Projectiles, Waves, Enemies, Boss, PoweUp, ExplosionPlayer, Explosion};
	static TextureManager *getInstance();
	Texture getSpriteSheet(Textures type);

private:
	map<Textures, Texture> textures;
};

#endif // _TEXTURE_MANAGER_INCLUDE
