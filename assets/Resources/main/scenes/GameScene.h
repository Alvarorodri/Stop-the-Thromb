#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include <glm/glm.hpp>

#include "Scene.h"
#include "utilities\ShaderProgram.h"
#include "textures\TileMap.h"
#include "characters\player\Player.h"
#include "characters\CharacterFactory.h"
#include "characters\ExplosionFactory.h"
#include "sound\SDL2Music.h"
#include "ui\UI_Contador.h"
#include "ui\UI_Trombito.h"
#include "projectiles\ProjectileFactory.h"
#include "objects\ObjectFactory.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class GameScene : public Scene{

private:

    GameScene();

public:

    static GameScene *getGame();
    ~GameScene();

    void init();
    void update(int deltaTime);
    void render();

    void GameScene::showTrombito(const string& text, int time);

	void setMapSpeed(float newSpeed);
	void teleport(float newPos);

	void inputManager();
	void spawnBoss();
private:

    void initShaders();
	
private:

    TileMap *map;
	CharacterFactory *cFactory;
	ExplosionFactory *cExplosion;
    UI_Trombito trombito;

	UI_Contador counter;

    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
	int contEnd = -1;
	int contspawn = -1;
	bool isSpawnedBoss = false;

	bool latchKeys[256] = { false };

public:
    int staticEnemies = 0;

};

#endif // _SCENE_INCLUDE
