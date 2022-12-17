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
	posText = glm::ivec2(100, SCREEN_HEIGHT / 3.0f-50.f);
	fontSize = 24;
	currentFontSize = fontSize;
	textAlignment = Text::Left;
	color = glm::vec4(1, 1, 1, 1);
	content.push_back("INSTRUCTIONS");
	content.push_back("");
	content.push_back("OBJECTIVE");
	content.push_back("");
	content.push_back("This game is made to educate about Thrombosis, a disease where blood coagulates and");
	content.push_back("affects people of any age. It is important to have an active life to prevent this disease,");
	content.push_back("like running or doing some sport 30 min a day. If a person has this disease,");
	content.push_back("using Tights or using anticoagulantes like Heparin can help to cure this disease.");
	content.push_back("");
	content.push_back("MOVEMENTS");
	content.push_back("");
	content.push_back("To move around the map use the arrow keys: up arrow to go up,down arrow to go down,");
	content.push_back("left arrow to go left and right arrow to go right.");
	content.push_back("Press the key 'X' to shoot or hold to make a charged attack that has more power");
	content.push_back("than a normal one and lasts longer.");
	content.push_back("");
	content.push_back("BOOSTERS");
	content.push_back("");
	content.push_back("If you acquire the Heparin, all your attacks change to charged attacks during 10s.");
	content.push_back("");
	content.push_back("If you acquire the Tights, your attacks get doubled for 10s.");
	content.push_back("");
	content.push_back("If you acquire the Runner, your attacks get faster for 10s.");
	content.push_back("");


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

	for (int i = 0; i < (int)buttons.size(); i++) buttons[i].update(deltaTime);

	if (enableControls) {
		if (Game::instance().getKey('x') && !latchKeys['x']) {
			latchKeys['x'] = true;
			AudioManager::getInstance()->playSoundEffect(AudioManager::ButtonPressed, 128);
			buttons[selectedButton].setState(UI_Button::Clicked);

		}

		if (!Game::instance().getKey('x') && latchKeys['x']) latchKeys['x'] = false;
	
	}
}

void InstructionsScene::render() {
	map->render();
	for (int i = 0; i < (int)buttons.size(); i++) buttons[i].render();
	bool first = true;
	int aux = posText.y;
	for (int i = 0; i < (int)content.size(); ++i) {
		if (first) {
			first = false;
			text.render(content[i], glm::ivec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f-100.f), 50, color, Text::Center);
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
