#ifndef SYSTEM_OS_HPP
#define SYSTEM_OS_HPP

#include <Core/System.hpp>

void osInitSys();
void osDeinitSys();


void osPollCpuUsage();
int osGetCoreUsage(int core);

bool osFolderExists(const char* dirName);

std::vector<Engine::System::FileDesc> osListFiles(const char* directoryName);

#endif