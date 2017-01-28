#include "AssetManager.hpp"

AssetManager::AssetManager() : basicFont(nullptr), loadedMeshList(0) {}

AssetManager::~AssetManager() {
}

void AssetManager::init(IRenderEngine * renderEngine) {
	re = renderEngine;
}

void AssetManager::release() {
	if ( basicFont ) {
		basicFont->release();
	}
}

IMesh * AssetManager::getMesh(char * meshName) {
	return nullptr;
}

IFont * AssetManager::getBasicFont() {
	if ( basicFont == nullptr ) {
		basicFont = re->createFont();
		basicFont->init("C:/Windows/Fonts/Arial.ttf", 24);
	}

	return basicFont;
}
