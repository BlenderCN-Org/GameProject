/// Internal Includes
#include "Cell.hpp"

/// External Includes

/// Std Includes


Cell::Cell()
	: mainCellGameObjects(nullptr)
	, borderCellGameObjects(nullptr) {}

Cell::~Cell() {

	delete[] mainCellGameObjects;
	delete[] borderCellGameObjects;

	mainCellGameObjects = nullptr;
	borderCellGameObjects = nullptr;
}

void Cell::load(LoadedData& data) {

	if (verifyData(data)) {

	}
}

void Cell::update(float dt) {

}

bool Cell::verifyData(LoadedData& data) {

	bool dataOk = false;

	if (data.tag == CELL_TAG) {

		dataOk = true;
	}

	return dataOk;
}