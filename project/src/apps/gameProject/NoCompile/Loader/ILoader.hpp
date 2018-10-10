#ifndef I_LOADER_HPP
#define I_LOADER_HPP

/// Internal Includes
#include "../../Engine/AssetManager.hpp"
#include "../../Engine/DataTags.hpp"

/// External Includes

/// Std Includes

class ILoader {

public:
	virtual ~ILoader() {}

	virtual Engine::Entry loadEntry2(uint32_t entryId) = 0;
	virtual void freeEntry(Engine::Entry& ld) = 0;

};


#endif