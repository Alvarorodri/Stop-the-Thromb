#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE

#include <vector>

#include <glm/glm.hpp>

#include "Scene.h"
#include "utilities\ShaderProgram.h"
#include "textures\TileMap.h"
#include "ui\UI_Button.h"

class CreditsScene : public Scene {

private:

	CreditsScene();

public:

	static CreditsScene *getCreditsScene();
	~CreditsScene();

	void init() override;
	void update(int deltaTime) override;
	void render() override;

	void buttonCallback(int id) override;

private:

	glm::vec2 posText;

	
	vector<string> content;
	Text text;
	int fontSize;
	int currentFontSize;
	Text::textAlignment textAlignment;
	glm::vec4 color;

	// in seconds
	int animationTime = 300;
	int currentAnimationTime = 0;

	TileMap *map;

	vector<UI_Button> buttons;
	int selectedButton;

	float currentTime;
	glm::mat4 projection;

	bool latchKeys[256];
	bool enableControls;
};

#endif // _CREDITS_INCLUDE
