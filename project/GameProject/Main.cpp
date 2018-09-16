
/// Internal Includes
#include "ExceptionHandling.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Input/Input.hpp"
#include "Engine/Graphics/Gui.hpp"
#include "Engine/Graphics/Gui/Panel.hpp"
#include "Engine/Graphics/Gui/Label.hpp"
#include "Engine/Graphics/Gui/ProgressBar.hpp"
#include "Engine/Graphics/Gui/ScrollBar.hpp"
#include "Game/Game.hpp"
#include "Debug.hpp"

#include <EngineCore/Core/LibraryLoader.hpp>

/// External Includes

/// Std Includes

typedef void(*fn)(ULONG);

void gameLoop() {
	float dt = 0;
	uint64_t clocks = 0U;
	Engine::System::HighResClock clock;

	CEngine* e = new CEngine();

	e->setAssetDataFolder("E:/GameProjectAssets/");
	
	Game* game = new Game(e);
	
	printf("starting loop\n");
	
	while (e->isRunning()) {
	
		game->update(dt, clocks);
		e->clearBackBuffer();
	
		game->render();
	
		e->presentFrame();
	
		clocks = clock.tick();
		dt = clock.seconds();
	}
	
	printf("finished execution\n");
	delete game;

	delete e;
}

int main(int argc, char* argv[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	initExceptionHandlers();

	loadRenderDocGpuDebugger();

	gameLoop();

	unloadRenderDocGpuDebugger();

	return 0;
}