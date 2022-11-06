#include "InstructionsScene.h"
#include "Game.h"
#include "ui\UI_Button.h"
#include "sound\AudioManager.h"
#include "GeneralDefines.h"
#include "sound\SDL2Music.h"

InstructionsScene *InstructionsScene::getInstructions() {
	InstructionsScene *scene = new InstructionsScene();
	return scene;
}

InstructionsScene::InstructionsScene() {
	map = NULL;
	buttons = vector<UI_Button>(1);
	selectedButton = 0;
}

InstructionsScene::~InstructionsScene() {
	if (map != NULL)
		delete map;
}

void InstructionsScene::init() {
	posText = glm::ivec2(100, SCREEN_HEIGHT / 2.0f-50.f);
	fontSize = 24;
	currentFontSize = fontSize;
	textAlignment = Text::Left;
	color = glm::vec4(1, 1, 1, 1);
	content.push_back("INSTRUCTIONS");
	content.push_back("         MOVEMENTS");
	content.push_back("");
	content.push_back("To movearound the map use the arrow keys: up arrow to go up,down arrow to go down,");
	content.push_back("left arrow to go left and right arrowto go right.");
	content.push_back("Press the key 'X' to shoot or maintain or hold to make a charged attack that has more");
	content.push_back("power thana normal one and lasts longer.");
	content.push_back("Press 'D' to be able to see or hide the collision boxes of all the elements of the game.");
	content.push_back("");
	content.push_back("         CHEATS");
	content.push_back("");
	content.push_back("-Invulnerability: press the 'G' key and you will not be able to die, or press it gain to");
	content.push_back(" desactive this option.");
	content.push_back("-Make the Force appear if you don't have it by pressing the 'F' key.");
	content.push_back("-Press the number keys 1, 2 and 3 to jump to specific points in the level.");
	content.push_back("-Press the key 'J' or 'H' to increase or decrease respectively the level of the Force.");

	if (!text.init("fonts/OpenSans-Bold.ttf"))
		cout << "Could not load font!!!" << endl;

	// 256.0f is the amount of pixel that has the map as height, it may need a rework to get that value directly from level.txt
	projection = glm::ortho(0.f, float((SCREEN_WIDTH / float(SCREEN_HEIGHT / 255.0f)) - 2), float((SCREEN_HEIGHT / float(SCREEN_HEIGHT / 255.0f)) - 2), 0.f);
	currentTime = 0.0f;

	map = TileMap::createTileMap("levels/Instructions.txt", glm::vec2(SCREEN_X, SCREEN_Y), &projection);

	buttons[0].init(0, glm::ivec2(SCREEN_WIDTH - 100, +50), "Back", 32, this);

	buttons[selectedButton].setState(UI_Button::Selected);
	enableControls = true;

}

void InstructionsScene::update(int deltaTime) {
	currentTime += deltaTime;

	for (int i = 0; i < buttons.size(); i++) buttons[i].update(deltaTime);

	if (enableControls) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !latchKeys[GLUT_KEY_LEFT]) {
			latchKeys[GLUT_KEY_LEFT] = true;
			buttons[selectedButton].setState(UI_Button::Clicked);

		}

		if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT) && latchKeys[GLUT_KEY_LEFT]) latchKeys[GLUT_KEY_LEFT] = false;
	
	}
}

void InstructionsScene::render() {
	map->render();
	for (int i = 0; i < (int)buttons.size(); i++) buttons[i].render();
	bool first = true;
	int aux = posText.y;
	for (int i = 0; i < content.size(); ++i) {
		if (first) {
			first = false;
			text.render(content[i], glm::ivec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f-100.f), 50, color, Text::Center);
		}
		else {
			if (content[i] == "") posText.y -= 10;
			text.render(content[i], glm::vec2(posText.x, posText.y + 26 * (i - 1)), currentFontSize, color, textAlignment);
		}
		
	}
	posText.y = aux;
}

void InstructionsScene::buttonCallback(int id) {

		Game::instance().changeToMainMenu(true);
		enableControls = false;
}
