#include "Explosion.h"

Explosion::Explosion(Explosions type, glm::mat4 &project, const glm::vec2 &pos, const glm::vec4 &box) {
	ended = false;
	spritesheet;
	if (Explosions::ExplosionNormal == type) {
		spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Explosion);
	}
	else {
		spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::ExplosionPlayer);
	}
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1 / 8.f, 1.f), &spritesheet, &project);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2((1 / 8.f)*0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2((1 / 8.f)*1.f, 0.f));
	sprite->addKeyframe(0, glm::vec2((1 / 8.f)*2.f, 0.f));
	sprite->addKeyframe(0, glm::vec2((1 / 8.f)*3.f, 0.f));
	sprite->addKeyframe(0, glm::vec2((1 / 8.f)*4.f, 0.f));
	sprite->addKeyframe(0, glm::vec2((1 / 8.f)*5.f, 0.f));

	sprite->changeAnimation(0, false);
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));


}
Explosion::~Explosion() {}

bool Explosion::isEnded() {
	return ended;
}

void Explosion::update(int deltaTime) {
	if (!ended) {
		sprite->update(deltaTime);
		ended = sprite->isFinidhedAnimation();
	}
	sprite->setPosition(glm::vec2(float(pos.x - 1.f), float(pos.y)));
}

void Explosion::render() {
	if(!ended)sprite->render();
}