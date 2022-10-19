#include "Enemy4.h"
#include "Game.h"
#include "GeneralDefines.h"

Enemy4::Enemy4(glm::mat4 *project) {
    projection = project;
    collider = new Collision(project, Collision::Enemy);

    collisionSystem = CollisionSystem::getInstance();
    collisionSystem->addColliderIntoGroup(collider);
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
    collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + posEnemy4.x, tileMapDispl.y + posEnemy4.y));

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy4.x), float(tileMapDispl.y + posEnemy4.y)));
	startY = posEnemy4.y;
}

void Enemy4::update(int deltaTime)
{
    sprite->update(deltaTime);
	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP/2.0;
		if (jumpAngle >= 180)
		{
			bJumping = false;
			collider->changePositionRelative(glm::vec2(0, startY - posEnemy4.y));
			posEnemy4.y = startY;
		}
		else
		{
			float antpos = posEnemy4.y;
			posEnemy4.y = (startY - 96.0f * sin(3.14159f * jumpAngle / 180.f)/2);
			collider->changePositionRelative(glm::vec2(0, (startY - 96.0f * sin(3.14159f * jumpAngle / 180.f)/2) - antpos));
			if (collisionSystem->isColliding(Enemy4::collider)) {

				bJumping = false;
				posEnemy4.y = antpos;
				collider->changePositionRelative(glm::vec2(0, -((startY - 96.0f * sin(3.14159f * jumpAngle / 180.f)/2) - antpos)));

			}
		}
	}
	else
	{
		posEnemy4.y += FALL_STEP;
		collider->changePositionRelative(glm::vec2(0, FALL_STEP));
		if (startY <= posEnemy4.y || collisionSystem->isColliding(Enemy4::collider))
		{
			bJumping = true;
			jumpAngle = 0;
			posEnemy4.y -= FALL_STEP;
			startY = posEnemy4.y;
			collider->changePositionRelative(glm::vec2(0, -FALL_STEP));
		}
	}

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy4.x), float(tileMapDispl.y + posEnemy4.y)));
}

void Enemy4::render() {
    sprite->render();

#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES
}

void Enemy4::setTileMap(TileMap *tileMap) {
    map = tileMap;
}

void Enemy4::setPosition(const glm::vec2 &pos) {
    posEnemy4 = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy4.x), float(tileMapDispl.y + posEnemy4.y)));
    collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + posEnemy4.x, tileMapDispl.y + posEnemy4.y));
}
