#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

/// Internal Includes
#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"
#include "../Engine/Graphics/Gui/Button.hpp"

/// External Includes

/// Std Includes

class MainMenu {

public:

	MainMenu(Engine::Graphics::CGui* gui);
	virtual ~MainMenu();

	void update(float dt);

	int buttonPressed();

private:

	Engine::Graphics::CGui* pGui;
	Engine::Graphics::Gui::Panel* metrixPanel;
	Engine::Graphics::Gui::Label* infoLabel;
	Engine::Graphics::Texture::Texture2D* panelTexture;
	Engine::Graphics::Texture::Texture2D* buttonHoverTexture;
	Engine::Graphics::Texture::Texture2D* buttonPressTexture;
	Engine::Graphics::Gui::Button* newGameButton;
	Engine::Graphics::Gui::Button* editorButton;
	Engine::Graphics::Gui::Button* exitButton;
	Engine::Graphics::Gui::Button* asteroids;

};

#endif