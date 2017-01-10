#ifndef LOADER_HPP
#define LOADER_HPP

#include <stdint.h>

#include "LoaderDescriptor.hpp"
#include "LoadingScreen.hpp"

#include <vector>

class Loader
{
public:

private:

	// loaded stuff
	std::vector<LoaderDescriptor> loadedAssets;

	// unloaded stuff
	std::vector<LoaderDescriptor> unloadedAssets;

	// loading screen stuff
	bool loadingScreenActive;
	LoadingScreen ls;

	uint64_t itemsToLoad;
	uint64_t itemsLoaded;

	std::vector<LoaderDescriptor> itemsToLoad;
};

#endif