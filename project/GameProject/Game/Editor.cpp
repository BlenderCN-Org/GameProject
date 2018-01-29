#include "Editor.hpp"

Editor::Editor() {

	editorGui = new Engine::Graphics::CGui();

	toolbarTexture = new Engine::Graphics::Texture::Texture2D();
	toolbarTexture->singleColor(0.5F, 0.5F, 0.5F, 0.5F);

	toolbar = new Engine::Graphics::Gui::Panel();
	toolbar->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	toolbar->setPosition(10, 10);
	toolbar->setSize(300, 100);
	toolbar->setTexture(toolbarTexture);
	toolbar->setVisible(true);

	editorGui->addGuiItem(toolbar);
	editorGui->setVisible(true);
}

Editor::~Editor() {

	delete editorGui;
	delete toolbar;
	delete toolbarTexture;

}

void Editor::update(float dt) {
	editorGui->update(dt);
}

Renderable** Editor::renderObjects(uint32_t count) {
	count = 1;
	return (Renderable**)&editorGui;
}