#include "Enemy2.h"
#include "Game.h"


Enemy2::Enemy2(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos) :Character(project, id, Collision::Enemy) {

	init(tileMapPos);
}

void Enemy2::init(const glm::ivec2 &tileMapPos) {
	bJumping = false;
	shootDelay = 60;
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
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
	collider->showHitBox();
#endif // SHOW_HIT_BOXES
	startY = pos.y;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Enemy2::update(int deltaTime)
{
	sprite->update(deltaTime);
	

	CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Enemy2::collider, glm::vec2(-0.5, 0));

	if (!info.colliding) {
        pos.x -= 0.5;
        collider->changePositionRelative(glm::vec2(-0.5, 0));
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	jumpAngle += JUMP_ANGLE_STEP+1;
	if (jumpAngle >= 360) {
		jumpAngle = 0;
	}
	else {
		CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::vec2(0, (startY - 50.0f * sin(3.14159f * jumpAngle / 180.f)) - pos.y));

		if (info.colliding) {
			if(jumpAngle < 180)jumpAngle = 180-jumpAngle;
			if (jumpAngle > 180)jumpAngle = 360-(jumpAngle-180);
		}

		else {
			collider->changePositionRelative(glm::vec2(0, (startY - 50.0f * sin(3.14159f * jumpAngle / 180.f)) - pos.y));
			pos.y = startY - 50.0f * sin(3.14159f * jumpAngle / 180.f);
		}
	}
	shoot();
	Character::update(deltaTime);
}

void Enemy2::shoot() {
	if (shootDelay == 0) {
		glm::vec2 playerpos = CharacterFactory::getInstance()->player->getPosition();
		glm::vec2 dir = (playerpos+glm::vec2(16.f,7.f))-(pos + glm::vec2(6.0f, 9.0f));

		float angle = atan2(dir.y, dir.x);
		dir = glm::vec2(cos(angle), sin(angle));

		float velocity = 2.0f;
		dir *= velocity;
		
		ProjectileFactory::getInstance()->spawnProjectile(pos + glm::vec2(6.0f, 9.0f), dir, false, Projectile::EnemyProjectile);
		shootDelay = 240;
	}
	else shootDelay -= 1;
}
