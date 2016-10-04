#ifndef SHADERSTATE_HPP
#define SHADERSTATE_HPP

#include "ICamera.hpp"

class ShaderState
{
public:

	void setActiveCamera(ICamera* cam);
	

//private:

	ICamera* activeCamera = nullptr;

};

#endif