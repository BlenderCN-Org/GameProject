#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

#include <stdint.h>
#include "IBaseComponent.hpp"

class RenderComponent : public IBaseComponent<RenderComponent>
{
public:

	void init();
	void setInstanceId(uint64_t instanceId);
	uint64_t getInstanceId() const;

private:

	uint64_t instanceId;

};

#endif