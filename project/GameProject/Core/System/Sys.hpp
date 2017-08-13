#ifndef SYS_HPP
#define SYS_HPP

// project includes

// project dependencies

// std includes
#include <cstdint>

/*
*	System file
*	contains function for accessing os functions
*/

void initSys();
void deinitSys();
void pollCpuUsage();

bool fileExists(const char* fileName);

int getLogicalProcessorCount();

int getCoreUsage(int coreIndex);

const int32_t getStringLength(const char* str);

#endif