#include "Enemy1.h"
#include "characters/CharacterFactory.h"

 
Enemy1::Enemy1(glm::mat4 *project,int id, const glm::ivec2 &tileMapPos):Character(project,id, Collision::Enemy){
	init(tileMapPos);
}

void Enemy1::init(const glm::ivec2 &tileMapPos) {
    bJumping = false;
	jumpDelay = 250;
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Enemies);

    sprite = Sprite::createSprite(glm::ivec2(24,24), glm::vec2(1/16.0, 1/10.0), spritesheet, projection);
    sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2((1.0/16.0)*2.0, (1.0/10.0)*7.0));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2((1.0/16.0)*3.0, (1.0/10.0)*7.0));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2((1.0/16.0)*4.0, (1.0/10.0)*7.0));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2((1.0/16.0)*5.0, (1.0/10.0)*7.0));
		
	sprite->setAnimationSpeed(FLY_LEFT, 8);
	sprite->addKeyframe(FLY_LEFT, glm::vec2((1.0 / 16.0)*1.0, (1.0 / 10.0)*7.0));

	sprite->setAnimationSpeed(LANDING_LEFT, 8);
	sprite->addKeyframe(LANDING_LEFT, glm::vec2((1.0 / 16.0)*2.0, (1.0 / 10.0)*7.0));
	sprite->addKeyframe(LANDING_LEFT, glm::vec2((1.0 / 16.0)*2.0, (1.0 / 10.0)*7.0));
	sprite->addKeyframe(LANDING_LEFT, glm::vec2((1.0 / 16.0)*2.0, (1.0 / 10.0)*7.0));
	sprite->addKeyframe(LANDING_LEFT, glm::vec2((1.0 / 16.0)*2.0, (1.0 / 10.0)*7.0));
	sprite->addKeyframe(LANDING_LEFT, glm::vec2((1.0 / 16.0)*2.0, (1.0 / 10.0)*7.0));


	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2((1.0 / 16.0)*0.0, (1.0 / 10.0)*7.0));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2((1.0 / 16.0)*0.0, (1.0 / 10.0)*7.0));


    sprite->changeAnimation(STAND_LEFT, false);
    tileMapDispl = tileMapPos;

    collider->addCollider(glm::ivec4(5, 8, 21, 23));
	collisionSystem->updateCollider(collider, glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
    collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
	collider->showHitBox();
    
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Enemy1::update(int deltaTime)
{
	if (!bJumping)jumpDelay -= 1;

	sprite->update(deltaTime);

	if (sprite->animation() != LANDING_LEFT || (sprite->animation() == LANDING_LEFT && sprite->isFinidhedAnimation())) {
		
		CollisionSystem::CollisionInfo info;

		if (!rot) info = collisionSystem->isColliding(Enemy1::collider, glm::vec2(-1.5, 0));
		else info = collisionSystem->isColliding(Enemy1::collider, glm::vec2(1.5, 0));

		if (info.colliding) {
			//if(sprite->animation() == MOVE_LEFT)sprite->changeAnimation(STAND_LEFT, false);
			
			if (info.collider->collisionGroup == Collision::CollisionGroups::Map) {
				rot = !rot;
				if (rot)rotate(0.f, 180.f, 0.f);
				else rotate(0.f, 0.f, 0.f);
			}
			else if (info.collider->collisionGroup == Collision::CollisionGroups::Player) {
				CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
				CharacterFactory::getInstance()->damageCharacter(id, 1);

			}

		}
		else {
			if (landed && sprite->animation() != MOVE_LEFT && sprite->animation() != FLY_LEFT && sprite->animation() != JUMP_LEFT) sprite->changeAnimation(MOVE_LEFT, false);

			if (!rot) {
				pos.x -= 1.5;
				collider->changePositionRelative(glm::vec2(-1.5, 0));
			}
			else{
				pos.x += 1.5;
				collider->changePositionRelative(glm::vec2(+1.5, 0));
			}
				
		}
	}

	if (jumpDelay <= 0 && landed) {
		jumpDelay = 250;
		bJumping = true;
		sprite->changeAnimation(JUMP_LEFT, false);
	}

	if (bJumping) {
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180) {
			bJumping = false;
			collider->changePositionRelative(glm::vec2(0, startY-pos.y));
			pos.y = startY;
		} else {
            CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::vec2(0, (startY - 96.0f * sin(3.14159f * jumpAngle / 180.f)) - pos.y));

			if (info.colliding) {
				if (info.collider->collisionGroup == Collision::CollisionGroups::Map) {
					bJumping = false;
				}
				else if (info.collider->collisionGroup == Collision::CollisionGroups::Player) {
					CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
					CharacterFactory::getInstance()->damageCharacter(id, 1);
				}

            }
			else {
                collider->changePositionRelative(glm::vec2(0, (startY - 96.0f * sin(3.14159f * jumpAngle / 180.f)) - pos.y));
                pos.y = startY - 96.0f * sin(3.14159f * jumpAngle / 180.f);
            }
		}
	}
	else {
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::vec2(0, FALL_STEP));

		if (info.colliding) {
			//sprite->changeAnimation(FLY_LEFT, false);
			if (info.collider->collisionGroup == Collision::CollisionGroups::Map) {
				if (sprite->animation() == FLY_LEFT) {
					sprite->changeAnimation(LANDING_LEFT, false);
				}

				bJumping = false;
				landed = true;
				jumpAngle = 0;
				startY = pos.y;
			}
			else if (info.collider->collisionGroup == Collision::CollisionGroups::Player) {
				CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
				CharacterFactory::getInstance()->damageCharacter(id, 1);
			}
			
        }
		else {
			sprite->changeAnimation(FLY_LEFT, false);
            landed = false;
            pos.y += FALL_STEP;
            collider->changePositionRelative(glm::vec2(0, FALL_STEP));
        }
	}

	if (sprite->animation() == JUMP_LEFT && sprite->isFinidhedAnimation()) sprite->changeAnimation(FLY_LEFT, false);

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	Character::update(deltaTime);
}

void Enemy1::deleteRoutine() {
	int random = rand() % 3;
	if (random == 0) ObjectFactory::getInstance()->spawnObject(pos, Object::PowerUpBlue);
	Character::deleteRoutine();
}
