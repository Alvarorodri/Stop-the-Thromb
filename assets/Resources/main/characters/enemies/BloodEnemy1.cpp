#include "BloodEnemy1.h"
#include "characters/CharacterFactory.h"


BloodEnemy1::BloodEnemy1(glm::mat4* project, int id, const glm::ivec2& tileMapPos) :Character(project, id, Collision::Enemy) {
	init(tileMapPos);
	up = rand() % 2 == 0;
	right = rand() % 2 == 0;
	coagulated = rand() % 100 == 1;
	staystatic = false;
}

void BloodEnemy1::init(const glm::ivec2& tileMapPos) {
	bJumping = false;
	jumpDelay = 250;
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::BloodEnemies);

	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(0.125f, 0.25f), spritesheet, projection);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, 8);
	sprite->addKeyframe(NORMAL, glm::vec2(0.f,0.f));

	sprite->setAnimationSpeed( COAGULATED, 8);
	sprite->addKeyframe(COAGULATED, glm::vec2(0.125f,0.f));

	sprite->changeAnimation(NORMAL, false);
	tileMapDispl = tileMapPos;

	collider->addCollider(glm::ivec4(0, 0, 24, 16));
	collisionSystem->updateCollider(collider, glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
	collider->showHitBox();

#endif // SHOW_HIT_BOXES

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));

    float randomOffset = rand() % 80;

    if (right) randomOffset /= rand() % 3;

    upPosition.x += 20.0f - randomOffset;
    downPosition.x += 20.0f - randomOffset;
}

void BloodEnemy1::update(int deltaTime) {
    sprite->update(deltaTime);

    if (!coagulated) coagulated = rand() % 200 == 1;
    else sprite->changeAnimation(COAGULATED, false);

    if (!staystatic) {
        if (pos.x < 270.0f) attachRoutine();
        else normalRoutine();
    }

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	Character::update(deltaTime);
}

void BloodEnemy1::normalRoutine() {
    CollisionSystem::CollisionInfo info = collisionSystem->isColliding(BloodEnemy1::collider, glm::vec2(-2, 0));
    if (info.colliding) {
        if (coagulated && info.collider->collisionGroup == Collision::CollisionGroups::Player) {
            CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
            CharacterFactory::getInstance()->damageCharacter(id, 1);
        }
        else if (info.collider->collisionGroup == Collision::CollisionGroups::Map) {
            coagulated = true;
        }
        else if (info.collider->collisionGroup == Collision::CollisionGroups::EnemyStatic) {
            collisionSystem->removeColliderFromGroup(collider);
            collider->changeGroup(Collision::CollisionGroups::EnemyStatic);
            collisionSystem->addColliderIntoGroup(collider);

            coagulated = true;
            staystatic = true;
        }
    }
    else {
        pos.x -= 2;
        collider->changePositionAbsolute(pos);
    }


    jumpAngle += JUMP_ANGLE_STEP + 1;
    if (jumpAngle >= 360) {
        jumpAngle = 0;
    }
    else {
        CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::vec2(0, (startY - 30.0f * sin(3.14159f * jumpAngle / 180.f)) - pos.y));

        if (info.colliding) {
            if (info.collider->collisionGroup == Collision::CollisionGroups::Map) {
                if (jumpAngle < 180)jumpAngle = 180 - jumpAngle;
                if (jumpAngle > 180)jumpAngle = 360 - (jumpAngle - 180);
            }
            else if (coagulated && info.collider->collisionGroup == Collision::CollisionGroups::Player) {
                CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
                CharacterFactory::getInstance()->damageCharacter(id, 1);
            }
            else if (info.collider->collisionGroup == Collision::CollisionGroups::EnemyStatic) {
                collisionSystem->removeColliderFromGroup(collider);
                collider->changeGroup(Collision::CollisionGroups::EnemyStatic);
                collisionSystem->addColliderIntoGroup(collider);

                coagulated = true;
                staystatic = true;
            }
        }
        else {
            pos.y = startY - 30.0f * sin(3.14159f * jumpAngle / 180.f);
            collider->changePositionAbsolute(pos);
        }
    }
}

void BloodEnemy1::attachRoutine() {
    glm::vec2 targetPosition;
    if (up) {
        targetPosition = upPosition;
    }
    else targetPosition = downPosition;

    glm::vec2 dir = targetPosition - pos;

    glm::vec2 tmp = dir / ((dir.x > dir.y) ? ((dir.y >= 0.0f) ? dir.x : dir.y) : dir.y);

    
    dir = tmp * glm::vec2(dir.x * tmp.x > 0.0f ? 1.0f : ((dir.x < 0.0f) ? -1.0f : 1.0f), dir.y * tmp.y > 0.0f ? 1.0f : ((dir.y < 0.0f) ? -1.0f : 1.0f));

    CollisionSystem::CollisionInfo info = collisionSystem->isColliding(BloodEnemy1::collider, dir * 1.5f);
    if (info.colliding) {
        if (coagulated && info.collider->collisionGroup == Collision::CollisionGroups::Player) {
            CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
            CharacterFactory::getInstance()->damageCharacter(id, 1);
        }
        else if (info.collider->collisionGroup == Collision::CollisionGroups::Map || info.collider->collisionGroup == Collision::CollisionGroups::EnemyStatic) {
            collisionSystem->removeColliderFromGroup(collider);
            collider->changeGroup(Collision::CollisionGroups::EnemyStatic);
            collisionSystem->addColliderIntoGroup(collider);

            coagulated = true;
            staystatic = true;
        }
    }
    else {
        pos += dir;
        collider->changePositionAbsolute(pos);
    }
}

void BloodEnemy1::deleteRoutine() {
	int random = rand() % 3;
	if (random == 0) ObjectFactory::getInstance()->spawnObject(pos, Object::PowerUpBlue);
	Character::deleteRoutine();
}