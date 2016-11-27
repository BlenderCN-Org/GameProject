#include "TextArea.hpp"

TextArea::TextArea(IRenderEngine* re)
{
	renderEngine = re;
	areaQuad = re->createMesh();
	textFont = re->createFont();

	areaQuad->init(MeshPrimitiveType::TRIANGLE_STRIP);
	textFont->init("C:/Windows/Fonts/Arial.ttf", 12);

	pos = glm::vec2(0);
	size = glm::vec2(100, 200);

	vert v[] = { vert{pos.x + size.x, pos.y + size.y, 0.0f, 1.0f, 1.0f},
		vert{ pos.x + size.x, pos.y, 0.0f, 1.0f, 0.0f },
		vert{ pos.x, pos.y + size.y, 0.0f, 0.0f, 1.0f },
		vert{ pos.x, pos.y, 0.0f, 0.0f, 0.0f }, };

	areaQuad->setMeshData(v, sizeof(v), MeshDataLayout::VERT_UV);
	mapped = areaQuad->map(mappedSize);

}

TextArea::~TextArea()
{
	areaQuad->unmap();

	textFont->release();
	areaQuad->release();
}

void TextArea::setPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
	update();
}

void TextArea::setSize(float w, float h)
{
	size.x = w;
	size.y = h;
	update();
}

void TextArea::clearText()
{
	tempString = "";
}

void TextArea::addText(const char * text)
{
	tempString += std::string(text);
	update();
}

void TextArea::insertCharacter(char c)
{
}

void TextArea::render()
{

	renderEngine->stencilMask(0x80);
	renderEngine->stencilFunc(0x0207, 0xFF, 0xFF);
	renderEngine->stencilOp(0x1E00, 0x1E00, 0x1E01);

	areaQuad->bind();
	areaQuad->render();

	renderEngine->stencilFunc(0x0202, 0x80, 0x80);
	renderEngine->stencilOp(0x1E00, 0x1E00, 0x1E00);

	renderEngine->stencilClear(0);
	renderEngine->stencilMask(0x80);
	renderEngine->clearStencil();

	areaQuad->bind();
	areaQuad->render();
}

void TextArea::renderHelpLines()
{

}

void TextArea::update()
{
	vert* v = (vert*)mapped;

	v[0].x = pos.x + size.x;
	v[0].y = pos.y + size.y;

	v[1].x = pos.x + size.x;
	v[1].y = pos.y;

	v[2].x = pos.x;
	v[2].y = pos.y + size.y;

	v[3].x = pos.x;
	v[3].y = pos.y;

	areaQuad->flush();

}
