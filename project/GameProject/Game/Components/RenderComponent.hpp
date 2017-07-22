#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

#include <stdint.h>
#include "IBaseComponent.hpp"
#include <RenderEngine/IMesh.hpp>

class RenderComponent : public IBaseComponent<RenderComponent>
{
public:

	void init();
	void setInstanceId(uint32_t instanceId);
	uint32_t getInstanceId() const;

private:

	uint32_t instanceId;
};

#endif