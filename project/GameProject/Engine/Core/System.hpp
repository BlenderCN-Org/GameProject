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

		template<typename T>
		float percentageInRange(T min, T max, T value) {

			float mmm = float(max - min);
			float vmm = float(value - min);

			return (vmm / mmm);
		}

	}
}

#endif