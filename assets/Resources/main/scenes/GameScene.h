#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include <glm/glm.hpp>

#include "Scene.h"
#include "utilities\ShaderProgram.h"
#include "textures\TileMap.h"
#include "characters\player\Player.h"
#include "characters\CharacterFactory.h"

#include "projectiles\ProjectileFactory.h"

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

private:

    void initShaders();

private:

    TileMap *map;
    Player *player;
	CharacterFactory *cFactory;

    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;

};

#endif // _SCENE_INCLUDE
