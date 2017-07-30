#include "Game\Game.hpp"

#include <time.h>
#include <string>

#include "ExceptionHandling.hpp"

#include <Memory\MemoryMan.hpp>

//#include <vld.h>

int main(int argc, char* argv[]) {

	InitMemoryManagement();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	initExceptionHandlers();
	
	float dt = 0.0f;
	unsigned int start = clock();

	Game* g = new Game();
	g->init();

	while ( g->isRunning() ) {
		g->updateAndRender(dt);

		unsigned int temp = clock();
		dt = unsigned int(temp - start) / 1000.0f;
		start = temp;
	}

	delete g;
	
	ReleaseMemoryManagement();
	
	return 0;
}