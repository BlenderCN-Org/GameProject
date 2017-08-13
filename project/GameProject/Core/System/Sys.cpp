// project includes
#include "Sys.hpp"
#include "../CoreGlobals.hpp"
#include "Console.hpp"
// project dependices

// std includes

#include <fstream>
#include <thread>

// os-specific includes

#ifdef _WIN32

#include <Pdh.h>

PDH_HQUERY cpuQuery;
PDH_HCOUNTER cpuTotal;
PDH_HCOUNTER* cpuCore;

#pragma comment(lib, "pdh.lib")

#endif

void initSys() {

	gConsole->print("Initializing System (Windows OS)\n");

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

#ifdef _WIN32

void pollCpuUsage() {
	PdhCollectQueryData(cpuQuery);
}

bool fileExists(const char * fileName) {
	std::ifstream infile(fileName);
	return infile.good();
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

const int32_t getStringLength(const char* str) {
	int32_t len = 0;
	if (str != nullptr) {
		while (str[len] != '\0') {
			len++;
		}
	}
	return len + 1;
}

#endif