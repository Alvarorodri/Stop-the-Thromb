#ifndef _UI_TROMBITO_INCLUDE
#define _UI_TROMBITO_INCLUDE

#include "textures\Sprite.h"
#include "ui\Text.h"
#include "TextureManager.h"

#include "GeneralDefines.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class UI_Trombito {

public:

public:

    UI_Trombito();
    UI_Trombito(glm::mat4* project);

    void init(int id, const glm::vec2 &pos, const string &buttonText, const int &size, void *context);
    void update(int deltaTime);
    void render();

    void setPosition(const glm::vec2 &pos);
    void setColor(const glm::vec3 &_color);
    void setSize(const int &_size);
    void setAlignment(const Text::textAlignment align);

    void setTime(int time);

    void settingText();
    void showTrombito(const string& buttonText, int time);

private:

    Texture* spritesheet;
    Sprite* sprite;
    glm::mat4* projection;
    glm::ivec2 offset1 = glm::ivec2(130, 25);
    glm::ivec2 offset2 = glm::ivec2(130, 47);
    glm::ivec2 offset3 = glm::ivec2(130, 69);

    glm::ivec2 tileMapDispl, posButton;

	string buttonText;
	string buttonText1;
	string buttonText2;
	string buttonText3;

	Text text;

    int fontSize;
    int currentFontSize;

    Text::textAlignment textAlignment;
    glm::vec4 color;

    int initTime;
    int timeRemaining;

    void *buttonContext;
    int buttonId;
    bool state = false;

};

#endif // _UI_TROMBITO_INCLUDE
