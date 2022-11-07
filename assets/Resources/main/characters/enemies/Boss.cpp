#include "Boss.h"
#include "GeneralDefines.h"
#include "characters/CharacterFactory.h"

Boss::Boss(glm::mat4 *project, int id, const glm::ivec2 &tileMapPos):Character(project, id, Collision::Enemy) {
	init(tileMapPos);
}

void Boss::init(const glm::ivec2 &tileMapPos) {
    bJumping = false;
	live = 3000;
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Boss);
	//Body
    sprite = Sprite::createSprite(glm::ivec2(110, 92), glm::vec2(1.f/2.209090909f, 1/ 2.630434782608696f), spritesheet, projection);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(STAND_RIGHT, 8);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
    sprite->changeAnimation(0,false);
    tileMapDispl = tileMapPos;

	//Head
	spriteHead = Sprite::createSprite(glm::ivec2(32, 34), glm::vec2(1.f / 7.59375f, 1 / 7.5625f), spritesheet, projection);
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

	spriteRightPart = Sprite::createSprite(glm::ivec2(26, 30), glm::vec2(1.f / 9.3461538461f, 1 / 7.5625f), spritesheet, projection);
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
	spriteRightPart->changeAnimation(0, false);
	spriteRightPart->setRotation(glm::vec3(0.f, 180.f, 0.f));


	//Left
	spriteLeftPart = Sprite::createSprite(glm::ivec2(26, 30), glm::vec2(1.f / 9.3461538461f, 1 / 7.5625f), spritesheet, projection);
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
	spriteLeftPart->changeAnimation(0, false);

//Tail
	spriteTail = Sprite::createSprite(glm::ivec2(32, 30), glm::vec2(1.f / 7.59375f, 1 / 7.5625f), spritesheet, projection);
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
	for (int i = 0; i < 19; ++i) {
		Collision* coll = new Collision(id, projection, Collision::CollisionGroups::Enemy);;
		coll->addCollider(boxcoordenates[i]);
		collisionSystem->updateCollider(coll, glm::vec2(tileMapDispl.x + pos.x, pos.y));
		coll->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
		collidersBody.push_back(coll);
	}
		Collision* coll = new Collision(id + 1, projection, Collision::CollisionGroups::Enemy);;
		coll->addCollider(glm::ivec4(-1, 40, 15, 55));
		collisionSystem->updateCollider(coll, glm::vec2(0,0));
		coll->changePositionAbsolute(glm::vec2(0, 0));
		collidersGreenBalls.push_back(coll);
		Collision* coll2 = new Collision(id + 2, projection, Collision::CollisionGroups::Enemy);;
		coll2->addCollider(glm::ivec4(95, 40, 112, 54));
		collisionSystem->updateCollider(coll2, glm::vec2(0, 0));
		coll2->changePositionAbsolute(glm::vec2(0, 0));
		collidersGreenBalls.push_back(coll2);



#ifdef SHOW_HIT_BOXES
	for (int i = 0; i < collidersBody.size(); i++) {
		collidersBody[i]->showHitBox();
	}
	for (int i = 0; i < collidersGreenBalls.size(); i++) {
		collidersGreenBalls[i]->showHitBox();
	}
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	spriteLeftPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x-3), float(tileMapDispl.y + pos.y+30)));
	spriteRightPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 50), float(tileMapDispl.y + pos.y + 30)));
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
	if((rand()%700)==1)AudioManager::getInstance()->playSoundEffect(AudioManager::BossRoar, 80);
	if (live <= 0) ExplosionsOfDeath();
	//Head
	if (spriteHead->animation() == 1 && spriteHead->isFinidhedAnimation()) {
		spriteHead->changeAnimation(0, false);
	}
	//Right
	else if (spriteRightPart->animation() == 1 && spriteRightPart->isFinidhedAnimation()) {
		spawnedRight = true;
		spriteRightPart->changeAnimation(2, false);
	}
	//Left
	else if (spriteLeftPart->animation() == 1 && spriteLeftPart->isFinidhedAnimation()) {
		spawnedLeft = true;
		spriteLeftPart->changeAnimation(2, false);
		
	}
	//Tail
	else if (spriteTail->animation() == 1 && spriteTail->isFinidhedAnimation()) {
		spriteTail->changeAnimation(0, false);
	}

	if (live <= 0)spawnGreenBalls();
	if (live <= 0)spawnWorm();
	updateColliders();
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	spriteLeftPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x - 3), float(tileMapDispl.y + pos.y + 30)));
	spriteRightPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 87), float(tileMapDispl.y + pos.y + 30)));
	spriteTail->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 39), float(tileMapDispl.y + pos.y + 62)));
	spriteHead->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 39), float(tileMapDispl.y + pos.y -2)));
	
}

void Boss::render() {
	sprite->render();
	spriteHead->render();
	spriteTail->render();
	spriteRightPart->render();
	spriteLeftPart->render();

#ifdef SHOW_HIT_BOXES
	for (int i = 0; i < collidersBody.size(); i++) {
		collidersBody[i]->render();
	}
	for (int i = 0; i < collidersGreenBalls.size(); i++) {
		collidersGreenBalls[i]->render();
	}
#endif // SHOW_HIT_BOXES
}

void Boss::setPosition(const glm::vec2 &pos) {
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	spriteLeftPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x - 3), float(tileMapDispl.y + pos.y + 30)));
	spriteRightPart->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 87), float(tileMapDispl.y + pos.y + 30)));
	spriteTail->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 39), float(tileMapDispl.y + pos.y + 62)));
	spriteHead->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 39), float(tileMapDispl.y + pos.y - 2)));

	updateColliders();
}

void Boss::updateColliders() {
	for (int i = 0; i < collidersBody.size(); i++) {
		collisionSystem->updateCollider(collidersBody[i], this->pos);
		collidersBody[i]->changePositionAbsolute(this->pos);
	}
	updateBoxBalls();
}

void Boss::updateBoxBalls(){
	if (spawnedLeft) {
		collisionSystem->updateCollider(collidersGreenBalls[0], this->pos);
		collidersGreenBalls[0]->changePositionAbsolute(this->pos);
	}
	else {
		collisionSystem->updateCollider(collidersGreenBalls[0], glm::vec2(SCREEN_WIDTH + 50, SCREEN_HEIGHT + 50));
		collidersGreenBalls[0]->changePositionAbsolute(glm::vec2(SCREEN_WIDTH + 50, SCREEN_HEIGHT + 50));
	}
	if (spawnedRight) {
		collisionSystem->updateCollider(collidersGreenBalls[1], this->pos);
		collidersGreenBalls[1]->changePositionAbsolute(this->pos);
	}
	else {
		collisionSystem->updateCollider(collidersGreenBalls[1], glm::vec2(SCREEN_WIDTH+50, SCREEN_HEIGHT +50));
		collidersGreenBalls[1]->changePositionAbsolute(glm::vec2(SCREEN_WIDTH + 50, SCREEN_HEIGHT + 50));
	}
}

void Boss::damage(int dmg, int id) {
	dmg += 10;
	if (id > this->id) {

		sprite->setFlicker();
		spriteHead->setFlicker();
		spriteLeftPart->setFlicker();
		spriteRightPart->setFlicker();
		spriteTail->setFlicker();

		if (id == (this->id + 1)) { 
			lifeLeft -= dmg;
			if (lifeLeft <= 0) {
				spriteLeftPart->changeAnimation(0, false);
				AudioManager::getInstance()->playSoundEffect(AudioManager::Explode, 40);
				spawnedLeft = false;
				updateBoxBalls();
			}
			live -= dmg;
		}
		if (id == (this->id + 2)) {
			lifeRight -= dmg;
			if (lifeRight <= 0) {
				spriteRightPart->changeAnimation(0, false);
				AudioManager::getInstance()->playSoundEffect(AudioManager::Explode, 40);
				spawnedRight = false;
				updateBoxBalls();
			}
			live -= dmg;
		}
	}
}

void  Boss::spawnWorm() {
	if (delaySpawnWorm == 0 && (!spawnedDown || !spawnedUp)){ 
		if (delaySpawnWormAnimation == 0) { 
			delaySpawnWormAnimation = 30;
			if (spawnedDown || spawnedUp) {
				if (!spawnedDown) {
					isGoingtoSpawnWorm = 0;
					spriteTail->changeAnimation(1, false);
				}
				else {
					isGoingtoSpawnWorm = 1;
					spriteHead->changeAnimation(1, false);
				}
			}
			else {
				int num = rand() % 2;
				if (num == 0) {
					isGoingtoSpawnWorm = 0;
					spriteTail->changeAnimation(1, false);
				}
				else {
					isGoingtoSpawnWorm = 1;
					spriteHead->changeAnimation(1, false);
				}
			}
		}
		delaySpawnWormAnimation -=1;
		if(delaySpawnWormAnimation == 0){
			switch (isGoingtoSpawnWorm)
			{
			case 0:
				CharacterFactory::getInstance()->spawnCharacter(CharacterFactory::CharacterAvailable::cWormDown, glm::vec2(0.0f,0.0f));
				spawnedDown = true;
				break;
			case 1:
				CharacterFactory::getInstance()->spawnCharacter(CharacterFactory::CharacterAvailable::cWormUp, glm::vec2(0.0f, 0.0f));
				spawnedUp = true;
				break;
			}
			delaySpawnWorm = 600;
		}
	}
	if((!spawnedDown || !spawnedUp) && delaySpawnWorm>0) delaySpawnWorm -= 1;
}

void  Boss::spawnGreenBalls() {
	if (delaySpawnGreenBall == 0 && (!spawnedLeft || !spawnedRight)) {
		if (delaySpawnGreenBallAnimation == 0) {
			delaySpawnGreenBallAnimation = 30;
			if (spawnedLeft || spawnedRight) {
				if (!spawnedLeft) {
					isGoingtoSpawnGreenBall = 0;
					spriteLeftPart->changeAnimation(1, false);
				}
				else {
					isGoingtoSpawnGreenBall = 1;
					spriteRightPart->changeAnimation(1, false);
				}
			}
			else {
				int num = rand() % 2;
				if (num == 0) {
					isGoingtoSpawnGreenBall = 0;
					spriteLeftPart->changeAnimation(1, false);
				}
				else {
					isGoingtoSpawnGreenBall = 1;
					spriteRightPart->changeAnimation(1, false);
				}
			}
		}
		delaySpawnGreenBallAnimation -= 1;
		if (delaySpawnGreenBallAnimation == 0) {
			switch (isGoingtoSpawnGreenBall)
			{
			case 0:
				lifeLeft = 30;
				break;
			case 1:
				lifeRight = 30;
				break;
			}
			delaySpawnGreenBall = 600;
		}
	}
	if ((!spawnedLeft || !spawnedRight) && delaySpawnGreenBall>0) delaySpawnGreenBall -= 1;
}

void  Boss::wormReturn(int id, bool upOrDown) {
	if (upOrDown) {
		spawnedUp = false;
		spriteHead->changeAnimation(1, false);
	}
	else {
		spawnedDown = false;
		spriteTail->changeAnimation(1, false);
	}

	int dmg = CharacterFactory::getInstance()->getHealthCharacter(id);
	live -= dmg;

	if (dmg > 0) {
		sprite->setFlicker();
		spriteHead->setFlicker();
		spriteLeftPart->setFlicker();
		spriteRightPart->setFlicker();
		spriteTail->setFlicker();
	}
}

void Boss::ExplosionsOfDeath() {
	if (loop >= 0) {
		if (delayExplosion1 == 20) {
			AudioManager::getInstance()->playSoundEffect(AudioManager::Explode, 80);
			for (int i = 0; i < collidersBody.size(); i += 3) {
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x, boxcoordenates[i].y), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x + 5, boxcoordenates[i].y), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x - 5, boxcoordenates[i].y - 10), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x - 8, boxcoordenates[i].y + 10), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x + 8, boxcoordenates[i].y - 5), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
			}
		}
		if (delayExplosion1 != 0)delayExplosion1 -= 1;
		if (delayExplosion1 == 0 && delayExplosion2 == 20) {
			AudioManager::getInstance()->playSoundEffect(AudioManager::Explode, 80);
			for (int i = 1; i < collidersBody.size(); i += 3) {
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x, boxcoordenates[i].y), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x + 5, boxcoordenates[i].y), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x - 5, boxcoordenates[i].y - 10), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x - 8, boxcoordenates[i].y + 10), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x + 8, boxcoordenates[i].y - 5), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
			}
			AudioManager::getInstance()->playSoundEffect(AudioManager::BossRoar, 128);
		}
		if (delayExplosion1 <= 0 && delayExplosion2 != 0)delayExplosion2 -= 1;
		if (delayExplosion2 == 0) {
			AudioManager::getInstance()->playSoundEffect(AudioManager::Explode, 80);
			for (int i = 2; i < collidersBody.size(); i += 3) {
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x, boxcoordenates[i].y), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x + 5, boxcoordenates[i].y), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x - 5, boxcoordenates[i].y - 10), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x - 8, boxcoordenates[i].y + 10), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
				ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos + glm::vec2(boxcoordenates[i].x + 8, boxcoordenates[i].y - 5), glm::vec4(0, 0, boxcoordenates[i].z - boxcoordenates[i].x, boxcoordenates[i].w - boxcoordenates[i].y));
			}
			AudioManager::getInstance()->playSoundEffect(AudioManager::BossRoar, 128);
			loop -= 1;
			delayExplosion1 = 20;
			delayExplosion2 = 20;

		}
	}
	if (loop == 0) {
		CharacterFactory::getInstance()->destroyCharacter(id);
		CharacterFactory::getInstance()->bossIsDead(true);
		AudioManager::getInstance()->playSoundEffect(AudioManager::Explode, 80);
	}
	
}

void Boss::deleteRoutine() {
	for (int i = 0; i < collidersBody.size(); i++) {
		collisionSystem->removeColliderFromGroup(collidersBody[i]);
		delete collidersBody[i];
	}
	collidersBody.clear();
	for (int i = 0; i < collidersGreenBalls.size(); i++) {
		collisionSystem->removeColliderFromGroup(collidersGreenBalls[i]);
		delete collidersGreenBalls[i];
	}
	collidersGreenBalls.clear();
}
