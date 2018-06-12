#ifndef SKY_DOME_HPP
#define SKY_DOME_HPP

/// Internal Includes

/// External Includes
#include <RenderEngine/IMesh.hpp>

/// Std Includes

class SkyDome {

public:

	SkyDome();
	virtual ~SkyDome();

	void render();

private:

	IMesh* skydome;

};

#endif