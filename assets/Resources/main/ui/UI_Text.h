#ifndef _UI_TEXT_INCLUDE
#define _UI_TEXT_INCLUDE

#include "textures\Sprite.h"
#include "ui\Text.h"

#include "GeneralDefines.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class UI_Text {

public:

	UI_Text();

	void init(int id, const glm::vec2 &pos, const string &buttonText, const int &size, void *context);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);
	void setColor(const glm::vec3 &_color);
	void setSize(const int &_size);
	void setAlignment(const Text::textAlignment align);
	void setState(const UI_Button::ButtonState _state);

private:

	glm::ivec2 tileMapDispl, posButton;

	string contentText;
	Text text;

	int fontSize;
	int currentFontSize;

	Text::textAlignment textAlignment;
	glm::vec4 color;

	// in seconds
	int animationTime = 300;
	int currentAnimationTime = 0;

	void *buttonContext;
	int textId;

};

#endif // _UI_BUTTON_INCLUDE
