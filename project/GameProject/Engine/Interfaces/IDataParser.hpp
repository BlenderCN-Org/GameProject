#ifndef I_LOADER_HPP
#define I_LOADER_HPP

/// Internal Includes
#include "IAssetData.hpp"
#include "../Utils/MemoryBuffer.hpp"

namespace Engine {
	namespace Interfaces {
		class IDataParser {

		public:
			virtual ~IDataParser() {};

			virtual void load(Core::MemoryBuffer& unParsedData, IAssetData* assetDataPtr) = 0;

		};
	}
}

#endif