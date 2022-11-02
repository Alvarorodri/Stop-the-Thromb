#include "Worm.h"

Worm::Worm(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos) :Character(project, id, Collision::Enemy) {
	init(tileMapPos);
}

void Worm::init(const glm::ivec2 &tileMapPos) {
	tileMapDispl = tileMapPos;

	for (int i = 0; i <= 8; ++i) {
		parts.push_back(new Part(projection,id,i,tileMapPos));
	}


	spritesheet.loadFromFile("images/enemies/Boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(26, 22), glm::vec2(1 / 2.0, 1 / 2.0), &spritesheet, projection);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0625*1.0, 0.f));

	sprite->changeAnimation(0, false);
	tileMapDispl = tileMapPos;

	collider->addCollider(glm::ivec4(0, 0, 26, 22));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
	collider->showHitBox();
#endif // SHOW_HIT_BOXES

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));

}

void Worm::update(int deltaTime)
{
	float mov = -1.f;
	CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Worm::collider, glm::vec2(mov, 0));

	if (!info.colliding) {
		pos.x += mov;
		collider->changePositionRelative(glm::vec2(mov,0.f));
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
		for (int i = 0; i < (parts.size() - 1); ++i) {
			parts[i]->setPosition(parts[i + 1]->getPosition());
		}
		parts[(parts.size() - 1)]->setPosition(pos);
	}
	
}

void Worm::render()
{
	sprite->render();
#ifdef SHOW_HIT_BOXES
	collider->render();
#endif // SHOW_HIT_BOXES
	for (int i = 0; i < parts.size(); ++i) {
		parts[i]->render();
	}
}

void Worm::setPosition(const glm::vec2 &pos) {
	for (int i = 0; i < parts.size(); ++i) {
		parts[i]->setPosition(pos);
	}
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
}



Part::Part(glm::mat4 *project, int id, int idBody, const glm::ivec2 &tileMapPos) :Character(project, id, Collision::Enemy) {
	this->idBody = idBody;
	init(tileMapPos);
}

void Part::init(const glm::ivec2 &tileMapPos) {
	bJumping = false;
	spritesheet.loadFromFile("images/enemies/Boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	if (idBody == 8) {
		sprite = Sprite::createSprite(glm::ivec2(26, 22), glm::vec2(1.f / 9.34f, 1.f / 11.f), &spritesheet, projection);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(1.f / 9.34f*0.f, 1.f / 11.f*6.f));

		collider->addCollider(glm::ivec4(0, 0, 26, 22));
		collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
	}
	else if (idBody == 0) {
		sprite = Sprite::createSprite(glm::ivec2(26, 22), glm::vec2(1.f / 9.34f, 1.f / 11.f), &spritesheet, projection);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(1.f / 9.34f*0.f, 1.f / 11.f*6.f));
		rotate(0.f,180.f, 0.f);

		collider->addCollider(glm::ivec4(0, 0, 26, 22));
		collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
	}
	else {
		sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f / 16.f, 1 / 16.0), &spritesheet, projection);
		sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(1.f/16.f*2.f, 1.f / 16.f*9.f));
		
		sprite->setAnimationSpeed(1, 8);
		sprite->addKeyframe(1, glm::vec2(1.f / 16.f*3.f, 1.f / 16.f*9.f));

		collider->addCollider(glm::ivec4(0, 0, 16, 16));
		collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
	}
	

	sprite->changeAnimation(0, false);
	tileMapDispl = tileMapPos;

	

#ifdef SHOW_HIT_BOXES
	collider->showHitBox();
#endif // SHOW_HIT_BOXES

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Part::update(int deltaTime)
{
	
}
