#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Components/BaseComponent.hpp"

#include <map>


class GameObject {

public:

	virtual ~GameObject();

	void init();

	void addComponent(BaseComponent* component);

	template <typename T>
	BaseComponent* getComponent();

private:

	std::map<uint64_t, BaseComponent*> components;

};

template<typename T>
inline BaseComponent* GameObject::getComponent() {
	if (components.count(IBaseComponent<T>::typeID)) {
		return components.at(IBaseComponent<T>::typeID);
	}
	return nullptr;
}

#endif