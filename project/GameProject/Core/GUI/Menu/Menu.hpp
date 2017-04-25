#ifndef MENU_HPP
#define MENU_HPP

#include "MenuItem.hpp"
#include "../Text.hpp"

class Menu {
	
	friend class AssetManager;

public:

	Menu();
	~Menu();

	void showMenu(bool visible);
	
	void update();
	void render();

private:

	bool menuVisible = false;

	MenuItem* menuItems = nullptr;
	unsigned int totalMenuItems = 0;
	unsigned int selectedMenuItem = 0;

};

#endif