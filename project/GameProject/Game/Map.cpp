
/// Internal Includes
#include "Camera.hpp"
#include "Map.hpp"

/// External Includes

/// Std Includes

Map::Map(LoadedData& mapData, MapLoader& ldr)
	: sky(nullptr)
	, sizeX(0)
	, sizeY(0)
	, cells(nullptr)
	, player(nullptr) {

	if (verifyData(mapData)) {

		MapData* md = (MapData*)mapData.data;

		uint32_t skyId = md->skyId;

		if (skyId != 0) {
			LoadedData skyData = ldr.loadDataEntry(skyId);

			sky = new Sky(skyData);

			ldr.freeData(skyData);
		}

		uint32_t* ids = &md->cellsPointer;

		sizeX = md->cellsX;
		sizeY = md->cellsY;

		size_t s = md->cellsX * md->cellsY;

		cells = new Cell*[md->cellsX];

		for (uint32_t i = 0; i < md->cellsX; i++) {
			cells[i] = new Cell[md->cellsY];

			for (uint32_t j = 0; j < md->cellsY; j++) {
				LoadedData data = ldr.loadDataEntry(ids[i]);
				cells[i][j].load(data);
				ldr.freeData(data);
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
