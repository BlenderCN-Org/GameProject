#include "Font.hpp"

#include "../Utils/FrameAllocator_static.hpp"
#include "../DataTypes/Image.hpp"

#include <list>

#include "../Utils/ShelfPack.hpp"

#include <fstream>

Font::Font(FT_Library &_fontLib) {
	fontLib = _fontLib;
}

void Font::init(char * font, int size) {
	
	FT_New_Face(fontLib, font, 0, &face);
	fontSize = size;
	FT_Set_Pixel_Sizes(face, 0, size);

	glGenTextures(1, &fontTexture);

	genFontTexture();
	
}

void Font::release() {
	FT_Done_Face(face);

	glDeleteTextures(1, &fontTexture);
	delete this;
}

void Font::setFontSize(int size) {
	fontSize = size;
	FT_Set_Pixel_Sizes(face, 0, size);
	genFontTexture();
}

void Font::setFont(char * font) {
	if(face)
		FT_Done_Face(face);
	FT_New_Face(fontLib, font, 0, &face);
	genFontTexture();
}

int Font::getFontSize() const
{
	return fontSize;
}

uint32_t Font::getTextureID() const {
	return fontTexture;
}

void Font::bindFontTexture() const {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
}

Character Font::getCharacter(unsigned char character) {
	return characters[character];
}

void Font::getTextureInfo(int & x, int & y) {
	x = texWidth;
	y = texHeight;
}

// comparison, not case sensitive.
bool cmp(const Character& first, const Character& second) {
	
	return (first.size.y > second.size.y);
}

void Font::genFontTexture() {
	
	FrameAllocator* frameAlloc = FrameAllocator_static::getFrameAllocator();
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	unsigned char* imageArray[255];

	std::list<Character> l;
	characters.clear();
	for (GLubyte c = 0; c < 255; c++)
	{
		imageArray[c] = nullptr;
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			//std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		size_t dataSize = face->glyph->bitmap.width * face->glyph->bitmap.rows;
		
		imageArray[c] = frameAlloc->allocate<unsigned char>(dataSize);

		memcpy_s(imageArray[c], dataSize, face->glyph->bitmap.buffer, dataSize);

		Character character = {
			c,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			glm::ivec2(0, 0),
			(GLuint)face->glyph->advance.x,
			(GLuint)face->glyph->advance.y
		};
		l.push_back(character);
		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	l.sort(cmp);

	ShelfPack sp(128, 512);


	for (std::list<Character>::const_iterator iterator = l.begin(), end = l.end(); iterator != end; ++iterator) {
		
		int w = iterator->size.x;
		int h = iterator->size.y;
		
		int id = iterator->charID;

		sp.addData(w+1, h+1, id);
	}
	int w = sp.getPackedWidth();
	int h = sp.getPackedHeight();

	size_t length = sp.length();

	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		w,
		h,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int offsetX = 0;
	int offsetY = 0;

	int width = 0;
	int height = 0;

	GLubyte id = 0;

	for (size_t i = 0; i < length; i++)
	{
		int index = int(i);
		offsetX = sp[index].x;
		offsetY = sp[index].y;
		width = sp[index].w-1;
		height = sp[index].h-1;
		id = sp[index].id;
		glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, GL_RED, GL_UNSIGNED_BYTE, imageArray[id]);

		characters[id].texturePos = glm::ivec2(offsetX, offsetY);
	}
	
	texHeight = h;
	texWidth = w;

	char* data = frameAlloc->allocate<char>(w*h);
	
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	
}