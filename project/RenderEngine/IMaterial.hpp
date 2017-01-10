#ifndef IMATERIAL_HPP
#define IMATERIAL_HPP

#include "ITexture.hpp"

class IMaterial
{
public:

	virtual void init(unsigned int nrTextures) = 0;
	virtual void release() = 0;

	virtual void setTexture(unsigned int textureIndex, ITexture* texture) = 0;
	virtual void useMaterial() = 0;

	virtual int getTextureCount() const = 0;
	virtual int getTextureBindLocation(unsigned int textureIndex) const = 0;
};

#endif