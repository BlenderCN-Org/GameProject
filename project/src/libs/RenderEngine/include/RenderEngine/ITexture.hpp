#ifndef ITEXTURE_HPP
#define ITEXTURE_HPP

class ITexture {
public:

	virtual void init(int internalComponents, bool compressed) = 0;
	virtual void release() = 0;

	virtual void getTextureSize(unsigned int& width, unsigned int& height) = 0;

	virtual void setCompressed(bool compression) = 0;

	virtual void setTextureData(int width, int height, int components, void* data) = 0;
	virtual void bind() = 0;
};

#endif