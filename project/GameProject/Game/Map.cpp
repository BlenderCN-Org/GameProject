
/// Internal Includes
#include "Map.hpp"


Map::Map(LoadedData& mapData, MapLoader& ldr)
	: sky(nullptr)
	, sizeX(0)
	, sizeY(0)
	, cells(nullptr) {

	if (verifyData(mapData)) {

		MapData* md = (MapData*)mapData.data;

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
				free(data.data);
			}
		}

		//for (size_t i = 0; i < s; i++) {
		//	printf("Cells : index %d, id %d\n", i, ids[i]);
		//}

		int b = 0;

	} else {

	}

}

Map::~Map() {

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

void Map::update(float dt) {}

void Map::updateRenderBatch(RenderBatch& batch) {}

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

void Map::createCells() {}