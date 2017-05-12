#ifndef TIMEALLOCATOR_HPP
#define TIMEALLOCATOR_HPP

#include <vector>
#include <iostream>

struct AllocObj
{
	float time;
	void* ptr;
};

class TimeAllocator
{
public:
	TimeAllocator(uint32_t size)
	{};
	~TimeAllocator()
	{
		for (size_t i = 0; i < allocatedObjects.size(); i++) {
			delete allocatedObjects[i].ptr;
			allocatedObjects.erase(allocatedObjects.begin() + i);
			i--;
		}
	};

	template<typename T>
	T* allocateTimedObject(float ttl) {

		AllocObj o;
		o.time = ttl;
		T* t = new T();
		o.ptr = t;

		allocatedObjects.push_back(o);
		return t;
	}

	void tick(float dt)
	{
		for (size_t i = 0; i < allocatedObjects.size(); i++)
		{
			allocatedObjects[i].time -= dt;
			if (allocatedObjects[i].time < 0.0f) {
				delete allocatedObjects[i].ptr;
				allocatedObjects.erase(allocatedObjects.begin() + i );
				i--;
			}
		}

	}

	std::vector<AllocObj> allocatedObjects;

};

#endif