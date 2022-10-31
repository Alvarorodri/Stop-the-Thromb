#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE

#include <iostream>
#include <vector>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utilities\Texture.h"
#include "utilities\ShaderProgram.h"
#include "utilities\AnimKeyframes.h"
#include "GeneralDefines.h"

#define _USE_MATH_DEFINES

// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet.
class Sprite {

private:

    Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, glm::mat4 *project);

    void initShaders();

public:

    // Textured quads can only be created inside an OpenGL context
    static Sprite *createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, glm::mat4 *project);

    void update(int deltaTime);
    void render();
    void free();

    void setNumberAnimations(int nAnimations);
	void setFinishedAnimation(bool finish);
	bool isFinidhedAnimation();
    bool isHalfFinidhedAnimation();
    void setAnimationSpeed(int animId, int keyframesPerSec);
    void addKeyframe(int animId, const glm::vec2 &frame);
    void changeAnimation(int animId, bool special);
    int animation() const;

    void setPosition(const glm::vec2 &pos);
	glm::vec2 getQuadsize() const;

    void setBox(const glm::vec2 &size);
    void setRotation(const glm::vec3 &rotation);

private:

    Texture *texture;
    GLuint vao;
    GLuint vbo;
    GLint posLocation, texCoordLocation;
    ShaderProgram shaderProgram;
    glm::mat4 *projection;

    glm::vec2 position;
    int currentAnimation, currentKeyframe;
    float timeAnimation;
    glm::vec2 texCoordDispl;
    vector<AnimKeyframes> animations;

    bool finishedAnimation, halfFinishedAnimation;

    glm::vec2 box;
    glm::vec3 angle;
};

#endif // _SPRITE_INCLUDE
