#ifndef _MAIN_MENU_SCENE_INCLUDE
#define _MAIN_MENU_SCENE_INCLUDE

#include <vector>

#include <glm/glm.hpp>

#include "Scene.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "UI_Button.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class MainMenuScene : public Scene {

private:

    MainMenuScene();

public:

    static MainMenuScene *getMainMenu();
    ~MainMenuScene();

    void init() override;
    void update(int deltaTime) override;
    void render() override;

    void buttonCallback(int id) override;

private:

    TileMap *map;

    vector<UI_Button> buttons;
    int selectedButton;

    float currentTime;
    glm::mat4 projection;

    bool latchKeys[256];
    bool enableControls;
};

#endif // _MAIN_MENU_SCENE_INCLUDE
