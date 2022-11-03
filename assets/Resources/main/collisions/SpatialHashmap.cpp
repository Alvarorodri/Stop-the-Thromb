#include "SpatialHashmap.h"

SpatialHashmap::SpatialHashmap(int size) {
	hashSize = size;
}

SpatialHashmap::~SpatialHashmap() {} 

void SpatialHashmap::insertObject(Collision* a) {
    glm::ivec4 cells = a->colliderBox;

	computeCells(cells, a->position);

    a->cells = cells;

	for (int i = cells.x; i <= cells.z; ++i) {
		for (int j = cells.y; j <= cells.w; ++j) {
			Hashmap[i][j][(int)a->collisionGroup].insert(a);
		}
	}
}

void SpatialHashmap::insertObject(Collision* a, const glm::vec2 &pos) {
	glm::ivec4 cells = a->colliderBox;

	computeCells(cells, a->position);

    a->cells = cells;

	for (int i = cells.x; i <= cells.z; ++i) {
		for (int j = cells.y; j <= cells.w; ++j) {
			Hashmap[i][j][(int)a->collisionGroup].insert(a);
		}
	}
}

void SpatialHashmap::removeObject(Collision* a) {
	for (int i = a->cells.x; i <= a->cells.z; ++i) {
		for (int j = a->cells.y; j <= a->cells.w; ++j) {

			Hashmap[i][j][a->collisionGroup].erase(a);
   
		}
	}
	a->cells = glm::ivec4(-1);
}

void SpatialHashmap::updateObject(Collision* a, const glm::vec2 &newPos) {
    glm::ivec4 cells = a->colliderBox;

	computeCells(cells, a->position);

    if (a->cells.x == cells.x && 
        a->cells.y == cells.y &&
        a->cells.z == cells.z &&
        a->cells.w == cells.w
        ) return;

	removeObject(a);
	insertObject(a, newPos);
}

vector<Collision*> SpatialHashmap::getNearByObjects(const glm::vec2 &pos, const int &radius, bool *groups) {
    vector<Collision*> result;

	glm::ivec4 cells = glm::ivec4(pos.x-radius*1.5f, pos.y-radius*1.5f, pos.x+radius*1.5f, pos.y+radius*1.5f);

	computeCells(cells, glm::vec2(0.0f));

	for (int i = (int)cells.x; i <= (int)cells.z; ++i) {
		for (int j = (int)cells.y; j <= (int)cells.w; ++j) {
            for (int t = 0; t < 10; ++t) {
                if (groups[t]) {
                    set<Collision*>::iterator it = Hashmap[i][j][t].begin();

                    while (it != Hashmap[i][j][t].end()) {
                        if (!(*it)->isSelected) {
                            result.push_back(*it);
                            (*it)->isSelected = true;
                        }

                        ++it;
                    }
                }
            }
		}
	}

    for (int i = 0; i < (int)result.size(); ++i) result[i]->isSelected = false;
	return result;
}

void SpatialHashmap::computeCells(glm::ivec4 &cells, const glm::vec2 &pos) {
	cells.x += (int)(pos.x + tileMapPos);
	cells.y += (int)(pos.y);
	cells.z += (int)(pos.x + tileMapPos);
	cells.w += (int)(pos.y);

	cells.x /= hashSize;
	cells.z /= hashSize;
	cells.y /= hashSize;
	cells.w /= hashSize;

	for (int i = 0; i < 4; ++i) if (cells[i] < 0) cells[i] = 0;
}
