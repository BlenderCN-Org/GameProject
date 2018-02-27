#ifndef CELL_HPP
#define CELL_HPP 

/// Internal Includes
#include "GameObject.hpp"

#include "Loader/MapLoader.hpp"
/// External Includes

/// Std Includes

/// Constants
const float CELL_SIZE = 20.0F;
const float BORDER_SIZE = 1.0F;

class Cell {

public:

	Cell();
	virtual ~Cell();

	void load(LoadedData& data);

	void update(float dt);

private:

	bool verifyData(LoadedData& data);

	GameObject* mainCellGameObjects;
	GameObject* borderCellGameObjects;



};

#endif