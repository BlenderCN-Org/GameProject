#ifndef MAP_HPP
#define MAP_HPP

/// Internal Includes
#include "Cell.hpp"
#include "Sky.hpp"

#include "../IMap.hpp"
#include "../Player.hpp"
#include "../RenderBatch.hpp"
//#include "../Loader/MapLoader.hpp"
#include "../../Engine/Core/Grid.hpp"

/// External Includes
#include <AssetHandling/Loader/MapLoader.hpp>

/// Std Includes

struct MapData {
	uint32_t cellsX;
	uint32_t cellsY;
	uint32_t skyId;
	uint32_t cellsPointer;
};

class Map : public IMap {

public:

	Map(Engine::DataLoader::ILoader** loader, uint32_t mapId);
	virtual ~Map();

	virtual Cell* getCurrentCell() const override;
	virtual IPlayer* getPlayer() const override;

	virtual void reloadCheck() override;

	virtual void update(float dt) override;
	virtual void updateRenderBatch(RenderBatch& batch) override;

	virtual void render() override {};

	virtual Grid<Cell>* getCellGrid() override;

private:

	Engine::DataLoader::ILoader** ppLoader;
	Engine::AssetHandling::EntryData mapData;

	void loadSky(uint32_t skyId);

	bool verifyData(Engine::AssetHandling::EntryData& data, uint32_t mapId);
	
	Sky* sky;

	Grid<Cell> cellGrid;

	uint32_t sizeX;
	uint32_t sizeY;

	Player* player;

};

#endif