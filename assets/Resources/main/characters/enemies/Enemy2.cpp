#include "Enemy2.h"
#include "Game.h"


Enemy2::Enemy2(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos) :Character(project, id, Collision::Enemy) {

	init(tileMapPos);
}

void Enemy2::init(const glm::ivec2 &tileMapPos) {
	bJumping = false;
	shootDelay = 60;
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Enemies);


	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1 / 16.0, 1 / 10.0), spritesheet, projection);
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
    collisionSystem->addColliderIntoGroup(collider);
    collisionSystem->updateCollider(collider, glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
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
	shoot();
	CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Enemy2::collider, glm::vec2(-1, 0));
	if (info.colliding) {
		if (info.collider->collisionGroup == Collision::CollisionGroups::Player) {
				CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
				CharacterFactory::getInstance()->damageCharacter(id, 1);
		}
	}else {
        pos.x -= 1;
        collider->changePositionAbsolute(pos);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	jumpAngle += JUMP_ANGLE_STEP+1;
	if (jumpAngle >= 360) {
		jumpAngle = 0;
	}
	else {
		CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::vec2(0, (startY - 30.0f * sin(3.14159f * jumpAngle / 180.f)) - pos.y));

		if (info.colliding) {
			if (info.collider->collisionGroup == Collision::CollisionGroups::Map) {
				if (jumpAngle < 180)jumpAngle = 180 - jumpAngle;
				if (jumpAngle > 180)jumpAngle = 360 - (jumpAngle - 180);
			} else if (info.collider->collisionGroup == Collision::CollisionGroups::Player) {
				CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
				CharacterFactory::getInstance()->damageCharacter(id, 1);

			}
		}

		else {
            pos.y = startY - 30.0f * sin(3.14159f * jumpAngle / 180.f);
            collider->changePositionAbsolute(pos);
		}
	}

	Character::update(deltaTime);
}

void Enemy2::shoot() {
	if (shootDelay == 0) {
		shootDelay = 60;
		int valor = rand() % 10;
		if (valor == 1) {
			glm::vec2 playerpos;
			bool existsPlayer = CharacterFactory::getInstance()->getPlayerPos(playerpos);

			if (!existsPlayer) return;

			glm::vec2 dir = (playerpos + glm::vec2(16.f, 7.f)) - (pos + glm::vec2(6.0f, 9.0f));

			float angle = atan2(dir.y, dir.x);
			dir = glm::vec2(cos(angle), sin(angle));

			float velocity = 2.0f;
			dir *= velocity;

			ProjectileFactory::getInstance()->spawnProjectile(pos + glm::vec2(6.0f, 9.0f), dir, false, Projectile::EnemyProjectile);
		}
	}
	else shootDelay -= 1;
}
