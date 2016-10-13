#include "Text.hpp"

#include "Font.hpp"
#include "../Utils/FrameAllocator_static.hpp"

#include "../Shader.h"

#include <glm\gtc\matrix_transform.hpp>


void Text::init() {

	std::string sha[] = { "Shaders/Text_vs.glsl" , "Shaders/Text_fs.glsl" };
	GLenum types[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

	CreateProgram(textShader, sha, types, 2);

	projection = glGetUniformLocation(textShader, "projection");

	colorUni = glGetUniformLocation(textShader, "textColor");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Text::release() {
	glDeleteProgram(textShader);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	delete this;
}

void Text::setText(char * text, size_t length, GLfloat x, GLfloat y, GLfloat scale ) {

	FrameAllocator* allocator = FrameAllocator_static::getFrameAllocator();
	Font* f = (Font*)font;

	const GLsizei sizePerChar = 4 * 6;

	GLfloat* verts = allocator->allocate<GLfloat>(length * sizePerChar); // 6 verts with 4 elements per character

	GLfloat dx = x;
	GLfloat dy = y;

	GLfloat yAdv = 0;
	GLfloat oldYAdv = 0;

	//GLfloat xpos;
	//GLfloat ypos;
	//GLfloat h;
	//GLfloat w;

	//GLfloat xpos = x + ch.Bearing.x * scale;
	//GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	for (size_t i = 0; i < length; i++)
	{
		Font::Character ch = f->characters[text[i]];

		if ( i == 0 )
			y += f->getFontSize();

		if (text[i] == '\n')
		{
			if (yAdv)
			{
				y += f->getFontSize();
				oldYAdv = f->getFontSize();
			} else {
				y += oldYAdv;
			}
			yAdv = 0;
			x = dx;
			memset(&verts[i*sizePerChar], 0, sizeof(GLfloat) * sizePerChar);
			continue;
		}
		else if ( text[i] == '\t' )
		{
			x += (ch.size.x * scale * 2.0f);
			memset(&verts[i*sizePerChar], 0, sizeof(GLfloat) * sizePerChar);
			continue;
		}
		else if ( text[i] == '\0' )
		{
			memset(&verts[i*sizePerChar], 0, sizeof(GLfloat) * sizePerChar);
			continue;
		}

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y + ((ch.size.y - ch.bearing.y) * scale);

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		yAdv = glm::max(yAdv, h);

		// Update VBO for each character
		float texX1 = (float)ch.texturePos.x / (float)f->texWidth;
		float texY1 = (float)ch.texturePos.y / (float)f->texHeight;

		float texX2 = (float)(ch.texturePos.x + ch.size.x) / (float)f->texWidth;
		float texY2 = (float)(ch.texturePos.y + ch.size.y) / (float)f->texHeight;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos - h,   texX1, texY1 },
			{ xpos,     ypos,       texX1, texY2 },
			{ xpos + w, ypos,       texX2, texY2 },

			{ xpos,     ypos - h,   texX1, texY1 },
			{ xpos + w, ypos,       texX2, texY2 },
			{ xpos + w, ypos - h,   texX2, texY1 }
		};

		memcpy(&verts[i * sizePerChar], vertices, sizeof(vertices));

		x += (ch.advance >> 6) * scale;
	}
	glBufferData(GL_ARRAY_BUFFER, length * sizePerChar * sizeof(GLfloat), verts, GL_DYNAMIC_DRAW);

	vertCount = (GLsizei)length * sizePerChar;

}

void Text::setFont(IFont * fnt) {
	font = fnt;
}

void Text::render(glm::vec3 color) {
	
	Font* f = (Font*)font;

	glUseProgram(textShader);

	glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f);

	glUniformMatrix4fv(projection, 1, GL_FALSE, &proj[0][0]);
	glUniform3f(colorUni, color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	//// Iterate through all characters
	//std::string::const_iterator c;
	//for (c = text.begin(); c != text.end(); c++)
	//{
	//	Font::Character ch = f->characters[*c];
	//
	//	GLfloat xpos = x + ch.bearing.x * scale;
	//	GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;
	//
	//	GLfloat w = ch.size.x * scale;
	//	GLfloat h = ch.size.y * scale;
	//	// Update VBO for each character
	//	float texX1 = (float)ch.texturePos.x / (float)f->texWidth;
	//	float texY1 = (float)ch.texturePos.y / (float)f->texHeight;
	//
	//	float texX2 = (float)(ch.texturePos.x + ch.size.x) / (float)f->texWidth;
	//	float texY2 = (float)(ch.texturePos.y + ch.size.y) / (float)f->texHeight;
	//
	//	GLfloat vertices[6][4] = {
	//		{ xpos,     ypos + h,   texX1, texY1 },
	//		{ xpos,     ypos,       texX1, texY2 },
	//		{ xpos + w, ypos,       texX2, texY2 },
	//
	//		{ xpos,     ypos + h,   texX1, texY1 },
	//		{ xpos + w, ypos,       texX2, texY2 },
	//		{ xpos + w, ypos + h,   texX2, texY1 }
	//	};
	//	// Render glyph texture over quad
	//	glBindTexture(GL_TEXTURE_2D, f->fontTexture);
	//	// Update content of VBO memory
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	// Render quad
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//	// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
	//	x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	//}
	//glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, f->fontTexture);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glDrawArrays(GL_TRIANGLES, 0, vertCount);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);


}
