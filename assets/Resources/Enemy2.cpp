#include "Enemy2.h"
#include "Game.h"
#include "GeneralDefines.h"

Enemy2::Enemy2(glm::mat4 *project) {
	projection = project;
	collider = new Collision(project, Collision::Enemy);

	collisionSystem = CollisionSystem::getInstance();
	collisionSystem->addColliderIntoGroup(collider);
}

void Enemy2::init(const glm::ivec2 &tileMapPos) {
	bJumping = false;
	spritesheet.loadFromFile("images/Enemies/basic-enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1 / 16.0, 1 / 10.0), &spritesheet, projection);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2((1.0 / 16.0)*0.0, (1.0 / 10.0)*0.0));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2((1.0 / 16.0)*1.0, (1.0 / 10.0)*0.0));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2((1.0 / 16.0)*2.0, (1.0 / 10.0)*0.0));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2((1.0 / 16.0)*3.0, (1.0 / 10.0)*0.0));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2((1.0 / 16.0)*4.0, (1.0 / 10.0)*0.0));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2((1.0 / 16.0)*5.0, (1.0 / 10.0)*0.0));

	sprite->changeAnimation(MOVE_LEFT, false);
	tileMapDispl = tileMapPos;

	collider->addCollider(glm::ivec4(2, 7, 15, 17));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + posEnemy2.x, tileMapDispl.y + posEnemy2.y));

#ifdef SHOW_HIT_BOXES
	collider->showHitBox();
#endif // SHOW_HIT_BOXES

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy2.x), float(tileMapDispl.y + posEnemy2.y)));
}

void Enemy2::update(int deltaTime)
{
	sprite->update(deltaTime);
	
	if (!collisionSystem->isColliding(Enemy2::collider, glm::vec2(-0.5, 0))) {
        posEnemy2.x -= 0.5;
        collider->changePositionRelative(glm::vec2(-0.5, 0));
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy2.x), float(tileMapDispl.y + posEnemy2.y)));
}

void Enemy2::render() {
	sprite->render();

#ifdef SHOW_HIT_BOXES
	collider->render();
#endif // SHOW_HIT_BOXES
}

void Enemy2::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Enemy2::setPosition(const glm::vec2 &pos) {
	posEnemy2 = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy2.x), float(tileMapDispl.y + posEnemy2.y)));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + posEnemy2.x, tileMapDispl.y + posEnemy2.y));
}