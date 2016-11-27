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

	if ( button.isPointInside((int)xx, (int)yy) && input->isKeyBindPressed(mouseButton, false))
		return true;
	return false;
}
