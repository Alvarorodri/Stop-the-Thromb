#include "PowerUp.h"
#include "Game.h"
#include "GeneralDefines.h"

PowerUp::PowerUp(int id, glm::mat4 *project, const ObjectType type) : Object(id, project, type) {
}

void PowerUp::init() {
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::PoweUp);

    sprite = Sprite::createSprite(glm::ivec2(PowerUp::size, PowerUp::size), glm::vec2(1.0f/4.0f, 1.0f), spritesheet, projection);
    sprite->setNumberAnimations(1);
	float mult = 0.0f;
	sprite->setAnimationSpeed(0, 8);
	if (type == Object::ModifierMBlue) mult = 1.0f;
	else if (type == Object::ModifierMRed) {
		mult = 2.0f;
	}
    
    sprite->addKeyframe(0, glm::vec2((1.f/4.f)* mult, 0.f));
        

    sprite->changeAnimation(0, false);

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
			if (type == ModifierMBlue)CharacterFactory::getInstance()->increasePlayerForce(1);
			break;
		}
	}
	return true;
}
