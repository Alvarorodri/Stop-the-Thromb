#include "ui\UI_Contador.h"
#include "GeneralDefines.h"

UI_Contador::UI_Contador() {
	text = Text();

    textAlignment = Text::Left;
    contadorState = UI_Contador::Stopped;
    color = glm::vec4(1,1,1,1);
}

void UI_Contador::init(int id, const glm::vec2& pos, const string& _buttonText, const int& size, void* context) {
    posButton = pos;
    fontSize = size;
    currentFontSize = fontSize;
    buttonText = _buttonText;
    buttonContext = context;
    buttonId = id;

    if (!text.init("fonts/OpenSans-Bold.ttf"))
        cout << "Could not load font!!!" << endl;
}

void UI_Contador::update(int deltaTime) {
    if (contadorState == ContadorStates::Started) {
        timeRemaining -= deltaTime;
    }

    if (timeRemaining <= -2) {
        contadorState = ContadorStates::Stopped;
        timeRemaining = initTime;
    }

    settingText();
}

void UI_Contador::render() {
    text.render(buttonText, glm::vec2(posButton.x, posButton.y), currentFontSize, color, textAlignment);
}

void UI_Contador::setPosition(const glm::vec2& _pos) {
    posButton = _pos;
}

void UI_Contador::setColor(const glm::vec3& _color) {
    color = glm::vec4(_color.x, _color.y, _color.z, 1);
}

void UI_Contador::setSize(const int& _size) {
    fontSize = _size;
}

void UI_Contador::setAlignment(const Text::textAlignment _align) {
    textAlignment = _align;
}

void UI_Contador::setState(const UI_Contador::ContadorStates _state) {
    contadorState = _state;
}

void UI_Contador::setTime(int time) {
    initTime = time;
    timeRemaining = time;
}

int UI_Contador::getTime() {
    return timeRemaining;
}

void UI_Contador::settingText() {
    int seconds = timeRemaining / 1000;
    int minutes = seconds / 60;
    seconds %= 60;

    buttonText = "Timer: ";
    buttonText = buttonText + ((minutes < 10) ? "0" : "") + std::to_string(minutes) + ":" + ((seconds < 10) ? "0" : "") + std::to_string(seconds);
}
