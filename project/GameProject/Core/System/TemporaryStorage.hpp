#ifndef TEMPORARYSTORAGE_HPP
#define TEMPORARYSTORAGE_HPP

#include <vector>

#include "../CoreGlobals.hpp"
#include "Console.hpp"
#include <typeinfo>
#include <string>
#include <mutex>

template <class T>
std::string
type_name() {
	typedef typename std::remove_reference<T>::type TR;
	std::unique_ptr<char, void(*)(void*)> own
	(
#ifndef _MSC_VER
		abi::__cxa_demangle(typeid(TR).name(), nullptr,
			nullptr, nullptr),
#else
		nullptr,
#endif
		std::free
	);
	std::string r = own != nullptr ? own.get() : typeid(TR).name();
	if (std::is_const<TR>::value)
		r += " const";
	if (std::is_volatile<TR>::value)
		r += " volatile";
	if (std::is_lvalue_reference<T>::value)
		r += "&";
	else if (std::is_rvalue_reference<T>::value)
		r += "&&";
	return r;
}

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
		T* o = nullptr;
		lock.lock();
		{
			o = new T();

			TemporaryObject to;

			to.tempObjPtr = o;
			to.ftl = ftl;

			to.destruct = [](void* ptr) {
				T* t = (T*)ptr;

				//std::string name = type_name<decltype(t)>();
				//gConsole->print("%s\n", name.c_str());
				delete t;
			};

			tempList.push_back(to);
		}
		lock.unlock();
		return o;
	}

	template<typename T>
	inline T* allocateArray(unsigned int count, uint32_t ftl) {
		if (count == 0) {
			return nullptr;
		}
		T* o = nullptr;
		lock.lock();
		{
			o = new T[count];

			TemporaryObject to;

			to.tempObjPtr = o;
			to.ftl = ftl;

			to.destruct = [](void* ptr) {
				T* t = (T*)ptr;
				//std::string name = type_name<decltype(t)>();
				//gConsole->print("%s\n", name.c_str());
				delete[]t;
			};

			tempList.push_back(to);
		}
		lock.unlock();
		return o;
	}

	inline void tick() {
		if (lock.try_lock()) {
			if (tempList.size() != 0) {
				uint32_t start = tempList.size() - 1;
				for (uint32_t i = start; i > 0; i--) {
					TemporaryObject& to = tempList[i];

					if (to.ftl == 0) {
						if (to.tempObjPtr) {
							to.destruct(to.tempObjPtr);
							to.tempObjPtr = nullptr;
						}

						tempList.erase(tempList.begin() + i);
					} else {
						to.ftl--;
					}
				}
			}
			lock.unlock();
		}
	}

private:

	std::vector<TemporaryObject> tempList;
	std::mutex lock;
};

#endif