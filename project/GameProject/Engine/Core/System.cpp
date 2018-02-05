/// Internal Includes
#include "System.hpp"

/// External Includes

/// Std Includes
#include <fstream>
#include <thread>

// os-specific includes

namespace Engine {
	namespace System {

#ifdef _WIN32

#include <Pdh.h>

		PDH_HQUERY cpuQuery;
		PDH_HCOUNTER cpuTotal;
		PDH_HCOUNTER* cpuCore;

#pragma comment(lib, "pdh.lib")

#endif


		void initSys() {
			//gConsole->print("Initializing System (Windows OS)\n");

			srand((unsigned int)time(nullptr));

			cpuCore = new PDH_HCOUNTER[getLogicalProcessorCount()];

			PdhOpenQuery(NULL, NULL, &cpuQuery);
			PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);

			for (int i = 0; i < getLogicalProcessorCount(); i++) {
				const int size = 256;
				WCHAR *temp = L"\\Processor(%d)\\%s Processor Time";
				WCHAR c[size];
				swprintf_s(c, size, temp, i, "%");
				PdhAddCounter(cpuQuery, c, NULL, &cpuCore[i]);
			}

			PdhCollectQueryData(cpuQuery);
		}

		void deinitSys() {
			PdhRemoveCounter(cpuTotal);
			for (int i = 0; i < getLogicalProcessorCount(); i++) {
				PdhRemoveCounter(cpuCore[i]);
			}
			PdhCloseQuery(cpuQuery);

			delete cpuCore;
		}

		int getLogicalProcessorCount() {
			int c = std::thread::hardware_concurrency();

			return c;
		}

		bool fileExists(const char * fileName) {
			std::ifstream infile(fileName);
			return infile.good();
		}

		const size_t strlen_s(const char* str) {
			if (str == nullptr) {
				return 0U;
			} else {
				return strlen(str);
			}
		}

		uint64_t HighResClock::tick() {

			using namespace std::chrono;

			high_resolution_clock::time_point temp = high_resolution_clock::now();
			
			auto d = temp - start;
			start = temp;
			time = duration_cast<duration<float>>(d).count();

			return d.count();
		}

		float HighResClock::seconds() {
			return time;
		}

#ifdef _WIN32

		void pollCpuUsage() {
			PdhCollectQueryData(cpuQuery);
		}

		int getCoreUsage(int coreIndex = -1) {
			PDH_FMT_COUNTERVALUE counterVal;

			if (coreIndex == -1 || coreIndex >= getLogicalProcessorCount()) {
				PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_LARGE, NULL, &counterVal);
			} else {
				PdhGetFormattedCounterValue(cpuCore[coreIndex], PDH_FMT_LARGE, NULL, &counterVal);
			}
			return (int)counterVal.largeValue;
		}


		bool folderExists(const char* dirName) {
			DWORD attribs = GetFileAttributesA(dirName);
			if (attribs == INVALID_FILE_ATTRIBUTES) {
				return false;
			}
			if (attribs & FILE_ATTRIBUTE_DIRECTORY) {
				return true;
			}
			return false;
		}
		
#endif
	}
}
