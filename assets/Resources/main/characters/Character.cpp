#include "Character.h"
#include "Game.h"
#include "GeneralDefines.h"

Character::Character(glm::mat4 *project, int id, Collision::CollisionGroups type) {
	projection = project;
	this->id = id;
	collider = new Collision(projection, type);
	collisionSystem = CollisionSystem::getInstance();
	collisionSystem->addColliderIntoGroup(collider);	
}	

void Character::init(const glm::ivec2 &tileMapPos) {

}

void Character::update(int deltaTime)
{
	
}

void Character::render() {
	sprite->render();

#ifdef SHOW_HIT_BOXES
	collider->render();
#endif // SHOW_HIT_BOXES

}

void Character::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Character::setPosition(const glm::vec2 &pos) {
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
}