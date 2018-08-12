
/// Internal Includes
#include "MainMenu.hpp"

#include "../Engine/Input/Input.hpp"

/// External Includes

/// Std Includes

MainMenu::MainMenu(Engine::Graphics::CGui* gui) : pGui(gui) {

	guiInfo.pAssetManager = gui->getAssetManager();

	buttonHoverTexture = new Engine::Graphics::Texture::Texture2D();
	buttonHoverTexture->singleColor(0.5F, 0.0F, 0.0F, 1.0F);

	buttonPressTexture = new Engine::Graphics::Texture::Texture2D();
	buttonPressTexture->singleColor(0.75F, 0.0F, 0.0F, 1.0F);

	panelTexture = new Engine::Graphics::Texture::Texture2D();
	panelTexture->singleColor(0.5F, 0.5F, 0.5F, 0.5F);

	menuPanel = new Engine::Graphics::Gui::Panel(guiInfo);
	menuPanel->setPosition(10, 0);
	menuPanel->setSize(200, 300);
	menuPanel->setAnchorPoint(Engine::Graphics::GuiAnchor::LEFT);
	menuPanel->setVisible(true);
	menuPanel->setTexture(panelTexture);

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
	newGameButton->setTexture(panelTexture);
	newGameButton->setText("New Game");
	newGameButton->setHoverTexture(buttonHoverTexture);
	newGameButton->setPressTexture(buttonPressTexture);

	editorButton = new Engine::Graphics::Gui::Button(guiInfo);
	editorButton->setSize(190, 30);
	editorButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	editorButton->setPosition(0, 70);
	editorButton->setVisible(true);
	editorButton->setTexture(panelTexture);
	editorButton->setText("Editor");
	editorButton->setHoverTexture(buttonHoverTexture);
	editorButton->setPressTexture(buttonPressTexture);

	exitButton = new Engine::Graphics::Gui::Button(guiInfo);
	exitButton->setSize(190, 30);
	exitButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	exitButton->setPosition(0, 110);
	exitButton->setVisible(true);
	exitButton->setTexture(panelTexture);
	exitButton->setText("Exit");
	exitButton->setHoverTexture(buttonHoverTexture);
	exitButton->setPressTexture(buttonPressTexture);

	asteroids = new Engine::Graphics::Gui::Button(guiInfo);
	asteroids->setSize(190, 30);
	asteroids->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	asteroids->setPosition(0, 150);
	asteroids->setVisible(true);
	asteroids->setTexture(panelTexture);
	asteroids->setText("Asteroids");
	asteroids->setHoverTexture(buttonHoverTexture);
	asteroids->setPressTexture(buttonPressTexture);

	gui->addGuiItem(menuPanel);
	menuPanel->addGuiItem(menuLabel);
	menuPanel->addGuiItem(newGameButton);
	menuPanel->addGuiItem(editorButton);
	menuPanel->addGuiItem(exitButton);
	menuPanel->addGuiItem(asteroids);
}

MainMenu::~MainMenu() {

	pGui->removeGuiItem(menuPanel);

	delete panelTexture;
	delete buttonHoverTexture;
	delete buttonPressTexture;
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
