#ifndef IFONT_HPP
#define IFONT_HPP

#include <glm/glm.hpp>

struct Character {
	unsigned int     charID;  // ID handle of the glyph texture
	glm::ivec2 size;       // Size of glyph
	glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
	glm::ivec2 texturePos;
	unsigned int     advance;    // Offset to advance to next glyph
	unsigned int     advanceY;    // Offset to advance to next glyph
};

class IReFont {
public:

	virtual void init(char* fontName, int size) = 0;
	virtual void release() = 0;

	virtual void setFontSize(int size) = 0;
	virtual void setFont(char* fontName) = 0;

	virtual int getFontSize() const = 0;

	virtual unsigned int getTextureID() const = 0;
	virtual void bindFontTexture() const = 0;

	virtual Character getCharacter(unsigned char character) = 0;

	virtual void getTextureInfo(int &x, int &y) = 0;
};

#endif