#ifndef _TRIANGLESRENDERER_INCLUDE
#define _TRIANGLESRENDERER_INCLUDE

#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "GeneralDefines.h"

class TrianglesRenderer {

private:

    TrianglesRenderer::TrianglesRenderer();
    ~TrianglesRenderer();

    void TrianglesRenderer::initShaders();

public:

    static TrianglesRenderer *TrianglesRenderer::createTriangleRender(glm::mat4 *project);

    void TrianglesRenderer::addTriangle(const glm::mat3x2 &vert);
    void TrianglesRenderer::moveHitBoxesRelative(const glm::vec2 &pos);
    void TrianglesRenderer::moveHitBoxesAbsolute(const glm::vec2 &pos);
	void TrianglesRenderer::setRotation(const glm::vec3 &rotation);
    int TrianglesRenderer::setColor(glm::vec3 color);
	void TrianglesRenderer::setBox(const glm::vec2 &size);

    void TrianglesRenderer::send();
    int TrianglesRenderer::render();

private:

    ShaderProgram shaderProgram;
    unsigned int VBO, VAO;

    vector<float> vertices;
    int nTriangles;

    GLint posLocation;
    glm::mat4 *projection;

    glm::vec3 lineColor;
    glm::vec2 position;

	float angleX, angleY, angleZ;
	glm::vec2 box;
};

#endif // _TRIANGLESRENDERER_INCLUDE
