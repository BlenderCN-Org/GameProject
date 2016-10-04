#include "GameObject.hpp"

void GameObject::init()
{
}

void GameObject::update(float dt)
{
}

void GameObject::addComponent(BaseComponent* component)
{
	components[component->getComponentID()] = component;
	component->setAttachedOn(this);
}
