#ifndef TEMPORARY_STORAGE_HPP
#define TEMPORARY_STORAGE_HPP

/// Internal Includes

/// External Includes

/// Std Includes
#include <memory>

#define KB 1024
#define MB KB*KB

class TemporaryStorage {

public:

	struct SnapShot {
		size_t currentUsed;
		size_t oldUsed;
	};

	TemporaryStorage(size_t maxSize)
		: current()
	{

		totalSize = maxSize;
		usedSize = 0U;

		tempStorage = malloc(maxSize);

	}
	virtual ~TemporaryStorage() {
		free(tempStorage);
	};

	size_t getUsed() const {
		return usedSize;
	}

	void clear() {
		current.currentUsed = 0;
		current.oldUsed = 0;

		usedSize = 0;
	}

	SnapShot takeSnapshot() {
		return current;
	}

	void restore(SnapShot) {



	}

	template<typename T, typename... Args>
	T* allocate(size_t count, Args ...args) {

		T* t = nullptr;

		size_t dataSize = sizeof(T) * count;

		char* arr = (char*)tempStorage;

		t = (T*)&arr[usedSize];

		usedSize += dataSize;

		for (size_t i = 0; i < count; i++) {

			new (&t[i]) T(args...);
		}


		return t;
	}

	template<typename T>
	T* allocateNoNew(size_t count) {

		T* t = nullptr;

		size_t dataSize = sizeof(T) * count;

		char* arr = (char*)tempStorage;

		t = (T*)&arr[usedSize];

		usedSize += dataSize;

		return t;
	}


private:

	size_t totalSize;
	size_t usedSize;

	SnapShot current;

	void* tempStorage;

};

#endif