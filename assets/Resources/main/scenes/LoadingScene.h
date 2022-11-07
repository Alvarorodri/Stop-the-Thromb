#ifndef _LOADING_SCENE_INCLUDE
#define _LOADING_SCENE_INCLUDE

#include <glm/glm.hpp>

#include "Scene.h"
#include "textures\TileMap.h"
#include "ui\UI_Button.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class LoadingScene : public Scene {

private:

    LoadingScene();

public:

    static LoadingScene *getLoading();
    ~LoadingScene();

    void init() override;
    void update(int deltaTime) override;
    void render() override;

    void buttonCallback(int id) override;
    void nextScreen(string screen);

private:

    TileMap *map;
	Text text;

    float currentTime;
    glm::mat4 projection;

    string nextScreenName;
    int waitingTime;
};

#endif // _LOADING_SCENE_INCLUDE
