#include "Enemy3.h"

Enemy3::Enemy3(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos) :Character(project, id, Collision::Enemy) {

	init(tileMapPos);
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
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
	collider->showHitBox();
#endif // SHOW_HIT_BOXES

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Enemy3::update(int deltaTime) {
	sprite->update(deltaTime);

    CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Enemy3::collider, glm::vec2(0, FALL_STEP));

	if(!landed){
		if (info.colliding) {
			landed = true;
			jumpAngle = 0;
			startY = pos.y;
        } else {
            pos.y += FALL_STEP;
            collider->changePositionRelative(glm::vec2(0, FALL_STEP));
        }
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}
