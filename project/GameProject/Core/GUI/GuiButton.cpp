#include "GuiButton.hpp"

GuiButton::GuiButton() {
	x = y = width = height = 0;
	text = "";
}

GuiButton::GuiButton(int _x, int _y, int _width, int _height, std::string _text) {
	x = _x;
	y = _y;
	width = _width;
	height = _height;
	text = _text;
}

GuiButton::GuiButton(Rect bounds, std::string _text) {
	x = (int)bounds.x;
	y = (int)bounds.y;
	width = (int)bounds.width;
	height = (int)bounds.height;
	text = _text;
}

bool GuiButton::isPointInside(int px, int py) {
	
	int w = width / 2;
	int h = height / 2;

	int cx = x + w;
	int cy = y + h;

	if ( abs(px - cx) < w && abs(py - cy) < h ) {
		printf("%s, (%d, %d)\n", text.c_str(), px, py);
		state = GuiButtonState::eButton_hover;
		return true;
	}
	return false;

}

bool GuiButton::isPointInside(Point p) {
	return isPointInside(p.x, p.y);
}

void GuiButton::setState(GuiButtonState btnstate) {
	state = btnstate;
}

GuiButtonState GuiButton::getState() {
	return state;
}

Rect GuiButton::getBounds() const {
	Rect r = { (float)x, (float)y, (float)width, (float)height };
	return r;
}
