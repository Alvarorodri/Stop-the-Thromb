#include "TrianglesRenderer.h"

TrianglesRenderer::TrianglesRenderer() {
    initShaders();
    lineColor = glm::vec3(0, 0, 1.0f);
    vertices.clear();
    position = glm::vec2(0);
    nTriangles = 0;
	angleX = angleY = angleZ = 0;
	box = glm::vec2(0.f, 0.f);
}

TrianglesRenderer::~TrianglesRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shaderProgram.free();
}

void TrianglesRenderer::initShaders() {
    Shader vShader, fShader;

    vShader.initFromFile(VERTEX_SHADER, "shaders/hitBox.vert");
    if (!vShader.isCompiled())
    {
        cout << "Vertex Shader Error" << endl;
        cout << "" << vShader.log() << endl << endl;
    }

    fShader.initFromFile(FRAGMENT_SHADER, "shaders/hitBox.frag");
    if (!fShader.isCompiled())
    {
        cout << "Fragment Shader Error" << endl;
        cout << "" << fShader.log() << endl << endl;
    }

    shaderProgram.init();
    shaderProgram.addShader(vShader);
    shaderProgram.addShader(fShader);
    shaderProgram.link();
    if (!shaderProgram.isLinked())
    {
        cout << "Shader Linking Error" << endl;
        cout << "" << shaderProgram.log() << endl << endl;
    }

    shaderProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();
}

TrianglesRenderer *TrianglesRenderer::createTriangleRender(glm::mat4 *project) {
    TrianglesRenderer *trianglesRenderer = new TrianglesRenderer();
    trianglesRenderer->projection = project;
    return trianglesRenderer;
}

void TrianglesRenderer::addTriangle(const glm::mat3x2 &vert) {
    nTriangles++;

    for (int i = 0; i < vert.length(); ++i) {
        for (int j = 0; j < vert[0].length(); ++j) {
            vertices.push_back(vert[i][j]);
        }
    }
}

void TrianglesRenderer::moveHitBoxesRelative(const glm::vec2 &pos) {
    position += pos;
}

void TrianglesRenderer::moveHitBoxesAbsolute(const glm::vec2 &pos) {
    position = pos;
}

void TrianglesRenderer::setRotation(const glm::vec3 &rotation) {
	angleX = rotation[0] * PI / 180.f;
	angleY = rotation[1] * PI / 180.f;
	angleZ = rotation[2] * PI / 180.f;
}

int TrianglesRenderer::setColor(glm::vec3 color) {
    lineColor = color;
    return 1;
}

void TrianglesRenderer::setBox(const glm::vec2 &size) {
	box[0] = size[0];
	box[1] = size[1];
}

void TrianglesRenderer::send() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * nTriangles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    posLocation = shaderProgram.bindVertexAttribute("position", 2, 2 * sizeof(float), 0);
}

int TrianglesRenderer::render() {
    shaderProgram.use();

    //setting uniforms of vertex
    shaderProgram.setUniformMatrix4f("projection", *projection);

    glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));

	modelview = glm::translate(modelview, glm::vec3(((box[0]) / 2.f), ((box[1]) / 2.f), 0.f));
	modelview = glm::rotate(modelview, angleZ, glm::vec3(0.f, 0.f, 1.f));
	modelview = glm::rotate(modelview, angleY, glm::vec3(0.f, 1.f, 0.f));
	modelview = glm::rotate(modelview, angleX, glm::vec3(1.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3((-(box[0]) / 2.f), -((box[1]) / 2.f), 0.f));
    shaderProgram.setUniformMatrix4f("modelview", modelview);

    // setting uniforms of fragment
    shaderProgram.setUniform4f("color", lineColor.x, lineColor.y, lineColor.z, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(posLocation);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, 3 * nTriangles);
    glDisable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    return 1;
}
