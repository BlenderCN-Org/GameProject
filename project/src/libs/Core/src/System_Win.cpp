#ifdef _WIN32
/// Internal Includes
#include "Core/System.hpp"
#include "System_os.hpp"

/// External Includes

/// Std Includes


#include <Pdh.h>
#include <strsafe.h>
#include <atlstr.h>

PDH_HQUERY cpuQuery;
PDH_HCOUNTER cpuTotal;
PDH_HCOUNTER* cpuCore;

#pragma comment(lib, "pdh.lib")


void osInitSys() {
	cpuCore = new PDH_HCOUNTER[Engine::System::getLogicalProcessorCount()];

	PdhOpenQuery(NULL, NULL, &cpuQuery);
	PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);

	for (int i = 0; i < Engine::System::getLogicalProcessorCount(); i++) {
		const int size = 256;
		const WCHAR *temp = L"\\Processor(%d)\\%s Processor Time";
		WCHAR c[size];
		swprintf_s(c, size, temp, i, "%");
		PdhAddCounter(cpuQuery, c, NULL, &cpuCore[i]);
	}

	PdhCollectQueryData(cpuQuery);
}

void osDeinitSys() {
	PdhRemoveCounter(cpuTotal);
	for (int i = 0; i < Engine::System::getLogicalProcessorCount(); i++) {
		PdhRemoveCounter(cpuCore[i]);
	}
	PdhCloseQuery(cpuQuery);

	delete cpuCore;
}


void osPollCpuUsage() {
	PdhCollectQueryData(cpuQuery);
}

int osGetCoreUsage(int coreIndex = -1) {
	PDH_FMT_COUNTERVALUE counterVal;

	if (coreIndex == -1 || coreIndex >= Engine::System::getLogicalProcessorCount()) {
		PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_LARGE, NULL, &counterVal);
	} else {
		PdhGetFormattedCounterValue(cpuCore[coreIndex], PDH_FMT_LARGE, NULL, &counterVal);
	}
	return (int)counterVal.largeValue;
}


bool osFolderExists(const char* dirName) {
	DWORD attribs = GetFileAttributesA(dirName);
	if (attribs == INVALID_FILE_ATTRIBUTES) {
		return false;
	}
	if (attribs & FILE_ATTRIBUTE_DIRECTORY) {
		return true;
	}
	return false;
}

std::vector<Engine::System::FileDesc> osListFiles(const char* directoryName) {

	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	size_t numConverted = 0;
	mbstowcs_s(&numConverted, szDir, directoryName, MAX_PATH);

	//StringCchCopy(szDir, MAX_PATH, szName);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

#ifdef _WIN64 // 64 bit
	// no need to do anything
#elif _WIN32 // 32 bit
	PVOID oldVal = NULL;
	Wow64DisableWow64FsRedirection(&oldVal);
#endif

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind) {
		throw;
	}

	// List all the files in the directory with some info about them.

	std::vector<Engine::System::FileDesc> fileList;
	Engine::System::FileDesc fd;

	do {

		std::mbstate_t state;
		char name[MAX_PATH];

		const wchar_t* ffdName = ffd.cFileName;
		numConverted = 0;
		wcsrtombs_s(&numConverted, name, &ffdName, MAX_PATH, &state);

		fd.name = name;
		fd.fileSize = Engine::System::makeUint64_t(ffd.nFileSizeHigh, ffd.nFileSizeLow);
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			fd.isDirectory = true;
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		} else {
			fd.isDirectory = false;
			//_tprintf(TEXT("  %s   <FILE>\n"), ffd.cFileName);
		}

		fileList.push_back(fd);

	} while (FindNextFile(hFind, &ffd) != 0);

#ifdef _WIN64 // 64 bit
	// no need to do anything
#elif _WIN32 // 32 bit
	Wow64RevertWow64FsRedirection(oldVal);
#endif

	if (FindClose(hFind) == 0) {
		printf("Failed to close search handle!");
	}

	sortFiles(fileList, true);

	return fileList;
}


#endif