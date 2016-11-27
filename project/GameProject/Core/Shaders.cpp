#include "Shaders.hpp"

Shaders* Shaders::singleInstance = nullptr;

Shaders* Shaders::getShaders() {
	if ( singleInstance == nullptr )
		singleInstance = new Shaders();
	return singleInstance;
}

void Shaders::release() {
	delete singleInstance;
	singleInstance = nullptr;
}
