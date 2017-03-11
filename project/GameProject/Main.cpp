#include "Game\Game.hpp"

#include <time.h>
#include <string>

#include "ExceptionHandling.hpp"

//#include <vld.h>

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	initExceptionHandlers();


	float dt = 0.0f;
	unsigned int start = clock();

	Game* g = new Game();
	g->init();

	while ( g->isRunning() ) {
		g->update(dt);
		g->render();

		unsigned int temp = clock();
		dt = unsigned int(temp - start) / 1000.0f;
		start = temp;
	}

	delete g;

	return 0;
}