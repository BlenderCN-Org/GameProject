#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"
#include "../Engine/Graphics/Gui/Button.hpp"

class MainMenu {

public:

	MainMenu();
	virtual ~MainMenu();

	void update(float dt);

	int buttonPressed();

	Renderable* getRenderable();

private:

	Engine::Graphics::CGui* mainMenu;
	Engine::Graphics::Gui::Panel* metrixPanel;
	Engine::Graphics::Gui::Label* infoLabel;
	Engine::Graphics::Texture::Texture2D* panelTexture;
	Engine::Graphics::Texture::Texture2D* buttonHoverTexture;
	Engine::Graphics::Texture::Texture2D* buttonPressTexture;
	Engine::Graphics::Gui::Button* newGameButton;
	Engine::Graphics::Gui::Button* editorButton;
	Engine::Graphics::Gui::Button* exitButton;

};

#endif