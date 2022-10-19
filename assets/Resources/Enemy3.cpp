#include "Enemy3.h"
#include "Game.h"
#include "GeneralDefines.h"

Enemy3::Enemy3(glm::mat4 *project) {
	projection = project;
	collider = new Collision(project, Collision::Enemy);

	collisionSystem = CollisionSystem::getInstance();
	collisionSystem->addColliderIntoGroup(collider);
}

void Enemy3::init(const glm::ivec2 &tileMapPos) {
	landed = false;
	spritesheet.loadFromFile("images/Enemies/basic-enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1 / 16.0, 1 / 10.0), &spritesheet, projection);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(POS_0, 8);
	sprite->addKeyframe(POS_0, glm::vec2((1.0 / 16.0)*0.0, (1.0 / 10.0)*4.0));
	sprite->setAnimationSpeed(POS_1, 8);
	sprite->addKeyframe(POS_1, glm::vec2((1.0 / 16.0)*1.0, (1.0 / 10.0)*4.0));
	sprite->setAnimationSpeed(POS_2, 8);
	sprite->addKeyframe(POS_2, glm::vec2((1.0 / 16.0)*2.0, (1.0 / 10.0)*4.0));
	sprite->setAnimationSpeed(POS_3, 8);
	sprite->addKeyframe(POS_3, glm::vec2((1.0 / 16.0)*3.0, (1.0 / 10.0)*4.0));
	sprite->setAnimationSpeed(POS_4, 8);
	sprite->addKeyframe(POS_4, glm::vec2((1.0 / 16.0)*4.0, (1.0 / 10.0)*4.0));


	sprite->changeAnimation(POS_0, false);
	tileMapDispl = tileMapPos;

	collider->addCollider(glm::ivec4(5, 8, 21, 23));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + posEnemy3.x, tileMapDispl.y + posEnemy3.y));

#ifdef SHOW_HIT_BOXES
	collider->showHitBox();
#endif // SHOW_HIT_BOXES

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy3.x), float(tileMapDispl.y + posEnemy3.y)));
}

void Enemy3::update(int deltaTime) {
	sprite->update(deltaTime);

	if(!landed){
		if (collisionSystem->isColliding(Enemy3::collider, glm::vec2(0, FALL_STEP))) {
			landed = true;
			jumpAngle = 0;
			startY = posEnemy3.y;
        } else {
            posEnemy3.y += FALL_STEP;
            collider->changePositionRelative(glm::vec2(0, FALL_STEP));
        }
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy3.x), float(tileMapDispl.y + posEnemy3.y)));
}

void Enemy3::render() {
	sprite->render();

#ifdef SHOW_HIT_BOXES
	collider->render();
#endif // SHOW_HIT_BOXES
}

void Enemy3::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Enemy3::setPosition(const glm::vec2 &pos) {
	posEnemy3 = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy3.x), float(tileMapDispl.y + posEnemy3.y)));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + posEnemy3.x, tileMapDispl.y + posEnemy3.y));
}
