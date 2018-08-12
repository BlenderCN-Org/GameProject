/// Internal Includes
#include "Debug.hpp"

/// External Includes
#include <EngineCore/Core/LibraryLoader.hpp>

/// Std Includes
#include <string>
#include <conio.h>

#if _DEBUG
Engine::Core::Library renderDoc;

#ifdef _WIN64 // 64 bit
const char* library = "C:/Program Files/RenderDoc/RenderDoc.dll";
#elif _WIN32 // 32 bit
const char* library = "C:/Program Files/RenderDoc/x86/RenderDoc.dll";
#endif

void loadRenderDocGpuDebugger() {

	printf("Press 1 to attach RenderDoc, any other key to ignore!\n");
	int c = _getch();
	if (c == '1') {
		if (renderDoc.loadLibrary(library)) {
			printf("RenderDoc successfully loaded!\nGPU debugging avalible!\n");
		} else {
			printf("Failed to load RenderDoc!\nGPU debugging not avaible!\n");
		}
	}
}

void unloadRenderDocGpuDebugger() {
	renderDoc.unloadLibrary();
}

#endif

