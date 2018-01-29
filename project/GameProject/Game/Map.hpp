#ifndef MAP_HPP
#define MAP_HPP

/// Internal Includes
#include "Sky.hpp"

/// External Includes

/// Std Includes

class Map {

public:

	Map();
	virtual ~Map() {};

	void update(float dt);



private:

	Sky* mapSky;

};

#endif