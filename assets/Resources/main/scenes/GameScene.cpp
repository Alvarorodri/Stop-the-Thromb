#include "GameScene.h"
#include "Game.h"
#include "GeneralDefines.h"

GameScene *GameScene::getGame() {
    GameScene *scene = new GameScene();
    return scene;
}

GameScene::GameScene() {
    map = NULL;
	cFactory = NULL;
	cExplosion = NULL;
}

GameScene::~GameScene() {
    if (map != NULL)
        delete map;
    //if (cFactory != NULL) delete cFactory;

}

void GameScene::init() {
    // 256.0f is the amount of pixel that has the map as height, it may need a rework to get that value directly from level.txt
    projection = glm::ortho(0.f, float((SCREEN_WIDTH / float(SCREEN_HEIGHT / 255.0f)) - 2), float((SCREEN_HEIGHT / float(SCREEN_HEIGHT / 255.0f)) - 2), 0.f);
    currentTime = 0.0f;

    initShaders();
    map = TileMap::createTileMap("levels/level00.txt", glm::vec2(SCREEN_X, SCREEN_Y), &projection);

    ProjectileFactory::getInstance()->setProjection(&projection);
    ProjectileFactory::getInstance()->init();
	ProjectileFactory::getInstance()->mapSpeed = map->getSpeed();
    
	cExplosion = ExplosionFactory::getInstance();
	cFactory = CharacterFactory::getInstance();
	cFactory->setProjection(&projection);
	cFactory->setTileMapPos(glm::ivec2(SCREEN_X, SCREEN_Y));
	cFactory->setSpawnFiles("images/background/testing-long-map_entities-computed_entitiesSpawn.txt");
	cFactory->setMap(map);
	cFactory->mapSpeed = map->getSpeed();

	cFactory->spawnCharacter(CharacterFactory::CharacterAvailable::cPlayer, glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	cFactory->spawnCharacter(CharacterFactory::CharacterAvailable::cEnemy1, glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize()+200, INIT_PLAYER_Y_TILES * map->getTileSize()));
	cFactory->spawnCharacter(CharacterFactory::CharacterAvailable::cEnemy2, glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 200, INIT_PLAYER_Y_TILES * map->getTileSize()));
	cFactory->spawnCharacter(CharacterFactory::CharacterAvailable::cEnemy4, glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 200, INIT_PLAYER_Y_TILES * map->getTileSize()));


}

void GameScene::update(int deltaTime) {
    currentTime += deltaTime;
	map->moveMap(map->getSpeed());
	map->update(deltaTime);
	
	//if (Game::instance().getKey('w')) map->moveMap(-5.0);
	//else if (Game::instance().getKey('q')) map->moveMap(5.0);

    cFactory->update(deltaTime);
    ProjectileFactory::getInstance()->update(deltaTime);
	cExplosion->update(deltaTime);
}

void GameScene::render() {
    map->render();

    cFactory->render();
    ProjectileFactory::getInstance()->render();
	cExplosion->render();
}

void GameScene::initShaders() {
    Shader vShader, fShader;

    vShader.initFromFile(VERTEX_SHADER, "shaders/level-obstacles.vert");
    if(!vShader.isCompiled())
    {
        cout << "Vertex Shader Error" << endl;
        cout << "" << vShader.log() << endl << endl;
    }
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/level-obstacles.frag");
    if(!fShader.isCompiled())
    {
        cout << "Fragment Shader Error" << endl;
        cout << "" << fShader.log() << endl << endl;
    }
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    if(!texProgram.isLinked())
    {
        cout << "Shader Linking Error" << endl;
        cout << "" << texProgram.log() << endl << endl;
    }
    texProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();
}
