#include "LibraryLoader.hpp"

bool Lib::loadLibrary(const char * libraryName)
{
	lib = LoadLibraryA(libraryName);
	if ( lib )
		return true;
	return false;
}

bool Lib::loadLibrary(const wchar_t * libraryName)
{
	lib = LoadLibraryW(libraryName);
	if ( lib )
		return true;
	return false;
}

void * Lib::getProcAddress(const char * procName)
{
	return GetProcAddress(lib, procName);
}


void Lib::unloadLibrary()
{
	FreeLibrary(lib);
	lib = nullptr;
}
