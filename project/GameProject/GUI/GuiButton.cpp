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
	x = bounds.x;
	y = bounds.y;
	width = bounds.width;
	height = bounds.height;
	text = _text;
}

bool GuiButton::isPointInside(int px, int py) {
	
	int w = width / 2;
	int h = height / 2;

	int cx = x + w;
	int cy = y + h;

	if ( abs(px - cx) < w && abs(py - cy) < w )
		return true;
	return false;

}

bool GuiButton::isPointInside(Point p) {
	return isPointInside(p.x, p.y);
}

Rect GuiButton::getBounds() const {
	Rect r = { x, y, width, height };
	return r;
}
