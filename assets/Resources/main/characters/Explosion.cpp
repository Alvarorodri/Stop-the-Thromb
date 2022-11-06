#include "Explosion.h"

Explosion::Explosion(Explosions type, glm::mat4 &project, const glm::vec2 &pos, const glm::vec4 &box) {
	ended = false;
	this->pos = pos;
	if (Explosions::ExplosionEnemy == type || Explosions::ExplosionPlayer == type) {
		if (Explosions::ExplosionEnemy == type) {
			spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::ExplosionEnemy);
		}
		else {
			spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::ExplosionPlayer);
		}
		sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1 / 8.f, 1.f), spritesheet, &project);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2((1 / 8.f)*0.f, 0.f));
		sprite->addKeyframe(0, glm::vec2((1 / 8.f)*1.f, 0.f));
		sprite->addKeyframe(0, glm::vec2((1 / 8.f)*2.f, 0.f));
		sprite->addKeyframe(0, glm::vec2((1 / 8.f)*3.f, 0.f));
		sprite->addKeyframe(0, glm::vec2((1 / 8.f)*4.f, 0.f));
		sprite->addKeyframe(0, glm::vec2((1 / 8.f)*5.f, 0.f));
	}
	else if (Explosions::ExplosionProyectileEnemy == type) {
		spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::ExplosionProyectileEnemy);
		sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1.0f / 10.0f, 1.0f / 16.0f), spritesheet, &project);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(1.0f / 10.0f*1.f, 1.0f / 16.0f*10.f));
		sprite->addKeyframe(0, glm::vec2(1.0f / 10.0f*2.f, 1.0f / 16.0f*10.f));
		sprite->addKeyframe(0, glm::vec2(1.0f / 10.0f*3.f, 1.0f / 16.0f*10.f));
	}
	else if (Explosions::ExplosionProyectilePlayer == type) {
		
		spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::ExplosionProyectilePlayer);
		sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1 / 16.0, 1 / 16.0), spritesheet, &project);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(1 / 16.0f*5.f, 1 / 16.0f*5.f));
		sprite->addKeyframe(0, glm::vec2(1 / 16.0f*6.f, 1 / 16.0f*5.f));
		sprite->addKeyframe(0, glm::vec2(1 / 16.0f*5.f, 1 / 16.0f*5.f));
		sprite->addKeyframe(0, glm::vec2(1 / 16.0f*6.f, 1 / 16.0f*5.f));
		sprite->addKeyframe(0, glm::vec2(1 / 16.0f*5.f, 1 / 16.0f*5.f));
		this->pos.y -= 8;
		this->pos.x -= 8;
	}
	sprite->changeAnimation(0, false);

	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));


}
Explosion::~Explosion() {}

bool Explosion::isEnded() {
	return ended;
}

void Explosion::update(int deltaTime) {
	pos.x -= 2.f;
	if (!ended) {
		sprite->update(deltaTime);
		ended = sprite->isFinidhedAnimation();
	}
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

void Explosion::render() {
	if(!ended)sprite->render();
}