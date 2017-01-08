#ifdef compile

#undef APIENTRY

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <TlHelp32.h>

int PrintModules() {

	HANDLE snapshot;

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	MODULEENTRY32 me32 = {};

	me32.dwSize = sizeof(MODULEENTRY32);

	int counter = 0;

	if ( Module32First(snapshot, &me32) ) {
		do {
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

#endif