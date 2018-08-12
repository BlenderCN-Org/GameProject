#ifndef I_EDIT_HPP
#define I_EDIT_HPP 

/// Internal Includes
#include "ILoader.hpp"

/// External Includes

/// Std Includes

class IEdit {

public:
	virtual ~IEdit() {};
	
	virtual void save() = 0;
	virtual void load(const char* fileName) = 0;

	virtual bool loading() = 0;
	virtual float getLoadingPercentage() = 0;
	
};

#endif