#ifndef TEXTAREA_HPP
#define TEXTAREA_HPP

#include <stdint.h>
#include <glm\vec2.hpp>

#include <RenderEngine\IRenderEngine.hpp>

class TextArea
{
public:

	TextArea(IRenderEngine* re);
	virtual ~TextArea();
	
	void setPos(float x, float y);
	void setSize(float w, float h);

	void clearText();
	void addText(const char* text);
	void insertCharacter(char c);

	void render();
	void renderHelpLines();

private:

	void update();

	void* mapped;
	size_t mappedSize;

	char* text;
	uint64_t caretPosition;
	uint64_t textLimit;

	glm::vec2 pos;
	glm::vec2 size;

	IMesh* areaQuad;
	IText* textObject;
	IFont* textFont;

	struct vert
	{
		float x, y, z, u, v;

	};

};

#endif