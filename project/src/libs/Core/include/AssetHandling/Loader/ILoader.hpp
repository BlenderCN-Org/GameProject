#ifndef I_LOADER_HPP
#define I_LOADER_HPP

/// Internal Includes
#include "../AssetManager.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace DataLoader {

		struct Header {
			uint32_t version;
			uint32_t numEntries;
		};

		struct OffsetData {
			uint32_t entry;
			uint32_t offset;
		};

		class ILoader {

		public:
			virtual ~ILoader() {}

			virtual void openFile(const char* fileName) = 0;
			virtual void closeFile() = 0;

			virtual Engine::AssetHandling::EntryData loadEntry(uint32_t entryId) = 0;
			virtual void freeEntry(Engine::AssetHandling::EntryData& ld) = 0;

		};
	} // DataLoader
} // Engine

#endif
