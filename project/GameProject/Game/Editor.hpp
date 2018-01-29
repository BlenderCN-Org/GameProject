#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"

class Editor {

public:

	Editor();
	~Editor();

	void update(float dt);

	Renderable** renderObjects(uint32_t count);

private:

	uint32_t renderObjectCount;

	Engine::Graphics::CGui* editorGui;


	Engine::Graphics::Texture::Texture2D* toolbarTexture;
	Engine::Graphics::Gui::Panel* toolbar;


};


#endif