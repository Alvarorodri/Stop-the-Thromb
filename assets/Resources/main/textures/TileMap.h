#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE

#include <fstream>
#include <sstream>

#include "collisions\Collision.h"
#include "collisions\CollisionSystem.h"
#include "utilities\Texture.h"
#include "utilities\ShaderProgram.h"

#include "GeneralDefines.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.
class TileMap {

private:

    TileMap(const string &levelFile, const glm::vec2 &minCoords, glm::mat4 *project);

public:

    // Tile maps can only be created inside an OpenGL context
    static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, glm::mat4 *project);

    ~TileMap();

    void moveMap(float increment);
    void render();
    void free();

    int getTileSize() const { return blockSize; }
	float getPosition() { return position; }

private:

    bool loadLevel(const string &levelFile, const glm::vec2 &minCoords);

    bool loadGame(ifstream &fin, const glm::vec2 &minCoords);
    bool loadStaticImage(ifstream &fin, const glm::vec2 &minCoords);

    bool getCollisions(const string &collisionFile);

    void prepareArrays(const glm::vec2 &minCoords);

    void initShaders();

private:

    GLuint vao;
    GLuint vbo;
    GLint posLocation, texCoordLocation;
    ShaderProgram shaderProgram;
    glm::mat4 *projection;

    int nTiles;
    glm::ivec2 mapSize, sectionSize, blocksheetSize;
    int blockSize;
    Texture tilesheet;
    glm::vec2 tileTexSize;
    int *map;

    float position;

    vector<Collision*> colliders;
    CollisionSystem *collisionSystem;

};

#endif // _TILE_MAP_INCLUDE
