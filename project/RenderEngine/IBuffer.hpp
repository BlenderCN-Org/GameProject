#ifndef I_BUFFER_HPP
#define I_BUFFER_HPP

#include <cstdint>

class IBuffer {

public:

	/*
	lock access to this object
	from calling thread
	returns true if lock was aquired or if current thread have already requested access.
	returns false if lock is already aquired by another thread
	*/
	virtual bool lock() = 0;

	/*
	unlock access to object
	if current thread is not main rendering thread
	rendering thread will issue commands between lock and unlock in order.
	*/
	virtual void unlock() = 0;

	/*
	lock must be aquired before calling any of these functions
	*/
	virtual uint32_t getDataSize() const = 0;
	virtual void* getDataPtr() const = 0;
	virtual void setData(uint32_t dataSize, void* data) = 0;

};

#endif