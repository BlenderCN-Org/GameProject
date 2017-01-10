#ifndef FONT_GL_HPP
#define FONT_GL_HPP

#include "../IFont.hpp"

#include <freetype\ft2build.h>
#include FT_FREETYPE_H

#include <GL\glew.h>
#include <GL\GL.h>

#include <glm\glm.hpp>

#include <map>

class Font_gl : public IFont {
public:

	Font_gl(FT_Library &fontLib);

	virtual void init(char* font, int size);
	virtual void release();

	virtual void setFontSize(int size);
	virtual void setFont(char* font);

	virtual int getFontSize() const;

	virtual uint32_t getTextureID() const;
	virtual void bindFontTexture() const;

	Character getCharacter(unsigned char character);

	virtual void getTextureInfo(int &x, int &y);

	std::map<GLchar, Character> characters;

	GLuint fontTexture;

	int texWidth;
	int texHeight;

private:

	void genFontTexture();

	FT_Library fontLib;
	FT_Face face;

	int fontSize;
};

#endif