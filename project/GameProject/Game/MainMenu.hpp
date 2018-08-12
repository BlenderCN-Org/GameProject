#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

/// Internal Includes
#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"
#include "../Engine/Graphics/Gui/Button.hpp"

/// External Includes

/// Std Includes

enum class MainMenuButtons {

	eButton_newGame,
	eButton_loadGame,
	eButton_continue,
	eButton_editor,
	eButton_options,
	eButton_exit

};

class MainMenu {

public:

	MainMenu(Engine::Graphics::CGui* gui);
	virtual ~MainMenu();

	void update(float dt);

	void setVisible(bool visible);

	int buttonPressed();

private:

	Engine::Graphics::GuiInfo guiInfo;
	Engine::Graphics::CGui* pGui;
	Engine::Graphics::Gui::Panel* menuPanel;
	Engine::Graphics::Gui::Label* menuLabel;
	Engine::Graphics::Texture::Texture2D* panelTexture;
	Engine::Graphics::Texture::Texture2D* buttonHoverTexture;
	Engine::Graphics::Texture::Texture2D* buttonPressTexture;
	Engine::Graphics::Gui::Button* newGameButton;
	Engine::Graphics::Gui::Button* editorButton;
	Engine::Graphics::Gui::Button* exitButton;
	Engine::Graphics::Gui::Button* asteroids;

};

#endif