#ifndef MENU_HPP
#define MENU_HPP

#include <cstdint>
#include <vector>

#include "GUI/Text.hpp"

struct MenuItem {
	uint32_t x;
	uint32_t y;

	uint32_t textLength;
	char* text;
};

class Menu {
public:

	Menu();

	uint8_t addItem(uint32_t x, uint32_t y, char* text);
	uint8_t getSelectedMenuItem();

	void setOrthoMatrix(glm::mat4* ortho);

	void selectNext();
	void selectPrev();

	void render();

private:

	uint8_t maxMenuItems;
	uint8_t selectedMenuItem;

	std::vector<MenuItem> menuItems;

	Text textObj;
	int32_t textureLocation;
	int32_t colorLocation;
	int32_t vpLocation;
	int32_t mdlLocation;
	glm::mat4* orthoPersp;
};

#endif