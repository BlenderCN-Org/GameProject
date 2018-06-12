#ifndef MAP_HPP
#define MAP_HPP

/// Internal Includes
#include "Cell.hpp"
#include "Sky.hpp"

#include "../IMap.hpp"
#include "../Player.hpp"
#include "../RenderBatch.hpp"
#include "../Loader/MapLoader.hpp"

/// External Includes

/// Std Includes

struct MapData {
	uint32_t cellsX;
	uint32_t cellsY;
	uint32_t skyId;
	uint32_t cellsPointer;
};

class Map : public IMap {

public:

	Map(LoadedData& mapData, MapLoader& ldr);
	virtual ~Map();

	virtual Cell* getCurrentCell() const override;
	virtual IPlayer* getPlayer() const override;

	virtual void update(float dt) override;
	virtual void updateRenderBatch(RenderBatch& batch) override;

	virtual void render() override {};

private:


	void createCells();
	bool verifyData(LoadedData& data);
	
	Sky * sky;

	uint32_t sizeX;
	uint32_t sizeY;

	Cell** cells;

	Player* player;

};

#endif