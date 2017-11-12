#ifndef TEXT_HPP
#define TEXT_HPP

#include <RenderEngine/IRenderEngine.hpp>

class Text {
public:

	Text();
	virtual ~Text();

	void setText(const char * text, size_t length, float x, float y, float scale);

	void render(IShaderObject* shader, uint32_t textureLocation);

private:

	IMesh* textMesh;
	IReFont* font;
};

#endif