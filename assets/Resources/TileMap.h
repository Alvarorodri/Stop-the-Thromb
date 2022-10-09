#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void moveMap(int increment);
	void render() const;
	void free();
	
	int getTileSize() const { return 32; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size);
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size);
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY);
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

	bool overlapVertical(glm::ivec4 r1, glm::ivec4 r2);
	bool overlapHorizontal(glm::ivec4 r1, glm::ivec4 r2);
	glm::ivec2 TileMap::getOffset(glm::ivec4 r1, glm::ivec4 r2);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 mapSize, sectionSize, blocksheetSize;
	int blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	int collidersSize;
	glm::ivec4 *collisions;

	int position;

	ShaderProgram *shaderProgram;

};


#endif // _TILE_MAP_INCLUDE


