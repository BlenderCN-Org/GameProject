
/// Internal Includes
#include "Map.hpp"


Map::Map(LoadedData& mapData)
	: sky(nullptr)
	, sizeX(0)
	, sizeY(0)
	, cells(nullptr) {

	if (mapData.tag == MAP_TAG) {

		MapData* md = (MapData*)mapData.data;

		uint32_t* ids = &md->cellsPointer;

		size_t s = md->cellsX * md->cellsY;

		for (size_t i = 0; i < s; i++) {
			printf("Cells : index %d, id %d\n", i, ids[i]);
		}

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
