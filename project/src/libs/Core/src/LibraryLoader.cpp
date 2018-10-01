
/// Internal Includes
#include "Core/LibraryLoader.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Core {
#ifdef _WIN32
		bool Library::loadLibrary(const char * libraryName) {
			lib = LoadLibraryA(libraryName);
			if (lib)
				return true;
			return false;
		}

		bool Library::loadLibrary(const wchar_t * libraryName) {
			lib = LoadLibraryW(libraryName);
			if (lib)
				return true;
			return false;
		}

		void * Library::getProcAddress(const char * procName) {
			return GetProcAddress(lib, procName);
		}

		void Library::unloadLibrary() {
			FreeLibrary(lib);
			lib = nullptr;
		}
#endif
	}
}