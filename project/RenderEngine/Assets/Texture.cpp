#include "Texture.hpp"

#include "../Utils/MemoryManager.hpp"
#include <memory>

void Texture::init(int _internalComponents, bool compress) {
	
	glGenTextures(1, &textureID);
	components = 0;
	width = 0;
	height = 0;
	textureData = nullptr;

	compressTexture = compress;

	internalComponents = _internalComponents;

	update = false;
	compressChange = false;

	int format = 0;
	if(compressTexture ) {
		switch ( internalComponents )
		{
			case 1:
				internalFormat = GL_COMPRESSED_RED;
				break;
			case 2:
				internalFormat = GL_COMPRESSED_RG;
				break;
			case 3:
				internalFormat = GL_COMPRESSED_RGB;
				break;
			case 4:
				internalFormat = GL_COMPRESSED_RGBA;
				break;
			default:
				break;
		}
	} else {
		switch ( internalComponents )
		{
			case 1:
				internalFormat = GL_RED;
				break;
			case 2:
				internalFormat = GL_RG;
				break;
			case 3:
				internalFormat = GL_RGB;
				break;
			case 4:
				internalFormat = GL_RGBA;
				break;
			default:
				break;
		}
	}
}

void Texture::release() {
	
	glDeleteTextures(1, &textureID);
	
	if ( textureData ) {
		delete textureData;
		//MemoryManager::getMemoryManager()->deallocate(textureData);
		textureData = nullptr;
	}

	delete this;
}

void Texture::setCompressed(bool compression)
{
	if ( compressTexture == compression ) return;
	
	compressTexture = compression;

	if ( compressTexture ) {
		switch ( internalComponents )
		{
			case 1:
				internalFormat = GL_COMPRESSED_RED;
				break;
			case 2:
				internalFormat = GL_COMPRESSED_RG;
				break;
			case 3:
				internalFormat = GL_COMPRESSED_RGB;
				break;
			case 4:
				internalFormat = GL_COMPRESSED_RGBA;
				break;
			default:
				break;
		}
	} else {
		switch ( internalComponents )
		{
			case 1:
				internalFormat = GL_RED;
				break;
			case 2:
				internalFormat = GL_RG;
				break;
			case 3:
				internalFormat = GL_RGB;
				break;
			case 4:
				internalFormat = GL_RGBA;
				break;
			default:
				break;
		}
	}
	compressChange = true;
}

void Texture::setTextureData(int _width, int _height, int _components, void * _data)
{
	components = _components;
	width = _width;
	height = _height;

	size_t size = width*height*components;

	if ( textureData ) {
		if ( update )
			return;
		else
			update = true;
		return;
		//MemoryManager::getMemoryManager()->deallocate(textureData);
		//textureData = nullptr;
	}

	void* d = new char[size];//MemoryManager_alloc(char, size);

	if ( d ) {
		memcpy(d, _data, size);

		// maybe unsafe idk
		textureData = d;
		update = true;
	}

}

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if ( update )
	{
		int format = 0;
		switch ( components )
		{
			case 1:
				format = GL_RED;
				break;
			case 2:
				format = GL_RG;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				break;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
		//MemoryManager::getMemoryManager()->deallocate(textureData);
		delete textureData;
		textureData = nullptr;
		int comp = false;
		int size = 0;
		
		int target = GL_TEXTURE_2D;
		int level = 0;



		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &comp);

		if ( comp ) {
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size);
		} else {
			//glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &oldW);
			//glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &oldH);
			
			int internalComponentSize = 0;
			
			glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
			int temp;
			glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
			internalComponentSize += temp;
			glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
			internalComponentSize += temp;
			glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
			internalComponentSize += temp;
			glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
			internalComponentSize += temp;
			
			internalComponentSize /= 8;

			size = width*height*internalComponentSize;
		}

		printf("Compressed: %d\n", comp);
		printf("Size: %d\n", size);
		update = false;
		compressChange = false;
	}
	else if ( compressChange )
	{
		int format;
		switch ( components )
		{
			case 1:
				format = GL_RED;
				break;
			case 2:
				format = GL_RG;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				break;
		}

		int target = GL_TEXTURE_2D;
		int level = 0;
		int size = 0;

		int internalComponentSize = 0;

		glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
		int temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
		internalComponentSize += temp;

		internalComponentSize /= 8;

		size = width*height*internalComponentSize;

		textureData = new char[size];

		glGetnTexImage(target, level, format, GL_UNSIGNED_BYTE, size, textureData);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
		//MemoryManager::getMemoryManager()->deallocate(textureData);
		delete textureData;
		textureData = nullptr;
		int comp = false;
		size = 0;

		target = GL_TEXTURE_2D;
		level = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &comp);

		if ( comp ) {
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size);
		} else {
			//glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &oldW);
			//glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &oldH);

			int internalComponentSize = 0;

			glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
			int temp;
			glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
			internalComponentSize += temp;
			glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
			internalComponentSize += temp;
			glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
			internalComponentSize += temp;
			glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
			internalComponentSize += temp;

			internalComponentSize /= 8;

			size = width*height*internalComponentSize;
		}

		printf("Compressed: %d\n", comp);
		printf("Size: %d\n", size);

		compressChange = false;
		update = false;
	}
}
