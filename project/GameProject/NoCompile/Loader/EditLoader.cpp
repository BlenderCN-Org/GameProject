
/// Internal Includes
#include "EditLoader.hpp"

/// External Includes

/// Std Includes

EditLoader::EditLoader(Engine::AssetManager& assetManager) : assetMan(assetManager) {}

EditLoader::~EditLoader() {

}

void EditLoader::save() {}

void EditLoader::load(const char * fileName) {}

bool EditLoader::loading() {
	return false;
}

float EditLoader::getLoadingPercentage() {
	return 0.0f;
}

Engine::Entry EditLoader::loadEntry2(uint32_t entryId) {
	return Engine::Entry();
}
void EditLoader::freeEntry(Engine::Entry & ld) {

}