/// Internal Includes
#include "Debug.hpp"
#include "Engine/Core/LibraryLoader.hpp"

/// External Includes

/// Std Includes
#include <string>
#include <conio.h>

#if _DEBUG
Engine::Core::Library renderDoc;

void loadRenderDocGpuDebugger() {

	printf("Press 1 to attach RenderDoc, any other key to ignore!\n");
	int c = _getch();
	if (c == 49) {
		
#ifdef _WIN64 // 64 bit
		if (renderDoc.loadLibrary("C:/Program Files/RenderDoc/RenderDoc.dll")) {
			printf("RenderDoc successfully loaded!\nGPU debugging avalible!\n");
		} else {
			printf("Failed to load RenderDoc!\nGPU debugging not avaible!\n");
		}
#elif _WIN32 // 32 bit
		if (renderDoc.loadLibrary("C:/Program Files/RenderDoc/x86/RenderDoc.dll")) {
			printf("RenderDoc successfully loaded!\nGPU debugging avalible!\n");
		} else {
			printf("Failed to load RenderDoc!\nGPU debugging not avaible!\n");
		}
#endif
	}
}

void unloadRenderDocGpuDebugger() {
	renderDoc.unloadLibrary();
}

#endif

