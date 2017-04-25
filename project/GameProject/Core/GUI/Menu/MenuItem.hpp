#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include <string>

class MenuItem
{

public:

	MenuItem();
	~MenuItem();

	void render();

private:

	std::string text;

};

#endif