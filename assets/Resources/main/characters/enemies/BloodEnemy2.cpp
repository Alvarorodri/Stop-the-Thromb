#include "BloodEnemy2.h"
#include "characters/CharacterFactory.h"
#include "Game.h"

BloodEnemy2::BloodEnemy2(glm::mat4* project, int id, const glm::ivec2& tileMapPos) :Character(project, id, Collision::Enemy) {
    init(tileMapPos);
    coagulated = true;
}

void BloodEnemy2::init(const glm::ivec2& tileMapPos) {
    live = 20;
    bJumping = false;
    jumpDelay = 250;
    spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::BloodEnemies);

    sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25f, 0.5f), spritesheet, projection);
    sprite->setNumberAnimations(2);

    sprite->setAnimationSpeed(NORMAL, 8);
    sprite->addKeyframe(NORMAL, glm::vec2(0.f, 0.25f));

    sprite->setAnimationSpeed(COAGULATED, 8);
    sprite->addKeyframe(COAGULATED, glm::vec2(0.25f, 0.25f));

    sprite->changeAnimation(COAGULATED, false);
    coagulated = true;
    tileMapDispl = tileMapPos;

    collider->addCollider(glm::ivec4(0, 0, 64, 64));
    collisionSystem->updateCollider(collider, glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
    collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();

#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void BloodEnemy2::update(int deltaTime) {
    sprite->update(deltaTime);

    normalRoutine();

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
    Character::update(deltaTime);
}

void BloodEnemy2::normalRoutine() {
    CollisionSystem::CollisionInfo info = collisionSystem->isColliding(BloodEnemy2::collider, glm::vec2(-0.5f, 0));
    if (info.colliding) {
        if (coagulated && info.collider->collisionGroup == Collision::CollisionGroups::Player) {
            CharacterFactory::getInstance()->damageCharacter(info.collider->getId(), 1);
            CharacterFactory::getInstance()->damageCharacter(id, 1);
        }
        else if (info.collider->collisionGroup == Collision::CollisionGroups::Map) {
            pos.x -= 0.5f;
            collider->changePositionAbsolute(pos);
        }
    }
    else {
        pos.x -= 0.5f;
        collider->changePositionAbsolute(pos);
    }
}

void BloodEnemy2::deleteRoutine() {
	int random = rand() % 3;
	if (random == 0) ObjectFactory::getInstance()->spawnObject(pos, Object::PowerUpBlue);
	Character::deleteRoutine();
}