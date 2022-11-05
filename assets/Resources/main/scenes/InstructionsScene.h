#ifndef _INSTRUCCIONS_SCENE_INCLUDE
#define _INSTRUCCIONS_SCENE_INCLUDE

#include <vector>

#include <glm/glm.hpp>

#include "Scene.h"
#include "utilities\ShaderProgram.h"
#include "textures\TileMap.h"
#include "ui\UI_Button.h"

class InstructionsScene : public Scene {

private:

	InstructionsScene();

public:

	static InstructionsScene *getInstructions();
	~InstructionsScene();

	void init() override;
	void update(int deltaTime) override;
	void render() override;

	void buttonCallback(int id) override;

private:

	glm::ivec2 posText;

	
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

#endif // _INSTRUCTIONS_SCENE_INCLUDE
