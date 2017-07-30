#ifndef TEMPORARYSTORAGE_HPP
#define TEMPORARYSTORAGE_HPP

#include <vector>

#include "../CoreGlobals.hpp"
#include "Console.hpp"

struct TemporaryObject {
	uint32_t ftl;
	void* tempObjPtr;
	void(*destruct)(void* ptr);
};

class TemporaryStorage {

public:
	inline TemporaryStorage() :tempList() {}
	inline virtual ~TemporaryStorage() {

		std::vector<TemporaryObject>::iterator it = tempList.begin();
		std::vector<TemporaryObject>::iterator eit = tempList.end();

		for (it; it != eit; it++) {
			TemporaryObject to = *it;
			to.destruct(to.tempObjPtr);
		}

		tempList.clear();

	}

	template<typename T>
	inline T* allocate(uint32_t ftl) {
		T* o = new T();

		TemporaryObject to;

		to.tempObjPtr = o;
		to.ftl = ftl;

		to.destruct = [](void* ptr) {
			T* t = (T*)ptr;
			t->~T();
			free(ptr);
		};

		tempList.push_back(to);
		return o;
	}

	inline void tick() {

		for (uint32_t i = 0; i < tempList.size(); ) {
			
			TemporaryObject& to = tempList[i];
			if (to.ftl == 0) {
				to.destruct(to.tempObjPtr);

				tempList.erase(tempList.begin() + i);

			} else {
				to.ftl--;
				i++;
			}
		}
	}

private:

	std::vector<TemporaryObject> tempList;

};


#endif