#include "Collision.h"

Collision::Collision(int id,glm::mat4 *project, Collision::CollisionGroups group) {
	this->id = id;
    colliderBox = glm::vec4(0);
    collisionGroup = group;

    position = glm::vec2(0, 0);

#ifdef SHOW_HIT_BOXES
	 trianglesRenderer = TrianglesRenderer::createTriangleRender(project);
#endif // SHOW_HIT_BOXES
}

Collision::Collision() {
    colliderBox = glm::vec4(0);

    position = glm::vec2(0, 0);
}

Collision::~Collision() {
    position = glm::vec2(0, 0);
    colliderBox = glm::vec4(0);
}

void Collision::setProjection(glm::mat4 *project) {
#ifdef SHOW_HIT_BOXES
	trianglesRenderer = TrianglesRenderer::createTriangleRender(project);
#endif // SHOW_HIT_BOXES
}

void Collision::addCollider(const glm::vec4 &boxCollider) {
    colliderBox = boxCollider;
	
#ifdef SHOW_HIT_BOXES
	
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
    
#endif // SHOW_HIT_BOXES
}

void Collision::removeCollider(const glm::vec4 &boxCollider) {
    // Unused
}

void Collision::changePositionAbsolute(const glm::vec2 &pos) {
    position = pos;
#ifdef SHOW_HIT_BOXES
	 trianglesRenderer->moveHitBoxesAbsolute(pos);
#endif // SHOW_HIT_BOXES
}

void Collision::changePositionRelative(const glm::vec2 &pos) {
    position += pos;
#ifdef SHOW_HIT_BOXES
	trianglesRenderer->moveHitBoxesRelative(pos);
#endif // SHOW_HIT_BOXES
}

glm::vec4 Collision::getBoundingBox() {
    return colliderBox;
}

void Collision::setRotation(const glm::vec3 &rotation) {
#ifdef SHOW_HIT_BOXES
	 trianglesRenderer->setRotation(rotation);
#endif // SHOW_HIT_BOXES
}

void Collision::setBox(const glm::vec2 &size) {
#ifdef SHOW_HIT_BOXES
	trianglesRenderer->setBox(size);
#endif // SHOW_HIT_BOXES
}

void Collision::changeGroup(Collision::CollisionGroups group) {
    collisionGroup = group;
}

int Collision::getId() {
	return id;
}

#ifdef SHOW_HIT_BOXES
void Collision::showHitBox() {
	trianglesRenderer->send();
}

void Collision::render() {
	 trianglesRenderer->render();
}
#endif // SHOW_HIT_BOXES
