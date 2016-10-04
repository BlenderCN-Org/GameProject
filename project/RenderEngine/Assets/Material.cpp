#include "Material.hpp"
#include "../Utils/MemoryManager.hpp"

void Material::init(unsigned int nrTextures)
{
	numberOfTextures = nrTextures;

	textures = MemoryManager_alloc(ITexture*, numberOfTextures);
	bindLocations = MemoryManager_alloc(int, numberOfTextures);

	for ( unsigned int i = 0; i < numberOfTextures; i++ )
	{
		textures[i] = nullptr;
		bindLocations[i] = 0;
	}
}

void Material::release()
{
	MemoryManager::getMemoryManager()->deallocate(textures);
	MemoryManager::getMemoryManager()->deallocate(bindLocations);

	delete this;
}

void Material::setTexture(unsigned int textureIndex, ITexture * texture)
{
	if(textureIndex > numberOfTextures )
		textures[textureIndex] = texture;
}

void Material::useMaterial()
{
	for ( unsigned int i = 0; i < numberOfTextures; i++ )
	{
		if ( textures[i] ) {
			glActiveTexture(GL_TEXTURE0 + i);
			textures[i]->bind();
			bindLocations[i] = i;
		}
	}
}

int Material::getTextureCount() const
{
	return numberOfTextures;
}

int Material::getTextureBindLocation(unsigned int textureIndex) const
{
	if ( textureIndex > numberOfTextures )
		return bindLocations[textureIndex];
	return 0;
}
