#ifndef GUIBUTTON_HPP
#define GUIBUTTON_HPP

// project includes
#include "GuiStructs.hpp"

// std includes
#include <string>

enum class GuiButtonState
{
	eButton_idle = 0,
	eButton_hover = 1,
	eButton_pressing = 2,
};

class GuiButton
{
public:

	GuiButton();

	GuiButton(int x, int y, int width, int height, std::string text);
	GuiButton(Rect bounds, std::string text);

	bool isPointInside(int x, int y);
	bool isPointInside(Point p);

	void setState(GuiButtonState btnstate);
	GuiButtonState getState();

	Rect getBounds() const;

private:

	int x;
	int y;
	int width;
	int height;

	std::string text;

	GuiButtonState state;

};

#endif