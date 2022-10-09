#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program) {
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program) {
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap() {
	if(map != NULL)
		delete map;
}

void TileMap::moveMap(int increment) {
	position += increment;
	render();
}

void TileMap::render() const {
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free() {
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile) {
	ifstream fin;
	string line, tilesheetFile, collisionFile;
	stringstream sstream;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 13, "BOX_COLLISION") != 0)
		return false;

	// Entire map size
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;

	// Section map size
	getline(fin, line);
	sstream.str(line);
	sstream >> sectionSize.x >> sectionSize.y;

	// Block Size
	getline(fin, line);
	sstream.str(line);
	sstream >> blockSize;

	// Map File
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;

	// Collisions File
	getline(fin, line);
	sstream.str(line);
	sstream >> collisionFile;

	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	// Number of block in Map
	getline(fin, line);
	sstream.str(line);
	sstream >> blocksheetSize.x >> blocksheetSize.y;
	tileTexSize = glm::vec2(1.f / blocksheetSize.x, 1.f / blocksheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];

	fin.close();

	// Get collisions boxe
	fin.open(collisionFile.c_str());
	if (!fin.is_open())
		return false;

	// Get number of collision boxes
	getline(fin, line);
	sstream.str(line);
	sstream >> collidersSize;

	collisions = new glm::ivec4[collidersSize];
	for (int i = 0; i < collidersSize; ++i) {
		int x, y, z, w;
		stringstream aa;

		getline(fin, line);
		aa.str(line);
		aa >> x >> y >> z >> w;
		collisions[i] = glm::ivec4(x, y, z, w);
	}

	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program) {
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = mapSize.y * mapSize.x;

	for(int i=0; i<mapSize.x; i++) {
		for(int j=0; j<mapSize.y; j++) {
				posTile = glm::vec2(minCoords.x + i * blockSize, minCoords.y + j * blockSize);
				texCoordTile[0] = glm::vec2(
											glm::mod(float(i * tileTexSize.x), 1.0f), 
											glm::mod(float(j * tileTexSize.y) + (int(float(i * tileTexSize.x)) * tileTexSize.y * mapSize.y), 1.0f) 
											);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));

	shaderProgram = &program;
	position = 0;
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) {
	glm::ivec4 objCollider = glm::ivec4(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

	for (int i = 0; i < collidersSize; ++i) {
		if (overlapHorizontal(objCollider, collisions[i])) {
			return true;
		}
	}
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) {
	glm::ivec4 objCollider = glm::ivec4(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

	for (int i = 0; i < collidersSize; ++i) {
		if (overlapHorizontal(objCollider, collisions[i])) {
			return true;
		}
	}
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) {
	glm::ivec4 objCollider = glm::ivec4(pos.x, pos.y, pos.x + size.x, pos.y+size.y);

	for (int i = 0; i < collidersSize; ++i) {
		if (overlapVertical(objCollider, collisions[i])) {
			*posY = pos.y-4;
			return true;
		}
	}

	return false;
}

bool TileMap::overlapVertical(glm::ivec4 r1, glm::ivec4 r2) {
	// if one rectangle is on left side of other
	if (r1[0] > r2[2] || r2[0] > r1[2]) {
		return false;
	}

	// if one rectangle is above other
	if (r1[3] < r2[1] || r1[1] > r2[3]) {
		return false;
	}

	return true;
}

bool TileMap::overlapHorizontal(glm::ivec4 r1, glm::ivec4 r2) {
	// if one rectangle is on left side of other
	if (r1[0] > r2[2] || r2[0] > r1[2]) {
		return false;
	}

	// if one rectangle is above other
	if (r1[3] < r2[1] || r1[1] > r2[3]) {
		return false;
	}

	return true;
}


glm::ivec2 TileMap::getOffset(glm::ivec4 r1, glm::ivec4 r2) {
	return glm::ivec2(0, 0);
}






























