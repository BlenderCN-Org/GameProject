
/// Internal Includes
#include "Map.hpp"
#include "../Camera.hpp"

/// External Includes

/// Std Includes

Map::Map(LoadedData& loadedData, MapLoader& loader)
	: sky(nullptr)
	, sizeX(0)
	, sizeY(0)
	, cells(nullptr)
	, player(nullptr) {

	if (verifyData(loadedData)) {

		MapData* mapData = (MapData*)loadedData.data;

		loadSky(mapData->skyId, loader);

		uint32_t* ids = &mapData->cellsPointer;

		sizeX = mapData->cellsX;
		sizeY = mapData->cellsY;

		size_t s = mapData->cellsX * mapData->cellsY;

		cells = new Cell*[mapData->cellsX];

		for (uint32_t i = 0; i < mapData->cellsX; i++) {
			cells[i] = new Cell[mapData->cellsY];

			for (uint32_t j = 0; j < mapData->cellsY; j++) {
				LoadedData data = loader.loadEntry(ids[i]);
				cells[i][j].load(data);
				loader.freeEntry(data);
			}
		}

		//for (size_t i = 0; i < s; i++) {
		//	printf("Cells : index %d, id %d\n", i, ids[i]);
		//}

		int b = 0;

	} else {

	}

	player = new Player();

}

Map::~Map() {

	delete player;

	if (sky) {
		delete sky;
	}

	if (cells) {

		for (size_t i = 0; i < sizeX; i++) {
			delete[] cells[i];
		}

		delete[] cells;
	}
}

Cell* Map::getCurrentCell() const {
	return nullptr;
}

IPlayer* Map::getPlayer() const {
	return player;
}

void Map::update(float dt) {
	if (sky) {
		sky->update(dt);
	}



}

void Map::updateRenderBatch(RenderBatch& batch) {
	
	TemporaryStorage* ts = batch.getBatchTempStorage();

	CameraSettings camSettings = batch.getCameraSettings();
	
	SkyCommand* sc = ts->allocate<SkyCommand>(1, sky, camSettings.vpMat, camSettings.cameraPosition, camSettings.cameraDirection, nullptr);
	batch.addCommand(sc);

}


/********************************************
*											*
*			PRIVATE FUNCTIONS				*
*											*
*********************************************/

void Map::loadSky(uint32_t skyId, MapLoader& loader) {
	
	if (skyId != 0) {
		LoadedData skyData = loader.loadEntry(skyId);

		sky = new Sky(skyData);

		loader.freeEntry(skyData);
	}
}

void Map::createCells() {}

bool Map::verifyData(LoadedData& data) {

	bool dataOk = false;

	// it is a map
	if (data.tag == MAP_TAG) {

		MapData* md = (MapData*)data.data;

		uint32_t calcSize = (md->cellsX * md->cellsY) * sizeof(uint32_t);

		// size of nr cells and sky id;
		calcSize += sizeof(uint32_t) * 3;

		if (calcSize == data.size) {
			dataOk = true;
		}
	}

	return dataOk;
}
