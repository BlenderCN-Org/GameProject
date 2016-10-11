#ifndef ITEXT_HPP
#define ITEXT_HPP

#include "IFont.hpp"

#include <string>

//#include <GL\glew.h>
//#include <GL\GL.h>

#include <glm\glm.hpp>


class IText {

public:

	virtual void init() = 0;
	virtual void release() = 0;

	virtual void setText(char* text, size_t length, float x, float y, float scale) = 0;

	virtual void setFont(IFont* fnt) = 0;

	virtual void render(glm::vec3 color) = 0;

};


#endif