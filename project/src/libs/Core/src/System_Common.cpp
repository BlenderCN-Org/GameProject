/// Internal Includes
//#include "Console.hpp"
#include "Core/System.hpp"
#include "System_os.hpp"

/// External Includes

/// Std Includes

#include <fstream>
#include <thread>
#include <algorithm>
#include <cstring>


namespace Engine {
	namespace System {

		void initSys() {
			osInitSys();
			srand((unsigned int)time(nullptr));
		}

		void deinitSys() {
			osDeinitSys();
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

		void pollCpuUsage() {
			osPollCpuUsage();
		}

		int getCoreUsage(int coreIndex = -1) {
			return osGetCoreUsage(coreIndex);
		}


		bool folderExists(const char* dirName) {
			return osFolderExists(dirName);
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
			return osListFiles(directoryName);
		}
	}
}
