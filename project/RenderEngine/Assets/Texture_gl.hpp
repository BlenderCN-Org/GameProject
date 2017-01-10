#ifndef TEXTURE_GL_HPP
#define TEXTURE_GL_HPP

#include "../ITexture.hpp"

#include <GL\glew.h>
#include <GL\GL.h>

#include <mutex>

struct UpdateInfo
{
	void* textureData;
	int w, h;
	int component;
	int layer;
};

class Texture_gl : public ITexture {
public:

	virtual void init(int internalComponents, bool compressed);
	virtual void release();

	virtual void setCompressed(bool compression);

	virtual void setTextureData(int width, int height, int components, void* data);
	virtual void bind();

private:

	GLuint textureID;

	int width;
	int height;
	int components;

	int internalComponents;
	int internalFormat;
	bool compressTexture;
	bool update;
	bool compressChange;

	void* textureData;
	UpdateInfo* updateInfo;
};

#endif