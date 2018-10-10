#ifndef GUI_HPP
#define GUI_HPP

#include <RenderEngine/IRenderEngine.hpp>
#include "../Camera.hpp"

class Gui {
public:

	void init();
	void freeResources();

	IReFont* getDefaultFont();
	IShaderObject* getDefaultTextShader();
	
	// lazy testing
	IShaderObject* pointQuadShader;
	IMesh* defaultQuad;

	Camera* cam;

private:

	IReFont* font;
	IShaderObject* textShader;

};

#endif