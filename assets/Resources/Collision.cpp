#include "Collision.h"

Collision::Collision(const glm::mat4 &project, Collision::CollisionGroups group) {
	collidersSize = 0;
	collisions = new glm::ivec4[1000];
	collisionGroup = group;

	position = glm::ivec2(0, 0);

	trianglesRenderer = TrianglesRenderer::createTriangleRender(project);
}

Collision::Collision() {
	collidersSize = 0;
	collisions = new glm::ivec4[1000];

	position = glm::ivec2(0, 0);
}

Collision::~Collision() {
	collidersSize = 0;

	position = glm::ivec2(0, 0);
}

void Collision::setProjection(const glm::mat4 &project) {
	trianglesRenderer = TrianglesRenderer::createTriangleRender(project);
}

void Collision::addCollider(const glm::ivec4 &boxCollider) {
	collisions[collidersSize] = boxCollider;
	collidersSize++;

	// first triangle
	glm::mat3x2 triangle = glm::mat3x2(glm::vec2(boxCollider.x, boxCollider.y),
		glm::vec2(boxCollider.z, boxCollider.y),
		glm::vec2(boxCollider.x, boxCollider.w));
	trianglesRenderer->addTriangle(triangle);

	// second triangle
	triangle = glm::mat3x2(glm::vec2(boxCollider.z, boxCollider.y),
		glm::vec2(boxCollider.z, boxCollider.w),
		glm::vec2(boxCollider.x, boxCollider.w));
	trianglesRenderer->addTriangle(triangle);
}

void Collision::removeCollider(const glm::ivec4 &boxCollider) {
	// Unused
}

void Collision::changePositionAbsolute(const glm::ivec2 &pos) {
	position = pos;
	trianglesRenderer->moveHitBoxesRelative(pos);
}

void Collision::changePositionRelative(const glm::ivec2 &pos) {
	position += pos;
	trianglesRenderer->moveHitBoxesRelative(pos);
}

#ifdef SHOW_HIT_BOXES
void Collision::showHitBox() {
	trianglesRenderer->send();
}

void Collision::render() {
	trianglesRenderer->render();
}
#endif // SHOW_HIT_BOXES





























