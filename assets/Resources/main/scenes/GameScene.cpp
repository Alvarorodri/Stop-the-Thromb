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

	CharacterFactory::getInstance()->deleteReference();
	ProjectileFactory::getInstance()->deleteReference();
	ObjectFactory::getInstance()->deleteReference();
	CollisionSystem::getInstance()->deleteReference();
	ExplosionFactory::getInstance()->deleteReference();

    initShaders();
	contEnd = -1;
    map = TileMap::createTileMap("levels/level00.txt", glm::vec2(SCREEN_X, SCREEN_Y), &projection);
    ProjectileFactory::getInstance()->setProjection(&projection);
    ProjectileFactory::getInstance()->init();
	ProjectileFactory::getInstance()->mapSpeed = map->getSpeed();
    
	cExplosion = ExplosionFactory::getInstance();
	cExplosion->setMap(map);
	cFactory = CharacterFactory::getInstance();
	cFactory->setProjection(&projection);

	cFactory->setTileMapPos(glm::ivec2(SCREEN_X, SCREEN_Y));
	cFactory->setSpawnFiles("images/background/level00_entities-computed_entitiesSpawn.txt");
	cFactory->setMap(map);
	cFactory->mapSpeed = map->getSpeed();

	ObjectFactory::getInstance()->setProjection(&projection);
	ObjectFactory::getInstance()->init();
	ObjectFactory::getInstance()->mapSpeed = map->getSpeed();

	
cFactory->spawnCharacter(CharacterFactory::CharacterAvailable::cPlayer, glm::vec2(-30.f, 128.0f));
	
	setMapSpeed(-1.0f);
	
}

void GameScene::update(int deltaTime) {

	inputManager();

    currentTime += deltaTime;
	map->moveMap(map->getSpeed());
	map->update(deltaTime);

	if (contEnd == 0) {
		CharacterFactory::getInstance()->destroyAllCharactersToEnd();
		CharacterFactory::getInstance()->bossIsDead(false);
		ProjectileFactory::getInstance()->destroyAllProjectiles();
		ObjectFactory::getInstance()->destroyAllObjects();
		ExplosionFactory::getInstance()->deleteAll();
	}

    cFactory->update(deltaTime);
    ProjectileFactory::getInstance()->update(deltaTime);
	cExplosion->update(deltaTime);
	ObjectFactory::getInstance()->update(deltaTime);
	if (contEnd == 0) {
		Game::instance().changeToCredits(true);
		Game::instance().music.playMusicTrack(Game::Songs::Menu);
	}
	if (contEnd == -1 && cFactory->isBossDead()) contEnd = 200;
	else if (contEnd != -1) contEnd -= 1;

	//if(!isSpawnedBoss)spawnBoss();
	if (contspawn > 0) contspawn -= 1;
	else if (contspawn == 0) {
		Game::instance().music.Play_Pause();
		Game::instance().music.playMusicTrack(Game::BossBattle); 
		contspawn -= 1;
	}
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

	CharacterFactory::getInstance()->destroyAllCharactersToTeleport();
	ProjectileFactory::getInstance()->destroyAllProjectiles();
	ObjectFactory::getInstance()->destroyAllObjects();
	ExplosionFactory::getInstance()->deleteAll();
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
		teleport(7400);
	}
	else if (Game::instance().getKey('p') && !latchKeys['p']) {
		latchKeys['p'] = true;
		if (map->getSpeed() != 0.0f) setMapSpeed(0.0f);
		else setMapSpeed(-1.0f);
	}
	else if (Game::instance().getKey('q') && !latchKeys['q']) {
		//latchKeys['q'] = true;
		map->moveMap(10.0f);
		map->update(0);
	}
	else if (Game::instance().getKey('w') && !latchKeys['w']) {
		//latchKeys['w'] = true;
		map->moveMap(-10.0f);
		map->update(0);
	}
	else if (Game::instance().getKey('d') && !latchKeys['d']) {
		latchKeys['d'] = true;
		Game::instance().showHBox = !Game::instance().showHBox;
	}

	if (!Game::instance().getKey('1') && latchKeys['1']) latchKeys['1'] = false;
	else if (!Game::instance().getKey('2') && latchKeys['2']) latchKeys['2'] = false;
	else if (!Game::instance().getKey('3') && latchKeys['3']) latchKeys['3'] = false;
	else if (!Game::instance().getKey('4') && latchKeys['4']) latchKeys['4'] = false;
	else if (!Game::instance().getKey('5') && latchKeys['5']) latchKeys['5'] = false;
	else if (!Game::instance().getKey('p') && latchKeys['p']) latchKeys['p'] = false;
	else if (!Game::instance().getKey('d') && latchKeys['d']) latchKeys['d'] = false;
}

void  GameScene::spawnBoss() {
	AudioManager::getInstance()->playSoundEffect(AudioManager::BossRoar,128);
	Game::instance().music.Play_Pause();
	Game::instance().music.playMusicTrack(Game::Alert);
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
