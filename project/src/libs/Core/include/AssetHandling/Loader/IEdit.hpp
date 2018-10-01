#ifndef I_EDIT_HPP
#define I_EDIT_HPP 

/// Internal Includes
#include "ILoader.hpp"
#include "../IAssetManager.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace DataLoader {

		struct EditData {
			uint32_t changeCount;
			uint32_t editFlags;
		};

		class IEdit {

		public:
			virtual ~IEdit() {};

			virtual void save(const char* fileName) = 0;

			virtual bool loading() = 0;
			virtual float getLoadingPercentage() = 0;

			virtual uint32_t createNewEntry(AssetHandling::EntryData& data) = 0;
			virtual void deleteEntry(uint32_t entryId) = 0;
			virtual void editEntry(uint32_t entryId, AssetHandling::EntryData& data) = 0;

		};

	} // DataLoader
} // Engine

#endif
