#ifndef LIBRARYLOADER_HPP
#define LIBRARYLOADER_HPP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wchar.h>

#define LIBRARY HMODULE

class Lib {
public:

	bool loadLibrary(const char* libraryName);
	bool loadLibrary(const wchar_t* libraryName);

	void* getProcAddress(const char* procName);

	void unloadLibrary();
private:

	LIBRARY lib;
};

#endif