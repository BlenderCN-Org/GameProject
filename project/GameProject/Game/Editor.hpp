#ifndef EDITOR_HPP
#define EDITOR_HPP

/// Internal Includes
#include "Editor/3dViewGizmo.hpp"

#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"
#include "../Engine/Graphics/Gui/GuiWindow.hpp"

/// External Includes

/// Std Includes

class Editor {

public:

	Editor();
	~Editor();

	bool mouseInGui();
	void update(float dt);

	Renderable** renderObjects(uint32_t count);

private:

	bool mouseDownInGui;
	bool pressing;

	uint32_t renderObjectCount;

	Engine::Graphics::CGui* editorGui;

	Engine::Graphics::Texture::Texture2D* buttonHoverTexture;
	Engine::Graphics::Texture::Texture2D* buttonPressTexture;

	Engine::Graphics::Texture::Texture2D* toolbarTexture;
	Engine::Graphics::Gui::Panel* toolbar;
	Engine::Graphics::Gui::Button* showButton;

	Engine::Graphics::Gui::TextArea* textArea;

	Engine::Graphics::Gui::GuiWindow* guiWindow;

};


#endif