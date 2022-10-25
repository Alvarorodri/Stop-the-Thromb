#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <vector>

#include <glm/glm.hpp>

#include "utilities\ShaderProgram.h"
#include "textures\TileMap.h"
#include "ui\UI_Button.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene {

public:

    Scene();
    ~Scene();

    virtual void init();
    virtual void update(int deltaTime);
    virtual void render();

    virtual void buttonCallback(int id);

};

#endif // _MAIN_MENU_SCENE_INCLUDE
