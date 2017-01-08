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

	for (int i = 0; i < getLogicalProcessorCount(); i++)
	{
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
	for (int i = 0; i < getLogicalProcessorCount(); i++)
	{
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

	if (coreIndex == -1 || coreIndex >= getLogicalProcessorCount()) {
		PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_LARGE, NULL, &counterVal);
	} else {
		PdhGetFormattedCounterValue(cpuCore[coreIndex], PDH_FMT_LARGE, NULL, &counterVal);
	}
	return (int)counterVal.largeValue;

}

#endif


/*
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void init(){
FILE* file = fopen("/proc/stat", "r");
fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
&lastTotalSys, &lastTotalIdle);
fclose(file);
}

double getCurrentValue(){
double percent;
FILE* file;
unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

file = fopen("/proc/stat", "r");
fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
&totalSys, &totalIdle);
fclose(file);

if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
totalSys < lastTotalSys || totalIdle < lastTotalIdle){
//Overflow detection. Just skip this value.
percent = -1.0;
}
else{
total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
(totalSys - lastTotalSys);
percent = total;
total += (totalIdle - lastTotalIdle);
percent /= total;
percent *= 100;
}

lastTotalUser = totalUser;
lastTotalUserLow = totalUserLow;
lastTotalSys = totalSys;
lastTotalIdle = totalIdle;

return percent;
}
*/