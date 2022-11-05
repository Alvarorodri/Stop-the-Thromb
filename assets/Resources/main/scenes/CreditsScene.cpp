#include "CreditsScene.h"
#include "Game.h"
#include "ui\UI_Button.h"
#include "sound\AudioManager.h"
#include "GeneralDefines.h"

CreditsScene *CreditsScene::getCreditsScene() {
	CreditsScene *scene = new CreditsScene();
	return scene;
}

CreditsScene::CreditsScene() {
	map = NULL;
	buttons = vector<UI_Button>(1);
	selectedButton = 0;
}

CreditsScene::~CreditsScene() {
	if (map != NULL)
		delete map;
}

void CreditsScene::init() {
	posText = glm::vec2(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT);
	fontSize = 24;
	currentFontSize = fontSize;
	textAlignment = Text::Center;
	color = glm::vec4(1, 1, 1, 1);
	content.push_back("CREDITS");
	content.push_back("");
	content.push_back("");
	content.push_back("");
	content.push_back("PROGRAMERS");
	content.push_back("");
	content.push_back("Alexandru Dumitru Maroz");
	content.push_back("&");
	content.push_back("Alvaro Rodriguez Rubio");
	content.push_back("");
	content.push_back("");
	content.push_back("");
	content.push_back("SPITES");
	content.push_back("");
	content.push_back("Belong to IREM");

	
	if (!text.init("fonts/OpenSans-Bold.ttf"))
		cout << "Could not load font!!!" << endl;

	// 256.0f is the amount of pixel that has the map as height, it may need a rework to get that value directly from level.txt
	projection = glm::ortho(0.f, float((SCREEN_WIDTH / float(SCREEN_HEIGHT / 255.0f)) - 2), float((SCREEN_HEIGHT / float(SCREEN_HEIGHT / 255.0f)) - 2), 0.f);
	currentTime = 0.0f;

	map = TileMap::createTileMap("levels/Credits.txt", glm::vec2(SCREEN_X, SCREEN_Y), &projection);

	buttons[0].init(0, glm::ivec2(SCREEN_WIDTH - 100, +50), "Back", 32, this);

	buttons[selectedButton].setState(UI_Button::Selected);
	enableControls = true;

	AudioManager::getInstance()->init("sounds/sounds.txt");
}

void CreditsScene::update(int deltaTime) {
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

void CreditsScene::render() {
	map->render();
	for (int i = 0; i < (int)buttons.size(); i++) buttons[i].render();
	
	
	if (content.size() != 0) {
		bool first = true;
		float aux = posText.y;
		for (int i = 0; i < content.size(); ++i) {
				if (first) {
					first = false;
					text.render(content[i], glm::vec2(posText.x, posText.y + 26 * i), 50, color, textAlignment);
				}
				else {
					if (content[i] == "") posText.y -= 10;
					text.render(content[i], glm::vec2(posText.x, posText.y + 26 * i), currentFontSize, color, textAlignment);
				}	
			}
			posText.y = aux-0.5f;
			if (posText.y <= -SCREEN_HEIGHT)content.clear();
	}
	
}

void CreditsScene::buttonCallback(int id) {

		Game::instance().changeToMainMenu(true);
		enableControls = false;
}
