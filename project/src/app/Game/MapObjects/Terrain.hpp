#ifndef TERRAIN_HPP
#define TERRAIN_HPP

/// Internal Includes

/// External Includes
#include <AssetHandling/Loader/MapLoader.hpp>

#include <glm/glm.hpp>
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

struct TerrainData {

	float heightOffset;
	float scale;
	uint8_t dataBits;
	uint8_t xRes;
	uint8_t yRes;
	uint8_t unused;
	uint32_t dataPointer;
};

class Terrain {

public:

	Terrain();
	Terrain(float cellSizeX, float cellSizeY);
	virtual ~Terrain();

	bool loadTerrain(Engine::AssetHandling::EntryData& terrainData);

private:

	bool verifyData(Engine::AssetHandling::EntryData& terrainData);

	IMesh * terrainMesh;

};

#endif
