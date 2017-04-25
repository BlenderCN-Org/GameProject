#include "Menu.hpp"

#include "../../Input/Input.hpp"
#include "../../Assets/AssetManager.hpp"

Menu::Menu()
{
}

Menu::~Menu()
{
	delete [] menuItems;
}

void Menu::showMenu(bool visible)
{
	menuVisible = visible;
}

void Menu::update()
{
	int fontSize = AssetManager::getAssetManager()->getBasicFont()->getFontSize();
	Input* in = Input::getInput();
	

}

void Menu::render()
{
	if (!menuVisible)
		return;

	for (size_t i = 0; i < totalMenuItems; i++)
	{
		menuItems[i].render();
	}
}