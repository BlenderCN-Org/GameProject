#ifndef GUI_HPP
#define GUI_HPP

// project includes
#include "GuiButton.hpp"

#include "../Input/Input.hpp"

// std includes

class Gui
{

public:

	void setWindowRes(int width, int height);
	bool isPressed(GuiButton &button, Input* input, KeyBind mouseButton);

protected:

	float guiResX = 1920;
	float guiResY = 1080;

	float winX = 0;
	float winY = 0;

};

#endif