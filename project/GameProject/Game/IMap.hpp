#ifndef I_MAP_HPP
#define I_MAP_HPP

/// Internal Includes
#include "IPlayer.hpp"
#include "RenderBatch.hpp"

/// External Includes

/// Std Includes

class Cell;

class IMap {

public:
	virtual ~IMap() {};

	virtual Cell* getCurrentCell() const = 0;
	virtual IPlayer* getPlayer() const = 0;
	
	virtual void update(float dt) = 0;
	virtual void updateRenderBatch(RenderBatch& batch) = 0;

	virtual void render() = 0;


};

#endif
