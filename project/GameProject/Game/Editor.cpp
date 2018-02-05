#include "Editor.hpp"
#include "../Engine/Input/Input.hpp"

Editor::Editor() : mouseDownInGui(false) {

	editorGui = new Engine::Graphics::CGui();

	toolbarTexture = new Engine::Graphics::Texture::Texture2D();
	toolbarTexture->singleColor(0.5F, 0.5F, 0.5F, 0.5F);

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

	editorGui->addGuiItem(toolbar);
	editorGui->addGuiItem(textArea);
	editorGui->setVisible(true);
}

Editor::~Editor() {

	delete textArea;
	delete editorGui;
	delete toolbar;
	delete toolbarTexture;

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

	Input::Input* in = Input::Input::GetInput();
	if (in->isKeyBindPressed(Input::KeyBindings[Input::KEYBIND_MOUSE_L_CLICK]) == false) {
		mouseDownInGui = false;
		pressing = false;
	}
	if (in->wasPressedThisFrame(Input::KeyBindings[Input::KEYBIND_MOUSE_L_CLICK])) {
		if (inGui) {
			mouseDownInGui = true;
		}
		pressing = true;
	}

	return inGui;
}

void Editor::update(float dt) {
	editorGui->update(dt);
}

Renderable** Editor::renderObjects(uint32_t count) {
	count = 1;
	return (Renderable**)&editorGui;
}