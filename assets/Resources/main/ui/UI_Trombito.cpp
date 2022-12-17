#include "ui\UI_Trombito.h"
#include "GeneralDefines.h"

UI_Trombito::UI_Trombito() {
    text = Text();

    textAlignment = Text::Left;
    color = glm::vec4(1, 1, 1, 1);
}

UI_Trombito::UI_Trombito(glm::mat4 *project) {
    projection = project;
    text = Text();

    textAlignment = Text::Left;
    color = glm::vec4(1, 1, 1, 1);
}

void UI_Trombito::init(int id, const glm::vec2& pos, const string& _buttonText, const int& size, void* context) {
    posButton = pos;
    fontSize = size;
    currentFontSize = fontSize;
    buttonText = _buttonText;
    buttonContext = context;
    buttonId = id;

    if (!text.init("fonts/OpenSans-Bold.ttf"))
        cout << "Could not load font!!!" << endl;

    spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Trombito);

    sprite = Sprite::createSprite(glm::ivec2(200, 30), glm::vec2(1.0f, 1.0f), spritesheet, projection);
    sprite->setNumberAnimations(3);

    sprite->setAnimationSpeed(0, 1);
    sprite->addKeyframe(0, glm::vec2(0, 0));

    sprite->changeAnimation(0, false);

    sprite->setPosition(glm::vec2(0.0f,0.0f));

    settingText();
}

void UI_Trombito::update(int deltaTime) {
    sprite->update(deltaTime);

    if (timeRemaining > 0) {
        timeRemaining -= deltaTime;
    }
    else if (timeRemaining <= 0) {
        state = false;
    }
    settingText();
}

void UI_Trombito::render() {
    if (state) {
        sprite->render();
        text.render(buttonText1, posButton + offset1, currentFontSize, color, textAlignment);
        text.render(buttonText2, posButton + offset2, currentFontSize, color, textAlignment);
        text.render(buttonText3, posButton + offset3, currentFontSize, color, textAlignment);
    }
}

void UI_Trombito::setPosition(const glm::vec2& _pos) {
    posButton = _pos;
}

void UI_Trombito::setColor(const glm::vec3& _color) {
    color = glm::vec4(_color.x, _color.y, _color.z, 1);
}

void UI_Trombito::setSize(const int& _size) {
    fontSize = _size;
}

void UI_Trombito::setAlignment(const Text::textAlignment _align) {
    textAlignment = _align;
}

void UI_Trombito::setTime(int time) {
    initTime = time;
}

void UI_Trombito::settingText() {
    buttonText += "                                                                       ";
    buttonText1 = buttonText.substr(0, 20);
    buttonText2 = buttonText.substr(20, 20);
    buttonText3 = buttonText.substr(40, 20);
}

void UI_Trombito::showTrombito() {
    state = true;
    timeRemaining = initTime;
}
