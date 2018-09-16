
/// Internal Includes
#include "MainMenu.hpp"

#include "../Engine/Input/Input.hpp"

/// External Includes

/// Std Includes

MainMenu::MainMenu(Engine::Graphics::CGui* gui) : pGui(gui) {

	guiInfo.pAssetManager = gui->getAssetManager();

	menuPanel = new Engine::Graphics::Gui::Panel(guiInfo);
	menuPanel->setPosition(10, 0);
	menuPanel->setSize(200, 300);
	menuPanel->setAnchorPoint(Engine::Graphics::GuiAnchor::LEFT);
	menuPanel->setVisible(true);

	menuLabel = new Engine::Graphics::Gui::Label(guiInfo);
	menuLabel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	menuLabel->setVisible(true);
	menuLabel->setText("Main Menu");

	menuLabel->setSize(menuLabel->calcTextWidth(), 100);

	newGameButton = new Engine::Graphics::Gui::Button(guiInfo);
	newGameButton->setSize(190, 30);
	newGameButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	newGameButton->setPosition(0, 30);
	newGameButton->setVisible(true);
	newGameButton->setText("New Game");

	editorButton = new Engine::Graphics::Gui::Button(guiInfo);
	editorButton->setSize(190, 30);
	editorButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	editorButton->setPosition(0, 70);
	editorButton->setVisible(true);
	editorButton->setText("Editor");

	exitButton = new Engine::Graphics::Gui::Button(guiInfo);
	exitButton->setSize(190, 30);
	exitButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	exitButton->setPosition(0, 110);
	exitButton->setVisible(true);
	exitButton->setText("Exit");

	asteroids = new Engine::Graphics::Gui::Button(guiInfo);
	asteroids->setSize(190, 30);
	asteroids->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	asteroids->setPosition(0, 150);
	asteroids->setVisible(true);
	asteroids->setText("Asteroids");

	gui->addGuiItem(menuPanel);
	menuPanel->addGuiItem(menuLabel);
	menuPanel->addGuiItem(newGameButton);
	menuPanel->addGuiItem(editorButton);
	menuPanel->addGuiItem(exitButton);
	menuPanel->addGuiItem(asteroids);
}

MainMenu::~MainMenu() {

	pGui->removeGuiItem(menuPanel);
	
	delete menuPanel;
	delete menuLabel;
	delete newGameButton;
	delete editorButton;
	delete exitButton;
	delete asteroids;
}

void MainMenu::update(float dt) {
}

void MainMenu::setVisible(bool visible) {
	menuPanel->setVisible(visible);
}

int MainMenu::buttonPressed() {

	int index = -1;

	if (newGameButton->wasPressed()) {
		printf("New game, YAY!!!!\n");
		index = 0;
	}

	if (editorButton->wasPressed()) {
		printf("Edit, YAY!!!!\n");
		index = 1;
	}

	if (exitButton->wasPressed()) {
		printf("Exit, YAY!!!!\n");
		index = 2;
	}

	if (asteroids->wasPressed()) {
		printf("Asteroids, YAY!!!!\n");
		index = 3;
	}

	return index;
}
