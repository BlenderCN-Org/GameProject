
/// Internal Includes
#include "DataTags.hpp"
#include "Terrain.hpp"

/// External Includes

/// Std Includes

Terrain::Terrain() {

}

Terrain::Terrain(float cellSizeX, float cellSizeY) {

}

Terrain::~Terrain() {

}

bool Terrain::loadTerrain(Engine::AssetHandling::EntryData& terrainData) {

	if (verifyData(terrainData)) {

	}

	return true;
}


bool Terrain::verifyData(Engine::AssetHandling::EntryData& terrainData) {

	bool dataOk = false;

	if (terrainData.tag == MESH_TAG) {
		dataOk = true;
	}

	return dataOk;
}