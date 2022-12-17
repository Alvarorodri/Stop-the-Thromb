#include "TextureManager.h"

TextureManager* TextureManager::instance_ = nullptr;

TextureManager *TextureManager::getInstance() {
	if (instance_ == nullptr) instance_ = new TextureManager();
	return instance_;
}

TextureManager::TextureManager() {
	instance_ = this;
}

TextureManager::~TextureManager() {
}

Texture* TextureManager::getSpriteSheet(Textures type) {
	map<Textures, Texture>::iterator it = textures.find(type);
	if (it == textures.end()) {
		Texture spritesheet;
		string image = "";
		switch (type)
		{
		case TextureManager::Map:
			image = "images/background/testing-long-map.png";
			break;
		case TextureManager::Background:
			image = "images/background/background.png";
			break;
		case TextureManager::Planets:
			image = "images/background/planets.png";
			break;
		case TextureManager::PlayerStt:
			image = "images/player/player-stt.png";
			break;
		case TextureManager::Force:
			image = "images/player/force-pit-beam.png";
			break;
		case TextureManager::Projectiles:
			image = "images/projectiles/projectiles.png";
			break;
		case TextureManager::Waves:
			image = "images/projectiles/waves.png";
			break;
		case TextureManager::BloodEnemies:
			image = "images/enemies/enemigos.png";
			break;
		case TextureManager::PoweUp:
			image = "images/misc/boosters.png";
			break;
		case TextureManager::ExplosionPlayer:
			image = "images/misc/explosion1.png";
			break;
		case TextureManager::ExplosionEnemy:
			image = "images/misc/explosion2.png";
			break;
		case TextureManager::ExplosionProyectilePlayer:
			image = "images/player/player.png";
			break;
		case TextureManager::ExplosionProyectileEnemy:
			image = "images/player/force-pit-beam.png";
			break;
		
		}

		spritesheet.loadFromFile(image, TEXTURE_PIXEL_FORMAT_RGBA);
		spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
		spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
		spritesheet.setMinFilter(GL_NEAREST);
		spritesheet.setMagFilter(GL_NEAREST);

		textures.insert({ type, spritesheet });
		it = textures.find(type);
		
	}
	return &(it->second);
	
}
