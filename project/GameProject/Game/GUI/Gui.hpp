#ifndef GUI_HPP
#define GUI_HPP

#include <RenderEngine/IRenderEngine.hpp>

class Gui {
public:

	void init();
	void freeResources();

	IReFont* getDefaultFont();
	IShaderObject* getDefaultTextShader();

private:

	IReFont* font;
	IShaderObject* textShader;
};

#endif