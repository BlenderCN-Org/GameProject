#include "RenderComponent.hpp"

const uint32_t IBaseComponent<RenderComponent>::typeID = (uint32_t)Components::RENDER_COMPONENT;

void RenderComponent::init() {
	instanceId = 0;
}

void RenderComponent::setInstanceId(uint32_t _instanceId) {
	instanceId = _instanceId;
}

uint32_t RenderComponent::getInstanceId() const {
	return instanceId;
}