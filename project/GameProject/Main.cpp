
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
			//_tprintf(TEXT("\n     base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
			_tprintf(TEXT("\n     base size      = %d"), me32.modBaseSize);
			counter++;
		} while ( Module32Next(snapshot, &me32) );
	}

	printf("\n%d\n", counter);

	CloseHandle(snapshot);

	return 0;
}

void term_func() {
	int breakHere = 3;
}

LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
	std::ofstream f;
	f.open("VectoredExceptionHandler.txt", std::ios::out | std::ios::trunc);
	f << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
	f << pExceptionInfo->ExceptionRecord->ExceptionInformation << std::endl;
	f.close();

	return EXCEPTION_CONTINUE_SEARCH;
}

LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
	std::ofstream f;
	f.open("TopLevelExceptionHandler.txt", std::ios::out | std::ios::trunc);
	f << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
	f.close();

	return EXCEPTION_CONTINUE_SEARCH;
}

int main(int argc, char* argv[])
{
	set_terminate(term_func);

	AddVectoredExceptionHandler(1, VectoredExceptionHandler);
	SetUnhandledExceptionFilter(TopLevelExceptionHandler);

	Lib l;

	l.loadLibrary("RenderEngine.dll");

	printf("Loaded successfully..\n");

	float dt = 0.0f;
	float timePass = 0.0f;
	int fps = 0;
	unsigned int start = clock();

	Core* core = new Core();
		
	Input* input = Input::getInput();

	core->init();
	
	while ( core->isRunning() )
	{
		core->update(dt);
		
		core->render();
		
		unsigned int temp = clock();
		dt = unsigned int(temp - start) / 1000.0f;
		timePass += float(dt);
		start = temp;
		fps++;

		if (timePass > 1.0f)
		{
			timePass -= 1.0f;
			core->setFPS(fps);
			fps = 0;
		}
	}

	core->release();
	input->release();

	return 0;
}