//#include "Game\Game.hpp"

#include <time.h>
#include <string>

#include "ExceptionHandling.hpp"

#include <Memory\MemoryMan.hpp>

#include <conio.h>

#include "Engine/Engine.hpp"
#include "Engine/Input/Input.hpp"
#include "Engine/Interfaces/IDataParser.hpp"
#include "Engine/Graphics/Gui.hpp"
#include "Engine/Graphics/Gui/Panel.hpp"
#include "Engine/Graphics/Gui/Label.hpp"
#include "Engine/Graphics/Gui/ProgressBar.hpp"

#include "Game/Game.hpp"

//#include <vld.h>

//void reset(Game*& g) {
//	Sleep(5000);
//	delete g;
//	Sleep(5000);
//	g = new Game();
//	g->init();
//}

/*Game* g = new Game();
g->init();

while (g->isRunning()) {
if (g->updateAndRender(dt) == false) {
reset(g);
}

unsigned int temp = clock();
dt = unsigned int(temp - start) / 1000.0f;
start = temp;
}

delete g;*/

int main(int argc, char* argv[]) {
	InitMemoryManagement();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	printf("Waiting for RenderDoc injection, Press any key to continue...\n");
	_getch();

	initExceptionHandlers();

	float dt = 0.0f;
	unsigned int start = clock();

	CEngine* e = new CEngine();

	Engine::Input::Input* in = Engine::Input::Input::GetInput();

	uint32_t value = 500U;

	Game game(e);

	printf("starting loop\n");

	while (e->isRunning()) {

		if (in->wasPressedThisFrame(Engine::Input::KeyBindings[Engine::Input::KEYBIND_ENTER], false)) {
			printf("Enter was pressed this frame\n");
		}
		if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_FORWARD], false)) {
			printf("Forward is begin pressed\n");
		}

		game.update(dt);
		//e->clearBackBuffer();
		e->clearDebug();
		game.render();

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

	delete e;
	ReleaseMemoryManagement();

	return 0;
}