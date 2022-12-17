#include "PowerUp.h"
#include "Game.h"
#include "GeneralDefines.h"

PowerUp::PowerUp(int id, glm::mat4 *project, const ObjectType type) : Object(id, project, type) {
}

void PowerUp::init() {
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::PoweUp);

    sprite = Sprite::createSprite(glm::ivec2(PowerUp::size, PowerUp::size), glm::vec2(1.0/8.0, 1.0), spritesheet, projection);
    sprite->setNumberAnimations(8);

        for (int i = 0; i < 8; ++i) {
            sprite->setAnimationSpeed(i, 8);
            sprite->addKeyframe(i, glm::vec2((1.0 / 8.0) * i, 0.f));
        }

    sprite->changeAnimation(type, false);

    collider->addCollider(glm::ivec4(0, 0, PowerUp::size, PowerUp::size));
	collisionSystem->addColliderIntoGroup(collider);
	collisionSystem->updateCollider(collider, pos);
    collider->changePositionAbsolute(pos);

	Object::init();
}

void PowerUp::update(int deltaTime) {
	if (!collisionRoutine()) return;
	Object::update(deltaTime);
}

bool PowerUp::collisionRoutine() {
	CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, vel);

	if (info.colliding) {
		switch (info.collider->collisionGroup) {
		case Collision::Force:
		case Collision::Player:
			ObjectFactory::getInstance()->destroyObject(id);
			AudioManager::getInstance()->playSoundEffect(AudioManager::CollectBonus, 128);
			if (type == Object::PowerUpBlue) CharacterFactory::getInstance()->increasePlayerForce(0);
			else if (type == Object::ModifierMBlue) CharacterFactory::getInstance()->increasePlayerForce(1);
			else if (type == Object::ModifierMRed) CharacterFactory::getInstance()->increasePlayerForce(2);
			break;
		}
	}
	return true;
}
