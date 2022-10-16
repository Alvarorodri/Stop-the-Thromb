#ifndef _TRIANGLESRENDERER_INCLUDE
#define _TRIANGLESRENDERER_INCLUDE

#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"

class TrianglesRenderer {

private:

    TrianglesRenderer::TrianglesRenderer();
    ~TrianglesRenderer();

    void TrianglesRenderer::initShaders();

public:

    static TrianglesRenderer *TrianglesRenderer::createTriangleRender(glm::mat4 *project);

    void TrianglesRenderer::addTriangle(const glm::mat3x2 &vert);
    void TrianglesRenderer::moveHitBoxesRelative(const glm::ivec2 &pos);
    void TrianglesRenderer::moveHitBoxesAbsolute(const glm::ivec2 &pos);

    int TrianglesRenderer::setColor(glm::vec3 color);

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
    glm::ivec2 position;
};

#endif // _TRIANGLESRENDERER_INCLUDE
