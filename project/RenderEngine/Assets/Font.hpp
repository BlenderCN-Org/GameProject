#ifndef FONT_HPP
#define FONT_HPP

#include "../IFont.hpp"

#include <freetype\ft2build.h>
#include FT_FREETYPE_H

#include <GL\glew.h>
#include <GL\GL.h>

#include <glm\glm.hpp>

#include <map>

class Font : public IFont{

public:

	Font(FT_Library &fontLib);

	virtual void init(char* font, int size);
	virtual void release();

	virtual void setFontSize(int size);
	virtual void setFont(char* font);
	
	struct Character {
		GLuint     charID;  // ID handle of the glyph texture
		glm::ivec2 size;       // Size of glyph
		glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
		glm::ivec2 texturePos;
		GLuint     advance;    // Offset to advance to next glyph
		GLuint     advanceY;    // Offset to advance to next glyph
	};

	std::map<GLchar, Character> characters;

	GLuint fontTexture;

	int texWidth;
	int texHeight;

private:

	void genFontTexture();

	FT_Library fontLib;
	FT_Face face;


};

#endif