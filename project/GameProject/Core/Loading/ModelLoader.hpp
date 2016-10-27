#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP

#include "AssetStructures.hpp"

class ModelLoader
{

public:

	static void* load(const char* fileName, unsigned int &dataSize);

};

#endif