#ifndef SYSTEM_HPP
#define SYSTEM_HPP

/// Internal Includes

/// External Includes

/// Std Includes
#include <cstdint>

/*
*	System file
*	contains function for accessing os functions
*/

namespace Engine {
	namespace System {
		void initSys();
		void deinitSys();
		void pollCpuUsage();

		bool fileExists(const char* fileName);
		bool folderExists(const char* dirName);

		int getLogicalProcessorCount();

		int getCoreUsage(int coreIndex);

		const size_t strlen_s(const char* str);

		float percentageInRange(uint32_t min, uint32_t max, uint32_t value);
		float percentageInRange(size_t min, size_t max, size_t value);

	}
}

#endif