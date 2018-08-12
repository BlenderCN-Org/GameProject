/// Internal Includes
//#include "Console.hpp"
#include "System.hpp"

/// External Includes

/// Std Includes

#include <fstream>
#include <thread>
#include <strsafe.h>
#include <atlstr.h>
#include <algorithm>

// os-specific includes

namespace Engine {
	namespace System {

#ifdef _WIN32

#include <Pdh.h>

		PDH_HQUERY cpuQuery;
		PDH_HCOUNTER cpuTotal;
		PDH_HCOUNTER* cpuCore;

#pragma comment(lib, "pdh.lib")

#endif


		void initSys() {
			//Engine::Core::gConsole->print("Initializing System (Windows OS)");

			srand((unsigned int)time(nullptr));

			cpuCore = new PDH_HCOUNTER[getLogicalProcessorCount()];

			PdhOpenQuery(NULL, NULL, &cpuQuery);
			PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);

			for (int i = 0; i < getLogicalProcessorCount(); i++) {
				const int size = 256;
				const WCHAR *temp = L"\\Processor(%d)\\%s Processor Time";
				WCHAR c[size];
				swprintf_s(c, size, temp, i, "%");
				PdhAddCounter(cpuQuery, c, NULL, &cpuCore[i]);
			}

			PdhCollectQueryData(cpuQuery);
		}

		void deinitSys() {
			PdhRemoveCounter(cpuTotal);
			for (int i = 0; i < getLogicalProcessorCount(); i++) {
				PdhRemoveCounter(cpuCore[i]);
			}
			PdhCloseQuery(cpuQuery);

			delete cpuCore;
		}

		int getLogicalProcessorCount() {
			int c = std::thread::hardware_concurrency();
			return c;
		}

		bool fileExists(const char * fileName) {
			std::ifstream infile(fileName);
			return infile.good();
		}

		const size_t strlen_s(const char* str) {
			if (str == nullptr) {
				return 0U;
			} else {
				return strlen(str);
			}
		}

		float randomFloat(float low, float high) {

			return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
		}

		uint64_t makeUint64_t(unsigned long high, unsigned long low) {
			uint64_t v = low;
			uint64_t v2 = high;
			v += (v2 << 32);
			return v;
		}

		uint64_t HighResClock::tick() {

			using namespace std::chrono;

			high_resolution_clock::time_point temp = high_resolution_clock::now();

			auto d = temp - start;
			start = temp;
			time = duration_cast<duration<float>>(d).count();

			return d.count();
		}

		float HighResClock::seconds() {
			return time;
		}

		std::string readShader(const char *filePath) {
			std::string content;
			std::ifstream fileStream(filePath, std::ios::in);

			if (!fileStream.is_open()) {
				//Debug::DebugOutput("Could not open file %s", filePath);
				return "";
			}

			std::string line = "";
			while (!fileStream.eof()) {
				std::getline(fileStream, line);
				content.append(line + "\n");
			}

			fileStream.close();
			return content;
		}

#ifdef _WIN32

		void pollCpuUsage() {
			PdhCollectQueryData(cpuQuery);
		}

		int getCoreUsage(int coreIndex = -1) {
			PDH_FMT_COUNTERVALUE counterVal;

			if (coreIndex == -1 || coreIndex >= getLogicalProcessorCount()) {
				PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_LARGE, NULL, &counterVal);
			} else {
				PdhGetFormattedCounterValue(cpuCore[coreIndex], PDH_FMT_LARGE, NULL, &counterVal);
			}
			return (int)counterVal.largeValue;
		}


		bool folderExists(const char* dirName) {
			DWORD attribs = GetFileAttributesA(dirName);
			if (attribs == INVALID_FILE_ATTRIBUTES) {
				return false;
			}
			if (attribs & FILE_ATTRIBUTE_DIRECTORY) {
				return true;
			}
			return false;
		}

		std::string removeDuplicateSlashes(std::string str) {

			std::string newStr = "";

			char last = '\0';
			for (size_t i = 0; i < str.size(); i++) {
				if (str[i] == last && last == '/') {
					continue;
				}
				last = str[i];
				newStr += last;
			}

			return newStr;
		}

		struct SortFile_S {
			SortFile_S(bool first) : dirFirst(first) {}
			bool operator () (FileDesc& i, FileDesc& j) {

				std::string s1 = " " + i.name;
				std::string s2 = " " + j.name;
				
				if (dirFirst) {
				
					if (i.isDirectory) {
						s1[0] = 'A';
					} else {
						s1[0] = 'Z';
					}
				
					if (j.isDirectory) {
						s2[0] = 'A';
					} else {
						s2[0] = 'Z';
					}
				} else {
					if (i.isDirectory) {
						s1[0] = 'Z';
					} else {
						s1[0] = 'A';
					}
				
					if (j.isDirectory) {
						s2[0] = 'Z';
					} else {
						s2[0] = 'A';
					}
				}
				
				return s1 < s2;

			}

			bool dirFirst;
		};


		void sortFiles(std::vector<FileDesc>& fileList, bool directoryFirst) {

			std::sort(fileList.begin(), fileList.end(), SortFile_S(directoryFirst));

		}

		std::vector<FileDesc> listFiles(const char* directoryName) {

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

			std::vector<FileDesc> fileList;
			FileDesc fd;

			do {

				std::mbstate_t state;
				char name[MAX_PATH];

				const wchar_t* ffdName = ffd.cFileName;
				numConverted = 0;
				wcsrtombs_s(&numConverted, name, &ffdName, MAX_PATH, &state);

				fd.name = name;
				fd.fileSize = makeUint64_t(ffd.nFileSizeHigh, ffd.nFileSizeLow);
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
	}
}
