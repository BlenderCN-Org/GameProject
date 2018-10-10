
/// Internal Includes
#include "DataTags.hpp"
#include "Map.hpp"
#include "../Camera.hpp"
#include "../../Engine/Core/Console.hpp"

/// External Includes

/// Std Includes

Map::Map(Engine::DataLoader::ILoader** loader, uint32_t mapId)
	: sky(nullptr)
	, sizeX(0)
	, sizeY(0)
	, player(nullptr)
	, ppLoader(loader) {

	mapData = (*ppLoader)->loadEntry(mapId);

	if (verifyData(mapData, mapId)) {

		MapData* convertedData = (MapData*)mapData.data;

		loadSky(convertedData->skyId);

		uint32_t* cellIds = &convertedData->cellsPointer;

		sizeX = convertedData->cellsX;
		sizeY = convertedData->cellsY;

		GridSettings gridSettings;
		gridSettings.width = sizeX;
		gridSettings.height = sizeY;
		gridSettings.offsetType = GridOffsetType::GRID_OFFSET_NONE;

		cellGrid.init(gridSettings);

		uint32_t numCells = sizeX * sizeY;

		for (uint32_t i = 0; i < numCells; i++) {
			Engine::AssetHandling::EntryData data = (*ppLoader)->loadEntry(cellIds[i]);
			//cellGrid[i].load(data, loader);
			(*ppLoader)->freeEntry(data);
		}
	} else {
		Engine::Core::FormattedString errorMessage = "Failed to load map";
		Engine::Core::gConsole->print(errorMessage, Engine::Core::LogSeverity::LOG_ERROR);
	}
}

Map::~Map() {

	(*ppLoader)->freeEntry(mapData);
	ppLoader = nullptr;

	delete player;

	if (sky) {
		delete sky;
	}

}

Cell* Map::getCurrentCell() const {
	return nullptr;
}

IPlayer* Map::getPlayer() const {
	return player;
}

void Map::reloadCheck() {

	Engine::AssetHandling::EntryData temp = (*ppLoader)->loadEntry(mapData.entryId);
	if (mapData.engineFlagsLow != temp.engineFlagsLow || mapData.engineFlagsHigh != temp.engineFlagsHigh) {

		MapData* convertedData = (MapData*)mapData.data;
		MapData* newConvData = (MapData*)temp.data;

		if (convertedData->skyId != newConvData->skyId) {

			delete sky;
			sky = nullptr;

			loadSky(newConvData->skyId);
		}

		if (convertedData->cellsX != newConvData->cellsX ||
			convertedData->cellsY != newConvData->cellsY) {
			//uint32_t* cellIds = &convertedData->cellsPointer;

			sizeX = newConvData->cellsX;
			sizeY = newConvData->cellsY;

			GridSettings gridSettings;
			gridSettings.width = sizeX;
			gridSettings.height = sizeY;
			gridSettings.offsetType = GridOffsetType::GRID_OFFSET_NONE;

			cellGrid.reinit(gridSettings);

			//uint32_t numCells = sizeX * sizeY;
		}

		Engine::AssetHandling::EntryData newData = temp;
		temp = mapData;
		mapData = newData;

	}

	(*ppLoader)->freeEntry(temp);

	if (sky) {
		sky->reloadCheck();
	}

}

void Map::update(float dt) {
	if (sky) {
		sky->update(dt);
	}

}

void Map::updateRenderBatch(RenderBatch& batch) {

	TemporaryStorage* ts = batch.getBatchTempStorage();

	CameraSettings camSettings = batch.getCameraSettings();

	if (sky) {
		SkyCommand* sc = ts->allocate<SkyCommand>(1, sky, camSettings.vpMat, camSettings.cameraPosition, camSettings.cameraDirection, nullptr);
		batch.addCommand(sc);
	}

}

Grid<Cell>* Map::getCellGrid() {
	return &cellGrid;
}

/********************************************
*											*
*			PRIVATE FUNCTIONS				*
*											*
*********************************************/

void Map::loadSky(uint32_t skyId) {

	sky = new Sky(ppLoader, skyId);
}

bool Map::verifyData(Engine::AssetHandling::EntryData& data, uint32_t mapId) {
	bool dataOk = false;

	// it is a map
	if (data.entryId == mapId) {
		if (data.tag == MAP_TAG) {

			MapData* md = (MapData*)data.data;

			uint32_t calcSize = (md->cellsX * md->cellsY) * sizeof(uint32_t);

			// size of nr cells and sky id;
			calcSize += sizeof(uint32_t) * 3;

			if (calcSize == data.size) {
				dataOk = true;
			}
		}
	}

	return dataOk;
}