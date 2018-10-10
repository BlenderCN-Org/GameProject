#ifndef IBASECOMPONENT_HPP
#define IBASECOMPONENT_HPP

#include "BaseComponent.hpp"

enum class Components : uint32_t {
	RENDER_COMPONENT = 1,
	PHYSICS_COMPONENT,
	TRANSFORM_COMPONENT,
	SCRIPT_COMPONENT,
	STATS_COMPONENT,
	MOVE_COMPONENT,
	TRIGGER_COMPONENT,
	ANIMATION_COMPONENT,
};

template<typename T>
class IBaseComponent : public BaseComponent {
public:

	static const uint32_t typeID;

	inline virtual uint32_t getComponentID() const {
		return typeID;
	};
};

#endif