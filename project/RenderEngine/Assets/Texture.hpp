#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../ITexture.hpp"

#include <GL\glew.h>
#include <GL\GL.h>

class Texture : public ITexture{

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

};

#endif