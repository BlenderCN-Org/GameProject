
#include <GLFW\glfw3.h>
#include "Core\Core.hpp"
#include "Core\Input.hpp"
#include "Core\Window\Window.hpp"

#include <time.h>
#include <string>

#include <vld.h>

#undef APIENTRY

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <TlHelp32.h>


int PrintModules()
{
	
	HANDLE snapshot;

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	MODULEENTRY32 me32 = {};

	me32.dwSize = sizeof(MODULEENTRY32);

	int counter = 0;

	if ( Module32First(snapshot, &me32) )
	{
		do
		{
			_tprintf(TEXT("\n\n     MODULE NAME:     %s"), me32.szModule);
			_tprintf(TEXT("\n     executable     = %s"), me32.szExePath);
			_tprintf(TEXT("\n     process ID     = 0x%08X"), me32.th32ProcessID);
			_tprintf(TEXT("\n     ref count (g)  =     0x%04X"), me32.GlblcntUsage);
			_tprintf(TEXT("\n     ref count (p)  =     0x%04X"), me32.ProccntUsage);
			_tprintf(TEXT("\n     base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
			_tprintf(TEXT("\n     base size      = %d"), me32.modBaseSize);
			counter++;
		} while ( Module32Next(snapshot, &me32) );
	}

	printf("\n%d\n", counter);

	CloseHandle(snapshot);

	return 0;
}

int main(int argc, char* argv[])
{
	glfwInit();
	float dt = 0.0f;
	float timePass = 0.0f;
	int fps = 0;
	unsigned int start = clock();

	Window* window = new Window();

	Core* core = new Core();
		
	Input* input = Input::getInput();
	//input->setupCallbacks(window->wnd);

	window->makeCurrent();
	
	core->init();

	core->getDisplaySettings()->setWindow(window->wnd);
	core->getDisplaySettings()->apply();

	window->showWindow();
	window->swapInterval(-1);
	
	while ( !window->shouldClose())
	{
		//window->makeCurrent();
		core->update(dt);
		glfwPollEvents();
		if ( core->hadGraphicsReset() )
		{
			while ( core->hadGraphicsReset() );
			core->release();
			delete window;

			glfwTerminate();
			glfwInit();

			window = new Window();
			core = new Core();

			//input->setupCallbacks(window->wnd);

			window->makeCurrent();

			core->init();

			core->getDisplaySettings()->setWindow(window->wnd);
			core->getDisplaySettings()->apply();

			window->showWindow();
			window->swapInterval(1);

		}
		double time = glfwGetTime();
		core->render();
		time = glfwGetTime() - time;

		window->swapBuffers();
		
		unsigned int temp = clock();
		dt = unsigned int(temp - start) / 1000.0f;
		timePass += float(dt);
		start = temp;
		fps++;

		if (timePass > 1.0f)
		{
			timePass -= 1.0f;
			window->setTitle(std::string(std::to_string(fps) + " : " + std::to_string(time)).c_str());
			core->setFPS(fps);
			fps = 0;
		}
	}
	
	//PrintModules();

	core->release();
	input->release();

	delete window;

	glfwTerminate();

	return 0;
}