#ifndef MATERIAL_GL_HPP
#define MATERIAL_GL_HPP

#include "Texture_gl.hpp"

class Material_gl {
public:

	virtual void init(unsigned int nrTextures);
	virtual void release();

	virtual void setTexture(unsigned int textureIndex, ITexture* texture);
	virtual void useMaterial();

	virtual int getTextureCount() const;
	virtual int getTextureBindLocation(unsigned int textureIndex) const;

private:

	unsigned int numberOfTextures;
	ITexture** textures;

	int* bindLocations;
};

#endif