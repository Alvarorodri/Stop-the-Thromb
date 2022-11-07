#include "Sprite.h" 

Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, glm::mat4 *project) {
    Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, project);

    return quad;
}

Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, glm::mat4 *project) {
    initShaders();
    projection = project;

	box = quadSize;
    angle = glm::vec3(0.0f);

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
    posLocation = shaderProgram.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
    texCoordLocation = shaderProgram.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
    texture = spritesheet;
    currentAnimation = -1;
    position = glm::vec2(0.f);
	finishedAnimation = true;
    halfFinishedAnimation = true;
}

void Sprite::update(int deltaTime) {
	flickering();
    if(currentAnimation >= 0) {
        timeAnimation += deltaTime;

		
        while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe) {
            timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
            currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
        }
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];

        finishedAnimation = (currentKeyframe == animations[currentAnimation].keyframeDispl.size() - 1);
        halfFinishedAnimation = (currentKeyframe == (int)(animations[currentAnimation].keyframeDispl.size() / 2) + 1);
		
    }
}

void Sprite::render() {
    shaderProgram.use();
    shaderProgram.setUniformMatrix4f("projection", *projection);
    shaderProgram.setUniform4f("color", color.x, color.y, color.z, color.w);

    glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	modelview = glm::translate(modelview, glm::vec3((box.x / 2.0), (box.y/2.0), 0.f));
	modelview = glm::rotate(modelview, angle.z, glm::vec3(0.f, 0.f, 1.f));
	modelview = glm::rotate(modelview, angle.y, glm::vec3(0.f, 1.f, 0.f));
	modelview = glm::rotate(modelview, angle.x, glm::vec3(1.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-(box.x / 2.0), -(box.y / 2.0), 0.f));


    shaderProgram.setUniformMatrix4f("modelview", modelview);
    shaderProgram.setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
    glEnable(GL_TEXTURE_2D);
    texture->use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_TEXTURE_2D);
}

void Sprite::free() {
    glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shaderProgram.free();
}

void Sprite::setNumberAnimations(int nAnimations) {
    animations.clear();
    animations.resize(nAnimations);
}

void Sprite::setFinishedAnimation(bool finish){
	finishedAnimation = finish;

}

void Sprite::setBox(const glm::vec2 &size) {
    box = size;
}

void Sprite::setRotation(const glm::vec3 &rotation) {
    angle = rotation * (PI / 180.f);
}

bool Sprite::isFinidhedAnimation() {
	return finishedAnimation;
}

bool Sprite::isHalfFinidhedAnimation() {
    return halfFinishedAnimation;
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec) {
    if(animId < int(animations.size()))
        animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement) {
    if(animId < int(animations.size()))
        animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId, bool special) {
    if(animId < int(animations.size())) {
		finishedAnimation = false;
        halfFinishedAnimation = false;
		int pastKeyframe = currentKeyframe;
        currentAnimation = animId;
        currentKeyframe = 0;
        timeAnimation = 0.f;
        texCoordDispl = animations[animId].keyframeDispl[0];
		
    }
}

int Sprite::animation() const {
    return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &pos) {
    position = pos;
}

glm::vec2 Sprite::getQuadsize() const{
	return box;
}

void Sprite::flickering() {
	if (flickeringTimeout > 0) {
		if (flickeringTimeout%4 == 0) color = glm::vec4(0.1f, 0.1f, 0.1f, 0.1f);
		else color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		flickeringTimeout--;
	}

	if (flickeringTimeout == 0) {
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		flickeringTimeout--;
	}
}

void Sprite::setFlicker() {
	flickeringTimeout = initflickeringTimeout;
}

void Sprite::initShaders() {
    Shader vShader, fShader;

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
