#ifndef MAP_HPP
#define MAP_HPP

/// Internal Includes
#include "Sky.hpp"
#include "Cell.hpp"
#include "RenderBatch.hpp"

#include "Loader/MapLoader.hpp"

/// External Includes

/// Std Includes

struct MapData {
	uint32_t cellsX;
	uint32_t cellsY;
	uint32_t skyId;
	uint32_t cellsPointer;
};

class Map {

public:

	Map(LoadedData& mapData);
	virtual ~Map();

	void update(float dt);

	void updateRenderBatch(RenderBatch& batch);

private:

	Sky * sky;

	uint32_t sizeX;
	uint32_t sizeY;

	Cell** cells;

};

#endif