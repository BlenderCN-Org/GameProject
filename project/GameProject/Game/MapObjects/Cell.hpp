#ifndef CELL_HPP
#define CELL_HPP 

/// Internal Includes
#include "Terrain.hpp"

#include "../GameObject.hpp"

/// External Includes
#include <EngineCore/AssetHandling/Loader/MapLoader.hpp>

/// Std Includes

/// Constants
const float CELL_SIZE = 20.0F;
const float BORDER_SIZE = 1.0F;

struct CellData {

	uint32_t terrainId;
	uint32_t numObjects;
	uint32_t objIdsList;
	uint32_t numObjectsAtBorder;
	uint32_t objIdsBorderList;
};

class Cell {

public:

	Cell();
	virtual ~Cell();

	virtual void load(Engine::DataLoader::ILoader** ppLoader, uint32_t cellId);
	virtual void load(Engine::AssetHandling::EntryData& cellData, Engine::DataLoader::MapLoader& loader);

	void update(float dt);

private:

	Engine::DataLoader::ILoader** ppLoader;

	bool verifyData(Engine::AssetHandling::EntryData& cellData);

	GameObject* mainCellGameObjects;
	GameObject* borderCellGameObjects;

	Terrain terrain;

};

#endif