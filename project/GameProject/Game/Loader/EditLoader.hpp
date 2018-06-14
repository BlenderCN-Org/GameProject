
#ifndef EDIT_LOADER_HPP
#define EDIT_LOADER_HPP

/// Internal Includes
#include "DataTags.hpp"
#include "../IEdit.hpp"

/// External Includes

/// Std Includes

class EditLoader : public IEdit {

public:


	// Inherited via IEdit
	virtual void save() override;

	virtual void load(const char * fileName) override;

	virtual bool loading() override;

	virtual float getLoadingPercentage() override;

};


#endif