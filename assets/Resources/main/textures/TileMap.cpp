#include "TileMap.h"

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, glm::mat4 *project) {
    TileMap *map = new TileMap(levelFile, minCoords, project);
    return map;
}

TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, glm::mat4 *project) {
    projection = project;
    initShaders();
    loadLevel(levelFile, minCoords);
    prepareArrays(minCoords);
}

TileMap::~TileMap() {
    if(map != NULL)
        delete map;
}

void TileMap::moveMap(float increment) {
    position += increment;
    collision->changePositionRelative(glm::vec2(increment, 0));
    render();
}

void TileMap::render() {

    shaderProgram.use();
    shaderProgram.setUniformMatrix4f("projection", *projection);
    shaderProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

    glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f, 0.f));
    shaderProgram.setUniformMatrix4f("modelview", modelview);
    glEnable(GL_TEXTURE_2D);
    tilesheet.use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
    glDisable(GL_TEXTURE_2D);

#ifdef SHOW_HIT_BOXES
    //TODO: find a way to indicate if the collision boxes are loaded
    collision->render();
#endif // SHOW_HIT_BOXES
}

void TileMap::free() {
    glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile, const glm::vec2 &minCoords) {
    ifstream fin;
    string line, tilesheetFile, collisionFile;
    stringstream sstream;

    fin.open(levelFile.c_str());
    if(!fin.is_open()) return false;
    getline(fin, line);

    if (line.compare(0, 13, "BOX_COLLISION") == 0) return loadGame(fin, minCoords);
    else if (line.compare(0, 9, "MAIN_MENU") == 0) return loadStaticImage(fin, minCoords);
    else if (line.compare(0, 7, "LOADING") == 0) return loadStaticImage(fin, minCoords);

    return true;
}

bool TileMap::loadGame(ifstream &fin, const glm::vec2 &minCoords) {
    string line, tilesheetFile, collisionFile;
    stringstream sstream;

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

    tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
    tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
    tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
    tilesheet.setMinFilter(GL_NEAREST);
    tilesheet.setMagFilter(GL_NEAREST);

    // Collisions File
    getline(fin, line);
    sstream.str(line);
    sstream >> collisionFile;

    if (collisionFile.compare(0, 4, "null") != 0) {
        if (!getCollisions(collisionFile)) return false;
    }

    // Number of block in Map
    getline(fin, line);
    sstream.str(line);
    sstream >> blocksheetSize.x >> blocksheetSize.y;
    tileTexSize = glm::vec2(1.f / blocksheetSize.x, 1.f / blocksheetSize.y);

    map = new int[mapSize.x * mapSize.y];

    fin.close();

    return true;
}

bool TileMap::loadStaticImage(ifstream &fin, const glm::vec2 &minCoords) {
    string line, tilesheetFile, collisionFile;
    stringstream sstream;

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

    tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
    tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
    tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
    tilesheet.setMinFilter(GL_NEAREST);
    tilesheet.setMagFilter(GL_NEAREST);

    // Collisions File
    getline(fin, line);
    sstream.str(line);
    sstream >> collisionFile;

    if (collisionFile.compare(0, 4, "null") != 0) {
        if (!getCollisions(collisionFile)) return false;
    }

    // Number of block in Map
    getline(fin, line);
    sstream.str(line);
    sstream >> blocksheetSize.x >> blocksheetSize.y;
    tileTexSize = glm::vec2(1.f / blocksheetSize.x, 1.f / blocksheetSize.y);

    map = new int[mapSize.x * mapSize.y];

    fin.close();

    collision = new Collision(0,projection, Collision::Uknown);
    collision->addCollider(glm::ivec4(-1,0,-2,1));

#ifdef SHOW_HIT_BOXES
    collision->showHitBox();
#endif // SHOW_HIT_BOXES

    return true;
}

bool TileMap::getCollisions(const string &collisionFile) {
    ifstream fin;
    string line;
    stringstream sstream;

    fin.open(collisionFile.c_str());
    if (!fin.is_open()) return false;

    // Get number of collision boxes
    int collidersSize;
    collision = new Collision(-1,projection, Collision::Map);

    collisionSystem = CollisionSystem::getInstance();
    collisionSystem->addColliderIntoGroup(collision);

    getline(fin, line);
    sstream.str(line);
    sstream >> collidersSize;

    for (int i = 0; i < collidersSize; ++i) {
        int x, y, z, w;
        stringstream aa;

        getline(fin, line);
        aa.str(line);
        aa >> x >> y >> z >> w;
        collision->addCollider(glm::ivec4(x, y, z, w));
    }

#ifdef SHOW_HIT_BOXES
    collision->showHitBox();
#endif // SHOW_HIT_BOXES

    fin.close();

    return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords) {
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
    posLocation = shaderProgram.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
    texCoordLocation = shaderProgram.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));

    position = 0;
}

void TileMap::initShaders() {
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