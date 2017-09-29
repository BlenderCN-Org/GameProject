#include "Game\Game.hpp"

#include <time.h>
#include <string>

#include "ExceptionHandling.hpp"

#include <Memory\MemoryMan.hpp>

//#include <vld.h>

void reset(Game*& g) {
	Sleep(5000);
	delete g;
	Sleep(5000);
	g = new Game();
	g->init();
}

int main(int argc, char* argv[]) {
	InitMemoryManagement();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	initExceptionHandlers();

	float dt = 0.0f;
	unsigned int start = clock();

	Game* g = new Game();
	g->init();

	while (g->isRunning()) {
		if (g->updateAndRender(dt) == false) {
			reset(g);
		}

		unsigned int temp = clock();
		dt = unsigned int(temp - start) / 1000.0f;
		start = temp;
	}

	delete g;

	ReleaseMemoryManagement();

	return 0;
}