//#include "Game\Game.hpp"

#include <time.h>
#include <string>

#include "ExceptionHandling.hpp"

#include <conio.h>

#include "Engine/Engine.hpp"
#include "Engine/Input/Input.hpp"
#include "Engine/Interfaces/IDataParser.hpp"
#include "Engine/Graphics/Gui.hpp"
#include "Engine/Graphics/Gui/Panel.hpp"
#include "Engine/Graphics/Gui/Label.hpp"
#include "Engine/Graphics/Gui/ProgressBar.hpp"
#include "Engine/Graphics/Gui/ScrollBar.hpp"

#include "Game/Game.hpp"

int main(int argc, char* argv[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#if _DEBUG
	Engine::Core::Library renderDoc;
	printf("Press 1 to attach RenderDoc, any other key to ignore!\n");
	int c = _getch();
	if (c == 49) {
		if (renderDoc.loadLibrary("C:/Program Files/RenderDoc/x86/RenderDoc.dll")) {
			printf("RenderDoc successfully loaded!\nGPU debugging avalible!\n");
		} else {
			printf("Failed to load RenderDoc!\nGPU debugging not avaible!\n");
		}
	}
#endif

	initExceptionHandlers();

	float dt = 0.0f;
	unsigned int start = clock();

	CEngine* e = new CEngine();

	e->setAssetDataFolder("E:/GameProjectAssets/");

	Engine::Input::Input* in = Engine::Input::Input::GetInput();

	uint32_t value = 500U;

	Game* game = new Game(e);

	printf("starting loop\n");

	
	while (e->isRunning()) {

		game->update(dt);
		e->clearBackBuffer();
		
		game->render();

		e->presentFrame();

		unsigned int temp = clock();
		dt = unsigned int(temp - start) / 1000.0f;
		start = temp;

		value++;
		if (value > 1000U) {
			value = 0U;
		}

	}
	
	printf("finished execution\n");
	delete game;

	delete e;
	renderDoc.unloadLibrary();

	return 0;
}