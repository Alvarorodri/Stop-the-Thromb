#include "ui\UI_Text.h"
#include "scenes\Scene.h"
#include "GeneralDefines.h"

UI_Button::UI_Button() {
	text = Text();

	textAlignment = Text::Center;
	buttonState = UI_Button::Unselected;
	color = glm::vec4(1, 1, 1, 1);
	buttonPressed = false;
}

void UI_Button::init(int id, const glm::vec2 &pos, const string &_buttonText, const int &size, void *context) {
	posButton = pos;
	fontSize = size;
	currentFontSize = fontSize;
	buttonText = _buttonText;
	buttonContext = context;
	buttonId = id;

	if (!text.init("fonts/OpenSans-Bold.ttf"))
		cout << "Could not load font!!!" << endl;
}

void UI_Button::update(int deltaTime)
{
	if (buttonPressed) {
		reinterpret_cast<Scene*>(buttonContext)->buttonCallback(buttonId);
		buttonPressed = false;
	}
	if (currentAnimationTime > 0) {
		if (int(currentAnimationTime / (3 * int(TIME_PER_FRAME))) % 2 == 0) setColor(glm::vec3(1, 0, 0));
		else setColor(glm::vec3(1, 1, 0));

		render();
		currentAnimationTime -= deltaTime;
	}
	else if (currentAnimationTime < 0) {
		currentAnimationTime = 0;
		setColor(glm::vec3(1, 1, 1));
		setState(UI_Button::Unselected);
		render();
		buttonPressed = true;
	}
}

void UI_Button::render() {
	text.render(buttonText, glm::vec2(posButton.x, posButton.y), currentFontSize, color, textAlignment);
}

void UI_Button::setPosition(const glm::vec2 &_pos) {
	posButton = _pos;
}

void UI_Button::setColor(const glm::vec3 &_color) {
	color = glm::vec4(_color.x, _color.y, _color.z, 1);
}

void UI_Button::setSize(const int &_size) {
	fontSize = _size;

	if (buttonState == UI_Button::Unselected) currentFontSize = fontSize;
	else currentFontSize = (int)(fontSize * 1.3f);
}

void UI_Button::setAlignment(const Text::textAlignment _align) {
	textAlignment = _align;
}

void UI_Button::setState(const UI_Button::ButtonState _state) {
	buttonState = _state;


	if (buttonState != UI_Button::Unselected) currentFontSize = (int)(fontSize * 1.3f);
	else currentFontSize = fontSize;

	if (buttonState == UI_Button::Clicked) currentAnimationTime = animationTime;
}
