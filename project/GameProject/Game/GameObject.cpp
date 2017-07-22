#include "GameObject.hpp"

GameObject::~GameObject() {
	std::map<uint64_t, BaseComponent*>::iterator it = components.begin();
	std::map<uint64_t, BaseComponent*>::iterator eit = components.end();
	for (it; it != eit; it++) {
		BaseComponent* b = it->second;
		delete b;
	}
}

void GameObject::init() {}

void GameObject::addComponent(BaseComponent * component) {
	components[component->getComponentID()] = component;
	component->setAttachedOn(this);
}
