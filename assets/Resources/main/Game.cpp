#include "Game.h"

void Game::init() {
    bPlay = true;
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	changeToMainMenu(true);
}

bool Game::update(int deltaTime) {

	if (!playedMusic) {
		AudioManager::getInstance()->init("sounds/sounds.txt");
		music.addMusicTrack("sounds/Menu.mp3");
		music.addMusicTrack("sounds/Game.mp3");
		music.addMusicTrack("sounds/RedAlert.mp3");
		music.addMusicTrack("sounds/BossBattle.mp3");
		music.playMusicTrack(Songs::Menu);
		playedMusic = true;
	}
    scene->update(deltaTime);

    return bPlay;
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->render();
}


void Game::changeToMainMenu(bool lateRender) {
	if (lateRender) {
		scene = LoadingScene::getLoading();
		scene->init();
		scene->render();
		reinterpret_cast<LoadingScene*>(scene)->nextScreen("MAIN_MENU");
	}

	if (!lateRender) {
		scene = MainMenuScene::getMainMenu();
		scene->init();
		music.playMusicTrack(Game::Songs::Menu);
	}
}

void Game::changeToInstructions(bool lateRender) {
	if (lateRender) {
		scene = LoadingScene::getLoading();
		scene->init();
		scene->render();
		reinterpret_cast<LoadingScene*>(scene)->nextScreen("INSTRUCTIONS");
	}

	if (!lateRender) {
		scene = InstructionsScene::getInstructions();
		scene->init();
	}
}

void Game::changeToCredits(bool lateRender) {
	if (lateRender) {
		scene = LoadingScene::getLoading();
		scene->init();
		scene->render();
		reinterpret_cast<LoadingScene*>(scene)->nextScreen("CREDITS");
	}

	if (!lateRender) {
		scene = CreditsScene::getCreditsScene();
		scene->init();
	}
}

void Game::changeToGame(bool lateRender) {
    if (lateRender) {
        scene = LoadingScene::getLoading();
        scene->init();
        scene->render();
        reinterpret_cast<LoadingScene*>(scene)->nextScreen("GAME");
    }

    if (!lateRender) {
		music.playMusicTrack(Songs::GameSong);
        scene = GameScene::getGame();
        scene->init();
    }
}

void Game::exit() {
    glutLeaveMainLoop();
}

void Game::keyPressed(int key) {
	if (key == 27) // Escape code
		changeToMainMenu(true);
    keys[key] = true;
}

void Game::keyReleased(int key) {
    keys[key] = false;
}

void Game::specialKeyPressed(int key) {
    specialKeys[key] = true;
}

void Game::specialKeyReleased(int key) {
    specialKeys[key] = false;
}

void Game::mouseMove(int x, int y) {
}

void Game::mousePress(int button) {
}

void Game::mouseRelease(int button) {
}

bool Game::getKey(int key) const {
    return keys[key];
}

bool Game::getSpecialKey(int key) const {
    return specialKeys[key];
}
