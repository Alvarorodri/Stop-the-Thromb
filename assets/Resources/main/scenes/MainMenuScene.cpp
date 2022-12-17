#include "MainMenuScene.h"
#include "Game.h"
#include "ui\UI_Button.h"
#include "ui\UI_Contador.h"
#include "sound\AudioManager.h"
#include "GeneralDefines.h"

MainMenuScene *MainMenuScene::getMainMenu() {
    MainMenuScene *scene = new MainMenuScene();
    return scene;
}

MainMenuScene::MainMenuScene() {
    map = NULL;
    buttons = vector<UI_Button>(4);
    selectedButton = 0;
}

MainMenuScene::~MainMenuScene() {
    if (map != NULL)
        delete map;
}

void MainMenuScene::init() {

    // 256.0f is the amount of pixel that has the map as height, it may need a rework to get that value directly from level.txt
    projection = glm::ortho(0.f, float((SCREEN_WIDTH / float(SCREEN_HEIGHT / 255.0f))-2), float((SCREEN_HEIGHT / float(SCREEN_HEIGHT / 255.0f))-2), 0.f);
    currentTime = 0.0f;

    map = TileMap::createTileMap("levels/MainMenu.txt", glm::vec2(SCREEN_X, SCREEN_Y), &projection);

    buttons[0].init(0, glm::ivec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f),          "Play",         32, this);
    buttons[1].init(1, glm::ivec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 50),     "Instructions", 32, this);
    buttons[2].init(2, glm::ivec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 100),    "Credits",      32, this);
    buttons[3].init(3, glm::ivec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 150),    "Quit Game",    32, this);

    counter.init(4, glm::ivec2(SCREEN_WIDTH - 200.0f, 50.0f), "Timer: 00:00", 32, this);
    counter.setTime(20000);
    counter.setState(UI_Contador::ContadorStates::Started);

    buttons[selectedButton].setState(UI_Button::Selected);
    enableControls = true;

}

void MainMenuScene::update(int deltaTime) {

	/*if (!playedMusic) {
		SDL2Music music;
		music.addMusicTrack("sounds/Menu.mp3");
		music.playMusicTrack(0);
		currentTime += deltaTime;
		playedMusic = true;
	}*/

    for (int i = 0; i < 4; i++) buttons[i].update(deltaTime);

    counter.update(deltaTime);

    if (enableControls) {
        if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !latchKeys[GLUT_KEY_UP]) {
            latchKeys[GLUT_KEY_UP] = true;
            buttons[selectedButton].setState(UI_Button::Unselected);
			AudioManager::getInstance()->playSoundEffect(AudioManager::ButtonSelected, 128);
            selectedButton--;
            if (selectedButton < 0) selectedButton = buttons.size() - 1;

            buttons[selectedButton].setState(UI_Button::Selected);

        }
        else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !latchKeys[GLUT_KEY_DOWN]) {
            latchKeys[GLUT_KEY_DOWN] = true;
            buttons[selectedButton].setState(UI_Button::Unselected);
			AudioManager::getInstance()->playSoundEffect(AudioManager::ButtonSelected,128);
            selectedButton++;
            if (selectedButton >= (int)buttons.size()) selectedButton = 0;

            buttons[selectedButton].setState(UI_Button::Selected);
        }
        else if (Game::instance().getKey('x') && !latchKeys['x']) {
            latchKeys['x'] = true;
			AudioManager::getInstance()->playSoundEffect(AudioManager::ButtonPressed, 128);
            buttons[selectedButton].setState(UI_Button::Clicked);
        }
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !latchKeys[GLUT_KEY_RIGHT]) {
			latchKeys[GLUT_KEY_RIGHT] = true;

			//AudioManager::getInstance()->playSoundEffect(0);
		}

        if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN) && latchKeys[GLUT_KEY_DOWN]) latchKeys[GLUT_KEY_DOWN] = false;
        else if (!Game::instance().getSpecialKey(GLUT_KEY_UP) && latchKeys[GLUT_KEY_UP]) latchKeys[GLUT_KEY_UP] = false;
        else if (!Game::instance().getKey('x') && latchKeys['x']) latchKeys['x'] = false;
		else if (!Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && latchKeys[GLUT_KEY_RIGHT]) latchKeys[GLUT_KEY_RIGHT] = false;
    }
}

void MainMenuScene::render() {
    map->render();
    for (int i = 0; i < (int)buttons.size(); i++) buttons[i].render();
    counter.render();
}

void MainMenuScene::buttonCallback(int id) {
    switch (id) {
    case 0:
        Game::instance().changeToGame(true);
        enableControls = false;
		playedMusic = false;
        break;
    case 1:
        Game::instance().changeToInstructions(true);
        enableControls = false;
        break;
    case 2:
        Game::instance().changeToCredits(true);
        enableControls = false;
        break;
    case 3:
        Game::instance().exit();
        enableControls = false;
        break;
    }
}
