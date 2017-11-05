#ifndef TEXT_HPP
#define TEXT_HPP

/// Internal Includes
#include "../../Core/String.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

class Text {

public:
	Text();
	virtual ~Text();

	void setText(Engine::Core::String text);

	void render(int textureSlot);

private:

	IReFont* font;
	IMesh* textMesh;

	int width;
	int height;

};

#endif