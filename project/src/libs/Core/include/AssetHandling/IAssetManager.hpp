#ifndef I_ASSET_MANAGER_HPP
#define I_ASSET_MANAGER_HPP

/// Internal Includes

/// External Includes

/// Std Includes
#include <cstdint>
#include <string>

namespace Engine {
	namespace AssetHandling {

		struct Tag {
			char tag[4];

			bool operator != (const Tag& right) {
				return !operator==(right);
			}

			bool operator==(const Tag& right) {
				if (tag[0] == right.tag[0] &&
					tag[1] == right.tag[1] &&
					tag[2] == right.tag[2] &&
					tag[3] == right.tag[3]) {
					return true;
				}
				return false;
			}
		};

		/// Constants
		const Tag NULL_TAG { 'N', 'U', 'L', 'L' };

		struct EntryDef {
			uint32_t entryId;
			uint32_t size;
			Tag tag;
			void* data;
			uint32_t engineFlagsLow;
			uint32_t engineFlagsHigh;
		};

		struct ResourceDef {
			uint32_t size;
			void* data;
		};

		struct ResourceThing {
			std::string name;

			friend bool operator<(const ResourceThing& l, const ResourceThing& r) {
				return l.name < r.name;
			}

		};

		typedef EntryDef EntryData;
		typedef ResourceDef AssetData;

		typedef ResourceThing ResourceIdentifier;

		class IAssetManager {
		public:

			virtual bool isAssetLoaded(ResourceIdentifier resourceName) const = 0;
			virtual AssetData loadAsset(ResourceIdentifier resourceName) = 0;
			virtual void unloadAsset(AssetData& assetData) = 0;

			virtual uint32_t numLoadedEntries() const = 0;
			virtual EntryData entryFromIndex(uint32_t idx) const = 0;

			virtual bool isEntryLoaded(uint32_t entryId) const = 0;
			virtual EntryData loadEntry(uint32_t entryId) = 0;
			virtual void unloadEntry(EntryData& entryData) = 0;
			
			virtual void cacheData(uint32_t entryId, EntryData data) = 0;
			virtual void cacheData(ResourceIdentifier resourceName, AssetData data) = 0;

			virtual void* loadFont(char* font, int size) = 0;

		};

	}
}

#endif