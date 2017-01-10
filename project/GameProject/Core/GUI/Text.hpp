#ifndef TEXT_HPP
#define TEXT_HPP

#include <RenderEngine\IRenderEngine.hpp>

class Text
{
public:

	void init(IRenderEngine* re);
	void release();

	void setFont(IFont* fnt);
	void setText(char* text, size_t length, float x, float y, float scale);

	void render(IShaderObject* shader, uint32_t textureLocation);

private:

	IFont* font;
	IMesh* mesh;
};

#endif