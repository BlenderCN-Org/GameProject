
#ifndef EDIT_LOADER_HPP
#define EDIT_LOADER_HPP

/// Internal Includes
#include "IEdit.hpp"

#include "../../Engine/AssetManager.hpp"
#include "../../Engine/DataTags.hpp"

/// External Includes

/// Std Includes

class EditLoader : public IEdit , public ILoader {

public:

	EditLoader(Engine::AssetManager& assetManager);
	virtual ~EditLoader();

	// Inherited via IEdit
	virtual void save() override;

	virtual void load(const char * fileName) override;

	virtual bool loading() override;

	virtual float getLoadingPercentage() override;

	virtual Engine::Entry loadEntry2(uint32_t entryId) override;
	virtual void freeEntry(Engine::Entry & ld) override;

private:

	Engine::AssetManager& assetMan;

};


#endif