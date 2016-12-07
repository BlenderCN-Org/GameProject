#include "Gui.hpp"

void Gui::setWindowRes(int width, int height) {
	winX = (float)width;
	winY = (float)height;
}

bool Gui::isPressed(GuiButton & button, Input* input, KeyBind mouseButton) {
	int x, y;

	input->getMousePos(x, y);

	float xx = (float)x, yy = (float)y;

	float scaleX = guiResX / winX;
	float scaleY = guiResY / winY;

	xx *= scaleX;
	yy *= scaleY;

	if ( button.isPointInside((int)xx, (int)yy) && input->isKeyBindPressed(mouseButton, false) ) {
		button.setState(GuiButtonState::eButton_pressing);
	}

	if ( button.isPointInside((int)xx, (int)yy) && button.getState() == GuiButtonState::eButton_pressing && input->releasedThisFrame(mouseButton, false) ) {
		return true;
	}

	return false;
}
