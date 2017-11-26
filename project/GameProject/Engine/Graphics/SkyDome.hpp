#ifndef SKY_DOME_HPP
#define SKY_DOME_HPP

#include <RenderEngine/IMesh.hpp>

class SkyDome {

public:

	SkyDome();
	virtual ~SkyDome();

	void render();

private:

	IMesh* skydome;

};

#endif