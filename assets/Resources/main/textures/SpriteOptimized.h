#ifndef _SPRITE_OPTIMIZED_INCLUDE
#define _SPRITE_OPTIMIZED_INCLUDE

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utilities\Texture.h"
#include "utilities\ShaderProgram.h"
#include "utilities\AnimKeyframes.h"

// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet.
class SpriteOptimized {

private:

    SpriteOptimized(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, glm::mat4 *project);
    SpriteOptimized(GLuint vao, Texture *spritesheet, ShaderProgram *shader, glm::mat4 *project);

    void initShaders();

public:

    // Textured quads can only be created inside an OpenGL context
    static SpriteOptimized *createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, glm::mat4 *project);
    static SpriteOptimized *createDuplicate(GLuint vao, Texture *spritesheet, ShaderProgram *shader, glm::mat4 *project);

    void update(int deltaTime);
    void render();
    void free();

    void setNumberAnimations(int nAnimations);
	void setFinishedAnimation(bool finish);
	bool isFinidhedAnimation();
    void setAnimationSpeed(int animId, int keyframesPerSec);
    void addKeyframe(int animId, const glm::vec2 &frame);
    void changeAnimation(int animId, bool special);
    int animation() const;

    void setPosition(const glm::vec2 &pos);

    GLuint getVao() { return vao; };
    ShaderProgram *getShader() { return shaderProgram; };

private:

    Texture *texture;
    GLuint vao;
    GLuint vbo;
    GLint posLocation, texCoordLocation;
    ShaderProgram *shaderProgram;
    glm::mat4 *projection;

    glm::vec2 position;
    int currentAnimation, currentKeyframe;
    float timeAnimation;
    glm::vec2 texCoordDispl;
    vector<AnimKeyframes> animations;

    bool finishedAnimation;
};

#endif // _SPRITE_OPTIMIZED_INCLUDE
