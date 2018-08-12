/// Internal Includes
#include "DataTags.hpp"
#include "Cell.hpp"

/// External Includes

/// Std Includes


Cell::Cell()
	: mainCellGameObjects(nullptr)
	, borderCellGameObjects(nullptr)
	, terrain(CELL_SIZE / 2, CELL_SIZE / 2)
{}

Cell::~Cell() {

	delete[] mainCellGameObjects;
	delete[] borderCellGameObjects;

	mainCellGameObjects = nullptr;
	borderCellGameObjects = nullptr;
}

void Cell::load(Engine::DataLoader::ILoader** ppLoader, uint32_t cellId) {

}

void Cell::load(Engine::AssetHandling::EntryData& cellData, Engine::DataLoader::MapLoader& loader) {

	if (verifyData(cellData)) {
		CellData* cell = (CellData*)cellData.data;
		Engine::AssetHandling::EntryData terrainData = loader.loadEntry(cell->terrainId);
		terrain.loadTerrain(terrainData);
		loader.freeEntry(terrainData);
	}
}

void Cell::update(float dt) {

}

bool Cell::verifyData(Engine::AssetHandling::EntryData& cellData) {

	bool dataOk = false;

	if (cellData.tag == CELL_TAG) {
	
		dataOk = true;
	}

	return dataOk;
}