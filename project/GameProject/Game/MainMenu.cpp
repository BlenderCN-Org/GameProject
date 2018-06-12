
/// Internal Includes
#include "MainMenu.hpp"

/// External Includes

/// Std Includes

MainMenu::MainMenu() : mainMenu(nullptr) {

	mainMenu = new Engine::Graphics::CGui();
	mainMenu->setVisible(true);

	buttonHoverTexture = new Engine::Graphics::Texture::Texture2D();
	buttonHoverTexture->singleColor(0.5F, 0.0F, 0.0F, 1.0F);

	buttonPressTexture = new Engine::Graphics::Texture::Texture2D();
	buttonPressTexture->singleColor(0.75F, 0.0F, 0.0F, 1.0F);

	panelTexture = new Engine::Graphics::Texture::Texture2D();
	panelTexture->singleColor(0.5F, 0.5F, 0.5F, 0.5F);

	metrixPanel = new Engine::Graphics::Gui::Panel();
	metrixPanel->setPosition(10, 0);
	metrixPanel->setSize(200, 300);
	metrixPanel->setAnchorPoint(Engine::Graphics::GuiAnchor::LEFT);
	metrixPanel->setVisible(true);
	metrixPanel->setTexture(panelTexture);

	infoLabel = new Engine::Graphics::Gui::Label();
	infoLabel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	infoLabel->setVisible(true);
	infoLabel->setText("Main Menu");

	infoLabel->setSize(infoLabel->calcTextWidth(), 100);

	newGameButton = new Engine::Graphics::Gui::Button();
	newGameButton->setSize(190, 30);
	newGameButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	newGameButton->setPosition(0, 30);
	newGameButton->setVisible(true);
	newGameButton->setTexture(panelTexture);
	newGameButton->setText("New Game");
	newGameButton->setHoverTexture(buttonHoverTexture);
	newGameButton->setPressTexture(buttonPressTexture);

	editorButton = new Engine::Graphics::Gui::Button();
	editorButton->setSize(190, 30);
	editorButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	editorButton->setPosition(0, 70);
	editorButton->setVisible(true);
	editorButton->setTexture(panelTexture);
	editorButton->setText("Editor");
	editorButton->setHoverTexture(buttonHoverTexture);
	editorButton->setPressTexture(buttonPressTexture);

	exitButton = new Engine::Graphics::Gui::Button();
	exitButton->setSize(190, 30);
	exitButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	exitButton->setPosition(0, 110);
	exitButton->setVisible(true);
	exitButton->setTexture(panelTexture);
	exitButton->setText("Exit");
	exitButton->setHoverTexture(buttonHoverTexture);
	exitButton->setPressTexture(buttonPressTexture);

	asteroids = new Engine::Graphics::Gui::Button();
	asteroids->setSize(190, 30);
	asteroids->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	asteroids->setPosition(0, 150);
	asteroids->setVisible(true);
	asteroids->setTexture(panelTexture);
	asteroids->setText("Asteroids");
	asteroids->setHoverTexture(buttonHoverTexture);
	asteroids->setPressTexture(buttonPressTexture);


	mainMenu->addGuiItem(metrixPanel);
	metrixPanel->addGuiItem(infoLabel);
	metrixPanel->addGuiItem(newGameButton);
	metrixPanel->addGuiItem(editorButton);
	metrixPanel->addGuiItem(exitButton);
	metrixPanel->addGuiItem(asteroids);
}

MainMenu::~MainMenu() {
	delete mainMenu;
	delete panelTexture;
	delete buttonHoverTexture;
	delete buttonPressTexture;
	delete metrixPanel;
	delete infoLabel;
	delete newGameButton;
	delete editorButton;
	delete exitButton;
	delete asteroids;
}

void MainMenu::update(float dt) {
	mainMenu->update(dt);

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

Renderable* MainMenu::getRenderable() {
	return mainMenu;
}