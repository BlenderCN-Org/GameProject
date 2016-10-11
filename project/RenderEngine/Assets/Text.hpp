#ifndef TEXT_HPP
#define TEXT_HPP

#include "../IFont.hpp"
#include "../IText.hpp"

#include <string>

#include <GL\glew.h>
#include <GL\GL.h>

#include <glm\glm.hpp>

class Text : public IText{

public:

	virtual void init();
	virtual void release();

	virtual void setText(char* text, size_t length, GLfloat x, GLfloat y, GLfloat scale);

	virtual void setFont(IFont* fnt);

	virtual void render(glm::vec3 color);

private:

	IFont* font;

	GLuint textShader;

	GLuint VAO, VBO;

	GLsizei vertCount;

	GLuint projection;
	GLuint colorUni;

};

#endif