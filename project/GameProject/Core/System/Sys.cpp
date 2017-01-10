// project includes
#include "Sys.hpp"

// project dependices

// std includes

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
	cpuCore = new PDH_HCOUNTER[getLogicalProcessorCount()];

	PdhOpenQuery(NULL, NULL, &cpuQuery);
	PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);

	for ( int i = 0; i < getLogicalProcessorCount(); i++ ) {
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
	for ( int i = 0; i < getLogicalProcessorCount(); i++ ) {
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

int getCoreUsage(int coreIndex = -1) {
	PDH_FMT_COUNTERVALUE counterVal;

	if ( coreIndex == -1 || coreIndex >= getLogicalProcessorCount() ) {
		PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_LARGE, NULL, &counterVal);
	} else {
		PdhGetFormattedCounterValue(cpuCore[coreIndex], PDH_FMT_LARGE, NULL, &counterVal);
	}
	return (int)counterVal.largeValue;
}

#endif