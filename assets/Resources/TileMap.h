#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Collision.h"
#include "CollisionSystem.h"
#include "Texture.h"
#include "ShaderProgram.h"

#include "GeneralDefines.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, glm::mat4 &project);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, glm::mat4 &project);

	~TileMap();

	void moveMap(int increment);
	void render();
	void free();
	
	int getTileSize() const { return blockSize; }
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

public:


private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	ShaderProgram *shaderProgram;
	glm::mat4 projection;

	int nTiles;
	glm::ivec2 mapSize, sectionSize, blocksheetSize;
	int blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

	int position;

	Collision *collision;
	CollisionSystem *collisionSystem;

};


#endif // _TILE_MAP_INCLUDE


