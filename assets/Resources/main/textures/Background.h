#ifndef _BACKGROUND_INCLUDE
#define _BACKGROUND_INCLUDE

#include <fstream>
#include <sstream>

#include "TextureManager.h"
#include "Sprite.h"
#include "utilities\Texture.h"
#include "utilities\ShaderProgram.h"

#include "GeneralDefines.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.
class Background {

private:

	Background(const string &levelFile, const glm::vec2 &minCoords, glm::mat4 *project);

public:

    // Tile maps can only be created inside an OpenGL context
    static Background *createBackground(const string &levelFile, const glm::vec2 &minCoords, glm::mat4 *project);

    ~Background();

    void render(float mapPos);
	void update(float speed, float mapPos, int deltaTime);
    void free();

private:

    bool loadBackground(const string &levelFile, const glm::vec2 &minCoords);

    void prepareBackground(int backgroundFile, const glm::vec2 &initBackground, const glm::vec2 &sizeBackground);


private:

    GLuint vao;
    GLuint vbo;
    GLint posLocation, texCoordLocation;
    ShaderProgram shaderProgram;
    glm::mat4 *projection;

	vector<vector<Sprite*>> backgrounds = vector<vector<Sprite*>>(0, vector<Sprite*>(0, nullptr));
	vector<vector<Sprite*>> parallax = vector<vector<Sprite*>>(0, vector<Sprite*>(0, nullptr));
};

#endif // _BACKGROUND_INCLUDE
