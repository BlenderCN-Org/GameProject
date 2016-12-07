
#include "Core\Core.hpp"
#include "Core\Input\Input.hpp"
#include "Core\Window\Window.hpp"

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
	float timePass = 0.0f;
	int fps = 0;
	unsigned int start = clock();

	Core* core = new Core();

	Input* input = Input::getInput();

	core->init();

	while ( core->isRunning() ) {
		if ( core->hadGraphicsReset() ) {
			core->release();
			core = new Core();
			core->init();
		}

		core->update(dt);

		core->render();

		unsigned int temp = clock();
		dt = unsigned int(temp - start) / 1000.0f;
		timePass += float(dt);
		start = temp;
		fps++;

		if ( timePass > 1.0f ) {
			timePass -= 1.0f;
			core->setFPS(fps);
			fps = 0;
		}
	}

	core->release();
	input->release();

	return 0;
}