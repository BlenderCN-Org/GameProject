#ifndef BASECOMPONENT_HPP
#define BASECOMPONENT_HPP

#include <stdint.h>

class GameObject;

class BaseComponent
{
public:

	virtual uint64_t getComponentID() const = 0;
	void setAttachedOn(GameObject* owner);
private:

	GameObject* attatchedOn;
};

#endif