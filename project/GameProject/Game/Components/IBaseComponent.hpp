#ifndef IBASECOMPONENT_HPP
#define IBASECOMPONENT_HPP

#include "BaseComponent.hpp"

enum class Components : uint64_t
{
	RENDER_COMPONENT = 1,
};

template< typename T>
class IBaseComponent : public BaseComponent
{
public:

	static const uint64_t typeID;
	
	inline virtual uint64_t getComponentID() const
	{
		return typeID;
	};

};


#endif