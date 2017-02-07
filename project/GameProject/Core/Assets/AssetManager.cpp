#include "AssetManager.hpp"

#include "AssetLoader.hpp"

#include <AssetLib\AssetLib.hpp>

AssetManager* AssetManager::assetManager = nullptr;

AssetManager * AssetManager::getAssetManager() {
	if ( assetManager == nullptr )
		assetManager = new AssetManager();
	return assetManager;
}

void AssetManager::release() {
	if ( assetManager )
		delete assetManager;
	assetManager = nullptr;
}

AssetManager::AssetManager() : basicFont(nullptr), loadedMeshList(0) {}

AssetManager::~AssetManager() {
	if ( basicFont ) {
		basicFont->release();
	}

	if ( smallFont ) {
		smallFont->release();
	}

	for ( size_t i = 0; i < loadedMeshList.size(); i++ ) {
		loadedMeshList[i].mesh->release();
	}
}

void AssetManager::init(IRenderEngine * renderEngine) {
	re = renderEngine;
}

IMesh * AssetManager::getMesh(char * meshName) {
	IMesh* mesh = nullptr;

	for ( size_t i = 0; i < loadedMeshList.size(); i++ ) {
		// @Speed
		// should use a hashed identifier instead or make it a map
		if ( strcmp(loadedMeshList[i].meshName.c_str(), meshName) == 0 ) {
			mesh = loadedMeshList[i].mesh;
		}
	}

	// mesh was not found load it from file
	if ( mesh == nullptr ) {
		mesh = re->createMesh();
		mesh->init(MeshPrimitiveType::TRIANGLE);

		uint32_t dataSize = 0;
		void * data = AssetLib::fileToMemory(meshName, dataSize);

		void * meshData = createVertUVData(data, dataSize);

		delete data;

		mesh->setMeshData(meshData, dataSize, MeshDataLayout::VERT_UV);

		delete meshData;

		MeshAsset ma = { meshName, mesh };
		loadedMeshList.push_back(ma);
	}

	return mesh;
}

IFont * AssetManager::getBasicFont() {
	if ( basicFont == nullptr ) {
		basicFont = re->createFont();
		basicFont->init("C:/Windows/Fonts/Arial.ttf", 24);
	}

	return basicFont;
}

IFont* AssetManager::getSmallFont() {
	if ( smallFont == nullptr ) {
		smallFont = re->createFont();
		smallFont->init("C:/Windows/Fonts/Arial.ttf", 8);
	}

	return smallFont;
}