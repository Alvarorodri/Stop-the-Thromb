#include "SpriteOptimized.h"

SpriteOptimized *SpriteOptimized::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, glm::mat4 *project) {
    SpriteOptimized *quad = new SpriteOptimized(quadSize, sizeInSpritesheet, spritesheet, project);

    return quad;
}

SpriteOptimized *SpriteOptimized::createDuplicate(GLuint vao, Texture *spritesheet, ShaderProgram *shader, glm::mat4 *project) {
    SpriteOptimized *quad = new SpriteOptimized(vao, spritesheet, shader, project);

    return quad;
}

SpriteOptimized::SpriteOptimized(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, glm::mat4 *project) {
    initShaders();
    projection = project;

    float vertices[24] = {	0.f, 0.f, 0.f, 0.f,
                            quadSize.x, 0.f, sizeInSpritesheet.x, 0.f,
                            quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
                            0.f, 0.f, 0.f, 0.f,
                            quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
                            0.f, quadSize.y, 0.f, sizeInSpritesheet.y};

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
    posLocation = shaderProgram->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
    texCoordLocation = shaderProgram->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
    texture = spritesheet;
    currentAnimation = -1;
    position = glm::vec2(0.f);
	finishedAnimation = true;
}

SpriteOptimized::SpriteOptimized(GLuint vao, Texture *spritesheet, ShaderProgram *shader, glm::mat4 *project) {
    shaderProgram = shader;
    projection = project;

    SpriteOptimized::vao = vao;
    texture = spritesheet;
    //glBindVertexArray(vao);
    //posLocation = shaderProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    //texCoordLocation = shaderProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    currentAnimation = -1;
    position = glm::vec2(0.f);
    finishedAnimation = true;
}

void SpriteOptimized::update(int deltaTime) {
    if(currentAnimation >= 0) {
        timeAnimation += deltaTime;

		
        while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe) {
            timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
            currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
        }
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
		if (currentKeyframe != animations[currentAnimation].keyframeDispl.size() - 1) {
			finishedAnimation = false;
		}
		else 
			finishedAnimation = true;
		
    }
}

void SpriteOptimized::render() {
    shaderProgram->use();
    shaderProgram->setUniformMatrix4f("projection", *projection);
    shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

    glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
    shaderProgram->setUniformMatrix4f("modelview", modelview);
    shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
    glEnable(GL_TEXTURE_2D);
    texture->use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_TEXTURE_2D);
}

void SpriteOptimized::free() {
    glDeleteBuffers(1, &vbo);
}

void SpriteOptimized::setNumberAnimations(int nAnimations) {
    animations.clear();
    animations.resize(nAnimations);
}

void SpriteOptimized::setFinishedAnimation(bool finish){
	finishedAnimation = finish;

}
bool SpriteOptimized::isFinidhedAnimation() {
	return finishedAnimation;
}

void SpriteOptimized::setAnimationSpeed(int animId, int keyframesPerSec) {
    if(animId < int(animations.size()))
        animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void SpriteOptimized::addKeyframe(int animId, const glm::vec2 &displacement) {
    if(animId < int(animations.size()))
        animations[animId].keyframeDispl.push_back(displacement);
}

void SpriteOptimized::changeAnimation(int animId, bool special) {
    if(animId < int(animations.size())) {
		finishedAnimation = false;
		int pastKeyframe = currentKeyframe;
        currentAnimation = animId;
        currentKeyframe = 0;
        timeAnimation = 0.f;
        texCoordDispl = animations[animId].keyframeDispl[0];
		
    }
}

int SpriteOptimized::animation() const {
    return currentAnimation;
}

void SpriteOptimized::setPosition(const glm::vec2 &pos) {
    position = pos;
}

void SpriteOptimized::initShaders() {
    Shader vShader, fShader;
    shaderProgram = new ShaderProgram();

    vShader.initFromFile(VERTEX_SHADER, "shaders/level-obstacles.vert");
    if (!vShader.isCompiled())
    {
        cout << "Vertex Shader Error" << endl;
        cout << "" << vShader.log() << endl << endl;
    }
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/level-obstacles.frag");
    if (!fShader.isCompiled())
    {
        cout << "Fragment Shader Error" << endl;
        cout << "" << fShader.log() << endl << endl;
    }
    shaderProgram->init();
    shaderProgram->addShader(vShader);
    shaderProgram->addShader(fShader);
    shaderProgram->link();
    if (!shaderProgram->isLinked())
    {
        cout << "Shader Linking Error" << endl;
        cout << "" << shaderProgram->log() << endl << endl;
    }
    shaderProgram->bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();
}
