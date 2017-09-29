#ifndef GUI_HPP
#define GUI_HPP

#include <RenderEngine/IRenderEngine.hpp>

class Gui {
public:

	void init();
	void freeResources();

	IReFont* getDefaultFont();
	IShaderObject* getDefaultTextShader();
	
	// lazy testing
	IShaderObject* pointQuadShader;
	IMesh* defaultQuad;

private:

	IReFont* font;
	IShaderObject* textShader;
	
};

#endif