#ifndef TEXT_HPP
#define TEXT_HPP

/// Internal Includes
#include "../../Core/FormattedString.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

class Text {

public:
	Text();
	virtual ~Text();

	void setText(const Engine::Core::FormattedString& text);

	void render(int textureSlot);

private:

	IReFont* font;
	IMesh* textMesh;

	int width;
	int height;

};

#endif