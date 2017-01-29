#ifndef MESHASSET_HPP
#define MESHASSET_HPP

#include <RenderEngine\IMesh.hpp>

#include <string>

struct MeshAsset
{
	std::string meshName;

	IMesh* mesh;
};


#endif