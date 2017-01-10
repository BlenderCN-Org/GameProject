#include "RenderComponent.hpp"

const uint64_t IBaseComponent<RenderComponent>::typeID = (uint64_t)Components::RENDER_COMPONENT;

void RenderComponent::init() {
	instanceId = 0;
}

void RenderComponent::setInstanceId(uint64_t _instanceId) {
	instanceId = _instanceId;
}

uint64_t RenderComponent::getInstanceId() const {
	return instanceId;
}

//uint64_t RenderComponent::getComponentID() const
//{
//	return 0;
//}