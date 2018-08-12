#ifndef LIBRARY_LOADER_HPP
#define LIBRARY_LOADER_HPP


/// Os Specific Includes
#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wchar.h>

#define LIBRARY HMODULE

#else
#define LIBRARY typedef struct Lib

#endif

namespace Engine {
	namespace Core {
		class Library {
		public:

			bool loadLibrary(const char* libraryName);
			bool loadLibrary(const wchar_t* libraryName);

			void* getProcAddress(const char* procName);

			void unloadLibrary();
		private:

			LIBRARY lib;
		};
	}
}

#endif