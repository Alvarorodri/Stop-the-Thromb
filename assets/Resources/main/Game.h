#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "scenes\GameScene.h"
#include "scenes\MainMenuScene.h"
#include "scenes\LoadingScene.h"
#include "scenes\InstructionsScene.h"
#include "scenes\CreditsScene.h"
#include "GeneralDefines.h"
#include "sound\SDL2Music.h"

// Game is a singleton (a class with a single instance) that represents our whole application
class Game {

private:

    Game() {}
	
public:
	enum Songs {Menu, GameSong, Alert, BossBattle};

    static Game &instance()
    {
        static Game G;

        return G;
    }

    void init();
    bool update(int deltaTime);
    void render();

	void changeToMainMenu(bool lateRender);
    void changeToInstructions(bool lateRender);
    void changeToCredits(bool lateRender);
    void changeToGame(bool lateRender);
    void exit();

    // Input callback methods
    void keyPressed(int key);
    void keyReleased(int key);
    void specialKeyPressed(int key);
    void specialKeyReleased(int key);
    void mouseMove(int x, int y);
    void mousePress(int button);
    void mouseRelease(int button);

    bool getKey(int key) const;
    bool getSpecialKey(int key) const;
	SDL2Music music;
	bool showHBox = false;

    void relativeEnemies(int num);
private:
	
	bool playedMusic = false;
    bool bPlay;                       // Continue to play game?
    Scene *scene;                     // Scene to render
    bool keys[256], specialKeys[256]; // Store key states so that
                                      // we can have access at any time
};

#endif // _GAME_INCLUDE
