#include "Enemy4.h"
#include "Game.h"

Enemy4::Enemy4(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos) :Character(project, id, Collision::Enemy) {

	init(tileMapPos);
}

void Enemy4::init(const glm::ivec2 &tileMapPos) {
	bJumping = false;
	shootDelay = 0;
	jumpAngle2 = 0;
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Enemies);

	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(1 / 8.0, 1 / 5.0), spritesheet, projection);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(STAND_RIGHT, 8);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2((1.0 / 8.0)*0.0, (1.0 / 5.0)*2.5f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2((1.0 / 8.0)*1.0, (1.0 / 5.0)*2.5f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2((1.0 / 8.0)*2.0, (1.0 / 5.0)*2.5f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2((1.0 / 8.0)*3.0, (1.0 / 5.0)*2.5f));


    sprite->changeAnimation(0, false);
    tileMapDispl = tileMapPos;

    collider->addCollider(glm::ivec4(5, 3, 25, 30));
	collisionSystem->updateCollider(collider, glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
    collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(pos.x), float( pos.y)));
	startY = pos.y;
}

void Enemy4::update(int deltaTime) {
    sprite->update(deltaTime);

	glm::vec2 playerpos;
	bool existsPlayer = CharacterFactory::getInstance()->getPlayerPos(playerpos);
	float vel = CharacterFactory::getInstance()->mapSpeed;
	CollisionSystem::CollisionInfo mov = collisionSystem->isColliding(Enemy4::collider,glm::vec2( vel, 0.f));

	if (mov.colliding) {
		if (mov.collider->collisionGroup == Collision::CollisionGroups::Map) {
			pos.x -= vel;
			collider->changePositionRelative(glm::vec2(-vel, 0));
		}
		else if (mov.collider->collisionGroup == Collision::CollisionGroups::Player) {
			CharacterFactory::getInstance()->damageCharacter(mov.collider->getId(), 1);
			CharacterFactory::getInstance()->damageCharacter(id, 1);
		}
	}
	else {
		pos.x += vel;
		collider->changePositionRelative(glm::vec2(vel,0));
	}

	if (bJumping && existsPlayer && (playerpos.x - pos.x) <= 200.f && (playerpos.x - pos.x) >= -200.f) {
		bJumping = false;
		jumpAngle2 = 0;
		startY2 = pos.y;

	}
	else if(!bJumping && existsPlayer && ((playerpos.x - pos.x) > 200.f || (playerpos.x - pos.x) < -200.f)) bJumping = true;

	if (bJumping) {
		jumpAngle += JUMP_ANGLE_STEP/2.0;

		if (jumpAngle >= 180) {
			collider->changePositionRelative(glm::vec2(0, startY - pos.y));
			pos.y = startY;
			jumpAngle = 0;
		}
		else {
            CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Enemy4::collider, glm::vec2(0, (startY - 96.0f * sin(3.14159f * jumpAngle / 180.f) / 2) - pos.y));

			if (info.colliding) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map) {
					jumpAngle -= JUMP_ANGLE_STEP / 2.0;
				} else if (info.collider->collisionGroup == Collision::CollisionGroups::Player){
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(),1);
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}

			}
			else {
                collider->changePositionRelative(glm::vec2(0, (startY - 96.0f * sin(3.14159f * jumpAngle / 180.f) / 2) - pos.y));
                pos.y = (startY - 96.0f * sin(3.14159f * jumpAngle / 180.f) / 2);
            }
		}
	} else {
		jumpAngle = 0;
		startY = pos.y;
		shoot();
		jumpAngle2 += 2.f*PI/180.f;
		if (jumpAngle2 >= 180) {
			collider->changePositionRelative(glm::vec2(0, startY2 - pos.y));
			jumpAngle2 = 0;
		}
		else {
			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Enemy4::collider, glm::vec2(0, cos(jumpAngle2)/ 8.f));

			if (info.colliding) {
					if (info.collider->collisionGroup == Collision::CollisionGroups::Map) {
						jumpAngle2 -= JUMP_ANGLE_STEP / 30.f*PI / 180.f;
					} 
					else if (info.collider->collisionGroup == Collision::CollisionGroups::Player){
						CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
						CharacterFactory::getInstance()->damageCharacter(id, 1);
					}
			}
			else {
				collider->changePositionRelative(glm::vec2(0, cos(jumpAngle2) / 8.f));
				pos.y = cos(jumpAngle2) / 8.f +pos.y;
			}
		}
		if (existsPlayer && (playerpos.y - pos.y > 20.f) || (playerpos.y - pos.y < -0.f)) {
			float pas = -0.5f;
			if (playerpos.y > pos.y) pas = 0.5;
			CollisionSystem::CollisionInfo info = collisionSystem->isColliding(Enemy4::collider, glm::vec2(0.f, pas));

			if (!info.colliding) {
				collider->changePositionRelative(glm::vec2(0, pas));
				pos.y = pas + pos.y;
			}
			else {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Player) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}
			}
		}
	}

    sprite->setPosition(glm::vec2(float( pos.x), float( pos.y)));	
	Character::update(deltaTime);
	
}

void Enemy4::shoot() {
		if (shootDelay == 0) {
			shootDelay = 60;

			int aux = 0;
			if (rot)aux = -5;

			glm::vec2 playerpos;
			bool existsPlayer = CharacterFactory::getInstance()->getPlayerPos(playerpos);

			if (!existsPlayer) return;

			glm::vec2 dir = (playerpos + glm::vec2(16.f, 7.f)) - (pos + glm::vec2(0.f,8.f));

			float angle = atan2(dir.y, dir.x);
			if ((angle <= (PI) && angle >= (5.f*PI/6.f)) || (angle >= (-PI) && angle <= (-5.f*PI / 6.f)) || (angle >= (-1.f*PI / 6.f) && angle <= (1.f*PI / 6.f))) {
				dir = glm::vec2(cos(angle), sin(angle));

				float velocity = 2.0f;
				dir *= velocity;

				ProjectileFactory::getInstance()->spawnProjectile(pos + glm::vec2(0.f, 8.f), dir, false, Projectile::Misil);
			}

		}
		else shootDelay -= 1;
}
