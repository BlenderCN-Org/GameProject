#include "../ThreadManager.hpp"

#include <AssetLib\AssetLib.hpp>
#include "../Assets/ModelAsset.hpp"
#include "../Assets/TextureAsset.hpp"

#include <assert.h>


void ThreadManager::loadAsset(AssetLoadStruct &ls) {

	uint32_t dataSize = 0;
	void* data = nullptr;

	switch ( ls.howToLoad.type ) {
		case LoadType::eLoadType_file:
		{
			switch ( ls.asset->getAssetType() ) {
				case AssetType::eAssetType_model:
				{
					printf("Loading mesh\n");
					data = AssetLib::loadWavefrontOBJ(ls.howToLoad.fileName, dataSize);
					ModelAsset* ma = (ModelAsset*)ls.asset;
					ma->setDataPtrAndSize(data, dataSize);
					delete data;
					ma->setAssetState(AssetState::eAssetState_loaded);
				}
				break;
				case AssetType::eAssetType_texture:
				{
					printf("Loading texture\n");
					int32_t w = 0, h = 0;
					data = AssetLib::loadBMP(ls.howToLoad.fileName, w, h);
					dataSize = w*h*4;
					TextureAsset* ta = (TextureAsset*)ls.asset;
					ta->setDataPtrAndSize(data, dataSize);
					ta->setTextureSize(w, h);
					ta->setAssetState(AssetState::eAssetState_loaded);
					delete data;
				}
				break;
				case AssetType::eAssetType_animation:
				{}
				break;
				default:
					break;
			}
		}
		break;
		case LoadType::eLoadType_memory:
			assert(0 && "memory data copy not implemented");
			break;
		case LoadType::eLoadType_network:
			assert(0 && "Network asset loading not implemented");
			break;
		default:
			assert(0 && "Undefined loading type");
			break;
	}
}
