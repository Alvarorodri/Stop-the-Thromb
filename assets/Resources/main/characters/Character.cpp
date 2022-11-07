#include "Character.h"
#include "Game.h"
#include "GeneralDefines.h"

Character::Character(glm::mat4 *project, int id, Collision::CollisionGroups type) {
	projection = project;
	this->id = id;
	bJumping = false;
	jumpAngle = 0;
	landed = false;
	rot = false;
	collider = new Collision(id,projection, type);
	collisionSystem = CollisionSystem::getInstance();
}	

void Character::init(const glm::ivec2 &tileMapPos) {
	throw exception("Not Implemented Method");
}

void Character::update(int deltaTime)
{
	if (pos.x <= -50.0f || pos.x >= 500.0f || pos.y >= 300.0f || pos.y < -50.0f) {
		CharacterFactory::getInstance()->destroyCharacter(id);
		return;
	}

	collisionSystem->updateCollider(collider, pos);
	clippingAvoidance();
}

void Character::render() {
	sprite->render();

#ifdef SHOW_HIT_BOXES
	if (Game::instance().showHBox) collider->render();
#endif // SHOW_HIT_BOXES

}

void Character::damage(int dmg, int id) {
	live -= dmg;
	if(live<=0)CharacterFactory::getInstance()->killCharacter(id);
}

void Character::setTileMap(TileMap *tileMap) {
	tmap = tileMap;
}

void Character::setPosition(const glm::vec2 &pos) {
	this->pos = pos;
	startY = pos.y;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
    collisionSystem->updateCollider(collider, this->pos);
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
}

glm::vec4 Character::getBoundingBox() {
	return collider->getBoundingBox();
}

int Character::getId() {
	return this->id;
}

int Character::getHealth() {
	return this->live;
}

void Character::rotate(const float &angleX, const float &angleY, const float &angleZ) {
	sprite->setRotation(glm::vec3(angleX, angleY, angleZ));
	collider->setBox(sprite->getQuadsize());
	collider->setRotation(glm::vec3(angleX, angleY, angleZ));
}

void Character::deleteRoutine() {
	collisionSystem->removeColliderFromGroup(collider);
	delete collider;
}

void Character::clippingAvoidance() {
	CollisionSystem::CollisionInfo info = collisionSystem->isColliding(collider, glm::vec2(0.0f));

	if (info.colliding && info.collider->collisionGroup == Collision::Map) {
		CharacterFactory::getInstance()->damageCharacter(id,100);
	}
}

void Character::wormReturn(int id, bool upOrDown) {
	throw exception("Not Implemented Method");
}
