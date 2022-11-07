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
	map->setSpeed(0);
    ProjectileFactory::getInstance()->setProjection(&projection);
    ProjectileFactory::getInstance()->init();
	ProjectileFactory::getInstance()->mapSpeed = map->getSpeed();
    
	cExplosion = ExplosionFactory::getInstance();
	cExplosion->setMap(map);
	cFactory = CharacterFactory::getInstance();
	cFactory->setProjection(&projection);
	cFactory->setTileMapPos(glm::ivec2(SCREEN_X, SCREEN_Y));
	cFactory->setSpawnFiles("images/background/testing-long-map_entities-computed_entitiesSpawn.txt");
	cFactory->setMap(map);
	cFactory->mapSpeed = map->getSpeed();

	cFactory->spawnCharacter(CharacterFactory::CharacterAvailable::cPlayer, glm::vec2(-30.f, 200.0f));
	cFactory->spawnCharacter(CharacterFactory::CharacterAvailable::cBoss, glm::vec2(200, 100));


	ObjectFactory::getInstance()->setProjection(&projection);
	ObjectFactory::getInstance()->init();
	ObjectFactory::getInstance()->mapSpeed = map->getSpeed();
	
}

void GameScene::update(int deltaTime) {

	inputManager();

    currentTime += deltaTime;
	map->moveMap(map->getSpeed());
	map->update(deltaTime);

    cFactory->update(deltaTime);
    ProjectileFactory::getInstance()->update(deltaTime);
	cExplosion->update(deltaTime);

	ObjectFactory::getInstance()->update(deltaTime);

}

void GameScene::render() {
    map->render();

    cFactory->render();
    ProjectileFactory::getInstance()->render();
	cExplosion->render();

	ObjectFactory::getInstance()->render();

}

void GameScene::setMapSpeed(float newSpeed) {
	map->setSpeed(newSpeed);
	CharacterFactory::getInstance()->mapSpeed = newSpeed;
	ProjectileFactory::getInstance()->mapSpeed = newSpeed;
	ObjectFactory::getInstance()->mapSpeed = newSpeed;
}

void GameScene::teleport(float newPos) {
	map->moveMap(abs(map->getPosition()) - newPos);

	CharacterFactory::getInstance()->destroyAllCharacters();
	ProjectileFactory::getInstance()->destroyAllProjectiles();
	ObjectFactory::getInstance()->destroyAllObjects();
}

void GameScene::inputManager() {
	if (Game::instance().getKey('1') && !latchKeys['1']) {
		latchKeys['1'] = true;
		teleport(0);
	}
	else if (Game::instance().getKey('2') && !latchKeys['2']) {
		latchKeys['2'] = true;
		teleport(1000);
	}
	else if (Game::instance().getKey('3') && !latchKeys['3']) {
		latchKeys['3'] = true;
		teleport(2000);
	}
	else if (Game::instance().getKey('4') && !latchKeys['4']) {
		latchKeys['4'] = true;
		teleport(3000);
	}
	else if (Game::instance().getKey('5') && !latchKeys['5']) {
		latchKeys['5'] = true;
		teleport(4000);
	}
	else if (Game::instance().getKey('p') && !latchKeys['p']) {
		latchKeys['p'] = true;
		setMapSpeed(0.0f);
	}

	if (!Game::instance().getKey('1') && latchKeys['1']) latchKeys['1'] = false;
	else if (!Game::instance().getKey('2') && latchKeys['2']) latchKeys['2'] = false;
	else if (!Game::instance().getKey('3') && latchKeys['3']) latchKeys['3'] = false;
	else if (!Game::instance().getKey('4') && latchKeys['4']) latchKeys['4'] = false;
	else if (!Game::instance().getKey('5') && latchKeys['5']) latchKeys['5'] = false;
	else if (!Game::instance().getKey('p') && latchKeys['p']) latchKeys['p'] = false;
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
