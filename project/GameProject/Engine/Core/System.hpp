#ifndef SYSTEM_HPP
#define SYSTEM_HPP

/// Internal Includes

/// External Includes

/// Std Includes
#include <cstdint>
#include <chrono>
#include <string>

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

		float randomFloat(float low, float high);

		template<typename T>
		float percentageInRange(T min, T max, T value) {

			float mmm = float(max - min);
			float vmm = float(value - min);

			return (vmm / mmm);
		}
		
		struct HighResClock {

			HighResClock() {
				start = std::chrono::high_resolution_clock::now();
				time = 0;
			}

			std::chrono::high_resolution_clock::time_point start;
			float time;

			uint64_t tick();
			float seconds();

		};

		std::string readShader(const char *filePath);

	}
}

#endif