#include "Enemy4.h"

Enemy4::Enemy4(glm::mat4 *project, int id, Collision::CollisionGroups type, const glm::ivec2 &tileMapPos) :Character(project, id, type) {

	init(tileMapPos);
}

void Enemy4::init(const glm::ivec2 &tileMapPos) {
	bJumping = false;
    spritesheet.loadFromFile("images/Enemies/basic-enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(1 / 8.0, 1 / 5.0), &spritesheet, projection);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(STAND_RIGHT, 8);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2((1.0 / 8.0)*0.0, (1.0 / 5.0)*2.5f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2((1.0 / 8.0)*1.0, (1.0 / 5.0)*2.5f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2((1.0 / 8.0)*2.0, (1.0 / 5.0)*2.5f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2((1.0 / 8.0)*3.0, (1.0 / 5.0)*2.5f));


    sprite->changeAnimation(0, false);
    tileMapDispl = tileMapPos;

    collider->addCollider(glm::ivec4(5, 3, 25, 30));
    collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	startY = pos.y;
}

void Enemy4::update(int deltaTime)
{
    sprite->update(deltaTime);
	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP/2.0;
		if (jumpAngle >= 180) {
			bJumping = false;
			collider->changePositionRelative(glm::vec2(0, startY - pos.y));
			pos.y = startY;
		} else {
            CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Enemy4::collider, glm::vec2(0, (startY - 96.0f * sin(3.14159f * jumpAngle / 180.f) / 2) - pos.y));

			if (info.colliding) {
				bJumping = false;
            } else {
                collider->changePositionRelative(glm::vec2(0, (startY - 96.0f * sin(3.14159f * jumpAngle / 180.f) / 2) - pos.y));
                pos.y = (startY - 96.0f * sin(3.14159f * jumpAngle / 180.f) / 2);
            }
		}
	} else {
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Enemy4::collider, glm::vec2(0, FALL_STEP));

		if (startY <= pos.y || info.colliding) {
			bJumping = true;
			jumpAngle = 0;
			startY = pos.y;
        } else {
            pos.y += FALL_STEP;
            collider->changePositionRelative(glm::vec2(0, FALL_STEP));
        }
	}

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

