#ifndef GUIBUTTON_HPP
#define GUIBUTTON_HPP

// project includes
#include "GuiStructs.hpp"

// std includes
#include <string>

class GuiButton
{
public:

	GuiButton();

	GuiButton(int x, int y, int width, int height, std::string text);
	GuiButton(Rect bounds, std::string text);

	bool isPointInside(int x, int y);
	bool isPointInside(Point p);

	Rect getBounds() const;

private:

	int x;
	int y;
	int width;
	int height;

	std::string text;

	bool isInside;
	bool isPressing;

};

#endif