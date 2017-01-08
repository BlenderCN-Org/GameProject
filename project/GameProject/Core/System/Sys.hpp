#ifndef SYS_HPP
#define SYS_HPP

// project includes

// project dependencies

// std includes


/*
*	System file
*	contains function for accessing os functions
*/

void initSys();
void deinitSys();
void pollCpuUsage();

int getLogicalProcessorCount();

int getCoreUsage(int coreIndex);

#endif