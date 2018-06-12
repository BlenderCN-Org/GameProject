#ifndef I_ASSET_MANAGER_HPP
#define I_ASSET_MANAGER_HPP 

/// Interface Includes
#include "ICMesh.hpp"

namespace Engine {
	namespace Interfaces {

		class IAssetManager {

		public:

			virtual ~IAssetManager() {};

			virtual ICMesh* loadMesh(const char* model) = 0;
		};
	}
}

#endif