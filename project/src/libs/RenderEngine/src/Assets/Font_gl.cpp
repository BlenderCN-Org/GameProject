#include "Font_gl.hpp"

#include <list>

#include "../Utils/ShelfPack.hpp"

#include <fstream>

Font_gl::Font_gl(FT_Library &_fontLib) {
	fontLib = _fontLib;
}

void Font_gl::init(char * font, int size) {

	fontFile = font;
	fontSize = size;
	
	glGenTextures(1, &fontTexture);

	loadAndGenerateFont();
}

void Font_gl::release() {

	glDeleteTextures(1, &fontTexture);
	delete this;
}

void Font_gl::setFontSize(int size) {
	fontSize = size;
	loadAndGenerateFont();
}

void Font_gl::setFont(char * font) {
	fontFile = font;
	loadAndGenerateFont();
}

int Font_gl::getFontSize() const {
	return fontSize;
}

uint32_t Font_gl::getTextureID() const {
	return fontTexture;
}

void Font_gl::bindFontTexture() const {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
}

Character Font_gl::getCharacter(unsigned char character) {
	return characters[character];
}

void Font_gl::getTextureInfo(int & x, int & y) {
	x = texWidth;
	y = texHeight;
}

// comparison, not case sensitive.
bool cmp(const Character& first, const Character& second) {
	return (first.size.y > second.size.y);
}

void Font_gl::loadAndGenerateFont() {
	FT_New_Face(fontLib, fontFile, 0, &face);
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	
	genFontTexture();
	FT_Done_Face(face);
}

void Font_gl::genFontTexture() {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	unsigned char* imageArray[255];

	std::list<Character> l;
	characters.clear();
	for (GLubyte c = 0; c < 255; c++) {
		imageArray[c] = nullptr;
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			//std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		size_t dataSize = face->glyph->bitmap.width * face->glyph->bitmap.rows;

		imageArray[c] = new unsigned char[dataSize];
		
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

	ShelfPack sp(512, 512);

	for (std::list<Character>::const_iterator iterator = l.begin(), end = l.end(); iterator != end; ++iterator) {
		int w = iterator->size.x;
		int h = iterator->size.y;

		int id = iterator->charID;

		sp.addData(w + 1, h + 1, id);
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

	for (size_t i = 0; i < length; i++) {
		int index = int(i);
		offsetX = sp[index].x;
		offsetY = sp[index].y;
		width = sp[index].w - 1;
		height = sp[index].h - 1;
		id = static_cast<GLbyte>(sp[index].id);
		glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, GL_RED, GL_UNSIGNED_BYTE, imageArray[id]);

		characters[id].texturePos = glm::ivec2(offsetX, offsetY);
	}

	texHeight = h;
	texWidth = w;

	char* data = new char[w*h];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	for (GLubyte c = 0; c < 255; c++) {
		delete imageArray[c];
		imageArray[c] = nullptr;
	}

	//	std::ofstream out;
	//	out.open("asd.data", std::ios::binary);
	//	out.write(data, w*h);
	//	out.close();

	delete[] data;
}