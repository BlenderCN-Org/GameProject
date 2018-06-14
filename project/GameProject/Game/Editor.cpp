
/// Internal Includes
#include "Editor.hpp"
#include "../Engine/Input/Input.hpp"

/// External Includes

/// Std Includes

Editor::Editor()
	: mouseDownInGui(false)
	, pGameEditAccess(nullptr) {

	editorGui = new Engine::Graphics::CGui();

	toolbarTexture = new Engine::Graphics::Texture::Texture2D();
	toolbarTexture->singleColor(0.5F, 0.5F, 0.5F, 0.5F);

	buttonHoverTexture = new Engine::Graphics::Texture::Texture2D();
	buttonHoverTexture->singleColor(0.5F, 0.0F, 0.0F, 1.0F);

	buttonPressTexture = new Engine::Graphics::Texture::Texture2D();
	buttonPressTexture->singleColor(0.75F, 0.0F, 0.0F, 1.0F);

	toolbar = new Engine::Graphics::Gui::Panel();
	toolbar->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	toolbar->setPosition(10, 10);
	toolbar->setSize(300, 100);
	toolbar->setTexture(toolbarTexture);
	toolbar->setVisible(true);

	textArea = new Engine::Graphics::Gui::TextArea();
	textArea->setVisible(true);
	textArea->setAnchorPoint(Engine::Graphics::GuiAnchor::LEFT);
	textArea->setPosition(0, 0);
	textArea->setSize(200, 400);
	textArea->setText("This is a string");
	textArea->showLineNumbers(false);
	textArea->setEditMode(true);

	showButton = new Engine::Graphics::Gui::Button();
	showButton->setVisible(true);
	showButton->setAnchorPoint(Engine::Graphics::GuiAnchor::LEFT);
	showButton->setPosition(0, 0);
	showButton->setSize(50, 50);
	showButton->setText("");
	showButton->setTexture(toolbarTexture);
	showButton->setHoverTexture(buttonHoverTexture);
	showButton->setPressTexture(buttonPressTexture);

	toolbar->addGuiItem(showButton);

	guiWindow = new Engine::Graphics::Gui::GuiWindow();
	guiWindow->setSize(400, 400);
	guiWindow->setPosition(0, 0);
	guiWindow->addGuiItem(textArea);

	editorGui->addGuiItem(toolbar);
	editorGui->addGuiItem(guiWindow);
	editorGui->setVisible(true);

}

Editor::~Editor() {

	if (nullptr != pGameEditAccess) {
		delete pGameEditAccess;
	}

	delete textArea;
	delete editorGui;
	delete toolbar;
	delete toolbarTexture;

	delete showButton;
	delete buttonHoverTexture;
	delete buttonPressTexture;

	delete guiWindow;

}

void Editor::start(IEdit* pEditClass, IMap** ppMap) {

	if (nullptr != pGameEditAccess) {
		delete pGameEditAccess;
	}
	pGameEditAccess = pEditClass;
	ppActiveMap = ppMap;

}

bool Editor::mouseInGui() {

	bool inGui = false;

	if (!pressing) {
		inGui = editorGui->mouseHitGui();
	}

	// force in gui while pressing down
	// to account for press move outside
	if (mouseDownInGui) {
		inGui = true;
	}

	Engine::Input::Input* in = Engine::Input::Input::GetInput();
	if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_MOUSE_L_CLICK]) == false) {
		mouseDownInGui = false;
		pressing = false;
	}
	if (in->wasPressedThisFrame(Engine::Input::KeyBindings[Engine::Input::KEYBIND_MOUSE_L_CLICK])) {
		if (inGui) {
			mouseDownInGui = true;
		}
		pressing = true;
	}

	if (showButton->wasPressed()) {
		guiWindow->setVisible(true);
	}

	return inGui;
}

void Editor::update(float dt) {
	editorGui->update(dt);

	Engine::Input::Input* in = Engine::Input::Input::GetInput();

	// @ TODO add check for moving around to not tirgger by accedent
	if (in->releasedThisFrame(EditorKeyBinds::editorSave)) {
		pGameEditAccess->save();
		printf("Saving changes\n");
	}

}

Renderable** Editor::renderObjects(uint32_t count) {
	count = 1;
	return (Renderable**)&editorGui;
}