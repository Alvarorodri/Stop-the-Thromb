#include "Boss.h"

Boss::Boss(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos):Character(project, id, Collision::Enemy) {
	init(tileMapPos);
}

void Boss::init(const glm::ivec2 &tileMapPos) {
    bJumping = false;
    spritesheet.loadFromFile("images/enemies/Boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	//Body
    sprite = Sprite::createSprite(glm::ivec2(110, 92), glm::vec2(1.f/2.209090909f, 1/ 2.630434782608696f), &spritesheet, projection);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(STAND_RIGHT, 8);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
    sprite->changeAnimation(0,false);
    tileMapDispl = tileMapPos;

	//Head
	spriteHead = Sprite::createSprite(glm::ivec2(32, 34), glm::vec2(1.f / 7.59375f, 1 / 7.5625f), &spritesheet, projection);
	spriteHead->setNumberAnimations(2);
	spriteHead->setAnimationSpeed(0, 8);
	spriteHead->addKeyframe(0, glm::vec2(1.f / 7.59375f*3.f, 1 / 7.5625f*6.f));

	spriteHead->setAnimationSpeed(1, 4);
	for (int i = 0; i < 7; ++i) {
		spriteHead->addKeyframe(1, glm::vec2(1.f / 7.59375f*4.f, 1 / 7.5625f*6.f));
		spriteHead->addKeyframe(1, glm::vec2(1.f / 7.59375f*5.f, 1 / 7.5625f*6.f));
	}
	spriteHead->changeAnimation(0, false);
//Parts
	//Right
	spriteRightPart = Sprite::createSprite(glm::ivec2(26, 30), glm::vec2(1.f / 9.3461538461f, 1 / 7.5625f), &spritesheet, projection);
	spriteRightPart->setNumberAnimations(3);
	spriteRightPart->setAnimationSpeed(0, 8);
	spriteRightPart->addKeyframe(0, glm::vec2(1.f / 9.3461538461f*0.f, 1 / 7.5625f*5.f));

	spriteRightPart->setAnimationSpeed(1, 8);
	for (int i = 0; i < 7; ++i) {
		spriteRightPart->addKeyframe(1, glm::vec2(1.f / 9.3461538461f*0.f, 1 / 7.5625f*5.f));
		spriteRightPart->addKeyframe(1, glm::vec2(1.f / 9.3461538461f*1.f, 1 / 7.5625f*5.f));

	}

	spriteRightPart->setAnimationSpeed(2, 8);
	spriteRightPart->addKeyframe(2, glm::vec2(1.f / 9.3461538461f*2.f, 1 / 7.5625f*5.f));

	spriteRightPart->changeAnimation(1, false);
	//Left
	spriteLeftPart = Sprite::createSprite(glm::ivec2(26, 30), glm::vec2(1.f / 9.3461538461f, 1 / 7.5625f), &spritesheet, projection);
	spriteLeftPart->setNumberAnimations(3);
	spriteLeftPart->setAnimationSpeed(0, 8);
	spriteLeftPart->addKeyframe(0, glm::vec2(1.f / 9.3461538461f*0.f, 1 / 7.5625f*5.f));

	spriteLeftPart->setAnimationSpeed(1, 8);
	for (int i = 0; i < 7; ++i) {
		spriteLeftPart->addKeyframe(1, glm::vec2(1.f / 9.3461538461f*0.f, 1 / 7.5625f*5.f));
		spriteLeftPart->addKeyframe(1, glm::vec2(1.f / 9.3461538461f*1.f, 1 / 7.5625f*5.f));
	}

	spriteLeftPart->setAnimationSpeed(2, 8);
	spriteLeftPart->addKeyframe(2, glm::vec2(1.f / 9.3461538461f*2.f, 1 / 7.5625f*5.f));
	spriteLeftPart->changeAnimation(1, false);
	spriteLeftPart->setRotation(glm::vec3(0.f,180.f,0.f));

//Tail
	spriteTail = Sprite::createSprite(glm::ivec2(32, 30), glm::vec2(1.f / 7.59375f, 1 / 7.5625f), &spritesheet, projection);
	spriteTail->setNumberAnimations(2);
	spriteTail->setAnimationSpeed(0, 8);
	spriteTail->addKeyframe(0, glm::vec2(1.f / 7.59375f*0.f, 1 / 7.5625f*6.f));

	spriteTail->setAnimationSpeed(1, 4);
	for (int i = 0; i < 7; ++i) {
		spriteTail->addKeyframe(1, glm::vec2(1.f / 7.59375f*1.f, 1 / 7.5625f*6.f));
		spriteTail->addKeyframe(1, glm::vec2(1.f / 7.59375f*2.f, 1 / 7.5625f*6.f));
	}
	spriteTail->changeAnimation(0, false);

//Positions
	//dentral
    collider->addCollider(glm::ivec4(15, 3, 95, 80));
	collider->addCollider(glm::ivec4(37,0,72,3));
	collider->addCollider(glm::ivec4(45,80,65,91));
	//quad1
	collider->addCollider(glm::ivec4(1, 21, 15, 37));
	collider->addCollider(glm::ivec4(3, 15, 15, 21));
	collider->addCollider(glm::ivec4(10, 6, 15, 15));
	collider->addCollider(glm::ivec4(6, 10, 10, 15));
	collider->addCollider(glm::ivec4(5, 50, 15, 57));
	//GreenBall collider->addCollider(glm::ivec4(-1, 40, 15, 55));
	//quad2
	collider->addCollider(glm::ivec4(95, 21, 109, 37));
	collider->addCollider(glm::ivec4(95, 15, 107, 21));
	collider->addCollider(glm::ivec4(95, 6, 100, 15));
	collider->addCollider(glm::ivec4(100, 10, 103, 15));
	collider->addCollider(glm::ivec4(95, 50, 105, 57));
	//GreenBall collider->addCollider(glm::ivec4(95, 40, 112, 54));
	//quad3
	collider->addCollider(glm::ivec4(11, 73, 15, 85));
	collider->addCollider(glm::ivec4(9, 83, 12, 91));
	collider->addCollider(glm::ivec4(21, 80, 29, 84));
	//quad4
	collider->addCollider(glm::ivec4(95, 73, 99, 85));
	collider->addCollider(glm::ivec4(97, 83, 101, 91));
	collider->addCollider(glm::ivec4(81, 80, 89, 84));
	//special

	
    collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
   collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	spriteRightPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x-3), float(tileMapDispl.y + pos.y+30)));
	spriteLeftPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 50), float(tileMapDispl.y + pos.y + 30)));
	spriteTail->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 39), float(tileMapDispl.y + pos.y + 62)));
	spriteHead->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 39), float(tileMapDispl.y + pos.y -2)));
}

void Boss::update(int deltaTime)
{

	sprite->update(deltaTime);
	spriteHead->update(deltaTime);
	spriteTail->update(deltaTime);
	spriteRightPart->update(deltaTime);
	spriteLeftPart->update(deltaTime);
	delay -= 1;

	//Heaf
	if (spriteHead->animation() == 0 && delay == 0) {
		spriteHead->changeAnimation(1, false);
	}
	else if (spriteHead->animation() == 1 && spriteHead->isFinidhedAnimation()) {
		spriteHead->changeAnimation(0, false);
		delay = 100;
	}
	//Right
	if (spriteRightPart->animation() == 0 && delay == 0) {
		spriteRightPart->changeAnimation(1,false);
	}
	else if (spriteRightPart->animation() == 1 && spriteRightPart->isFinidhedAnimation()) {
		spriteRightPart->changeAnimation(2, false);
		delay = 100;
	}
	else if (spriteRightPart->animation() == 2 && delay == 0) {
		spriteRightPart->changeAnimation(0, false);
		delay = 100;
	}
	//Left
	if (spriteLeftPart->animation() == 0 && delay == 0) {
		spriteLeftPart->changeAnimation(1, false);
	}
	else if (spriteLeftPart->animation() == 1 && spriteLeftPart->isFinidhedAnimation()) {
		spriteLeftPart->changeAnimation(2, false);
		delay = 100;
	}
	else if (spriteLeftPart->animation() == 2 && delay == 0) {
		spriteLeftPart->changeAnimation(0, false);
		delay = 100;
	}
	//Tail
	if (spriteTail->animation() == 0 && delay == 0) {
		spriteTail->changeAnimation(1, false);
	}
	else if (spriteTail->animation() == 1 && spriteTail->isFinidhedAnimation()) {
		spriteTail->changeAnimation(0, false);
		delay = 100;
	}


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	spriteRightPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x - 3), float(tileMapDispl.y + pos.y + 30)));
	spriteLeftPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 87), float(tileMapDispl.y + pos.y + 30)));
	spriteTail->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 39), float(tileMapDispl.y + pos.y + 62)));
	spriteHead->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 39), float(tileMapDispl.y + pos.y -2)));


}

void Boss::render() {
	Character::render();
	spriteHead->render();
	spriteTail->render();
	spriteRightPart->render();
	spriteLeftPart->render();
}

void Boss::setPosition(const glm::vec2 &pos) {
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
	spriteRightPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x - 3), float(tileMapDispl.y + pos.y + 30)));
	spriteLeftPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 87), float(tileMapDispl.y + pos.y + 30)));
	spriteTail->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 39), float(tileMapDispl.y + pos.y + 62)));
	spriteHead->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 39), float(tileMapDispl.y + pos.y -2)));



}