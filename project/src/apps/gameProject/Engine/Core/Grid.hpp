
#ifndef GRID_HPP
#define GRID_HPP

/// Internal Includes

/// External Includes

/// Std Includes
#include <cstdint>

enum class GridOffsetType {
	GRID_OFFSET_NONE,
	GRID_OFFSET_CENTER_EVEN_POSITIVE,
	GRID_OFFSET_CENTER_EVEN_NEGATIVE,
	GRID_OFFSET_REVERSED,
};

struct GridSettings {

	int width;
	int height;
	GridOffsetType offsetType;

};

struct GridRange {
	int minX;
	int minY;
	int maxX;
	int maxY;
};

struct GridItemPosition {
	int x;
	int y;
};

template <typename T>
class Grid {

public:

	Grid();
	virtual ~Grid();

	void init(GridSettings initSettings);
	void reinit(GridSettings initSettings);

	int size() const;

	GridRange getRange() const;

	T& get(int x, int y);

	GridItemPosition getGridPosition(int index);

	T& operator[](int index);

private:

	int offsetX;
	int offsetY;

	int width;
	int height;

	T* tArray;
};


template<typename T>
Grid<T>::Grid() : height(0), width(0) {
	tArray = nullptr;
	offsetX = offsetY = 0;
}

template<typename T>
Grid<T>::~Grid() {
	delete[] tArray;
}

template<typename T>
void Grid<T>::init(GridSettings initSettings) {
	width = initSettings.width;
	height = initSettings.height;

	if (GridOffsetType::GRID_OFFSET_NONE == initSettings.offsetType) {
		offsetX = offsetY = 0;
	} else if (GridOffsetType::GRID_OFFSET_CENTER_EVEN_POSITIVE == initSettings.offsetType) {
		offsetX = (int)floor(width / 2.0);
		offsetY = (int)floor(height / 2.0);
	} else if (GridOffsetType::GRID_OFFSET_CENTER_EVEN_NEGATIVE == initSettings.offsetType) {
		offsetX = (int)ceil(width / 2.0);
		offsetY = (int)ceil(height / 2.0);
	} else if (GridOffsetType::GRID_OFFSET_REVERSED == initSettings.offsetType) {
		offsetX = width;
		offsetY = height;
	}

	tArray = new T[width * height];
}

template<typename T>
void Grid<T>::reinit(GridSettings initSettings) {
	delete[] tArray;
	init(initSettings);
}

template<typename T>
int Grid<T>::size() const {
	return width * height;
}

template<typename T>
GridRange Grid<T>::getRange() const {

	GridRange range;

	range.maxX = width - offsetX;
	range.maxY = height - offsetY;
	range.minX = -offsetX;
	range.minY = -offsetY;

	return range;
}

template<typename T>
T& Grid<T>::get(int x, int y) {

	int index = (x + offsetX) + ((y + offsetY) * width);

	return tArray[index];
}

template<typename T>
GridItemPosition Grid<T>::getGridPosition(int index) {

	GridItemPosition pos;
	pos.x = index % width;
	pos.y = index / width;

	pos.x -= offsetX;
	pos.y -= offsetY;

	return pos;
}

template<typename T>
T& Grid<T>::operator[](int index) {
	return tArray[index];
}

#endif