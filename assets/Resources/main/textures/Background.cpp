#include "Background.h"
#include "Game.h"

Background *Background::createBackground(const string &levelFile, const glm::vec2 &minCoords, glm::mat4 *project) {
	Background *map = new Background(levelFile, minCoords, project);
    return map;
}

Background::Background(const string &levelFile, const glm::vec2 &minCoords, glm::mat4 *project) {
    projection = project;
    loadBackground(levelFile, minCoords);
}

Background::~Background() {
}

void Background::render(float mapPos) {
	for (int i = 0; i < backgrounds.size(); ++i) {
		for (int j = 0; j < backgrounds[i].size(); ++j) {
			float temp = abs(mapPos) + backgrounds[i][j]->getPosition().x + backgrounds[i][j]->getQuadsize().x;
			if ((i == 0 || i == 1) && (temp < 2048 || temp  >= 4096)) backgrounds[i][j]->render();
			else if (i == 2 && temp >= 2048 && temp  < 4096) backgrounds[i][j]->render();
		}
	}
}

void Background::update(float speed, float mapPos, int deltaTime) {
	for (int i = 0; i < backgrounds.size(); ++i) {
		for (int j = 0; j < backgrounds[i].size(); ++j) {
			glm::vec2 spritePos = backgrounds[i][j]->getPosition();
			glm::vec2 spriteQuad = backgrounds[i][j]->getQuadsize();

			float modifiedSpeed = speed;
			if (i == 0) modifiedSpeed *= 0.5;
			else if (i == 1) modifiedSpeed *= 0.75;

			if (spritePos.x + spriteQuad.x <= 0.0f) backgrounds[i][j]->setPosition(spritePos + glm::vec2(spriteQuad.x * backgrounds[i].size() + modifiedSpeed,0.0f));
			else backgrounds[i][j]->setPosition(spritePos + glm::vec2(modifiedSpeed, 0.0f));
			
			backgrounds[i][j]->update(deltaTime);
		}
	}
}

void Background::free() {
    glDeleteBuffers(1, &vbo);
}

bool Background::loadBackground(const string &levelFile, const glm::vec2 &minCoords) {

	glm::ivec2 initBackground, sizeBackground;
	int spritesheetNumber;

	spritesheetNumber = 1;
	initBackground = glm::vec2(0.0, 0.0);
	sizeBackground = glm::vec2(18.0, 8.0);

	prepareBackground(spritesheetNumber, initBackground, sizeBackground);

	spritesheetNumber = 2;
	initBackground = glm::vec2(0.0, 0.0);
	sizeBackground = glm::vec2(36.0, 8.0);

	prepareBackground(spritesheetNumber, initBackground, sizeBackground);

	spritesheetNumber = 1;
	initBackground = glm::vec2(18.0, 0.0);
	sizeBackground = glm::vec2(18.0, 8.0);

	prepareBackground(spritesheetNumber, initBackground, sizeBackground);

    return true;
}

void Background::prepareBackground(int backgroundFile, const glm::vec2 &initBackground, const glm::vec2 &sizeBackground) {
	Texture *spritesheet = TextureManager::getInstance()->getSpriteSheet((TextureManager::Textures)backgroundFile);
	vector<Sprite*> background;

	for (int i = 0; i < sizeBackground.x; ++i) {
		Sprite *sprite = Sprite::createSprite(glm::ivec2(32, 32 * sizeBackground.y), glm::vec2(1 / 45.0, 1.0), spritesheet, projection);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 0);
		sprite->addKeyframe(0, glm::vec2((1 / 45.0) * (i + initBackground.x), initBackground.y));
		sprite->changeAnimation(0, false);

		sprite->setPosition(glm::vec2(32.0f * i, 0.0));

		background.push_back(sprite);
	}

	backgrounds.push_back(background);
}