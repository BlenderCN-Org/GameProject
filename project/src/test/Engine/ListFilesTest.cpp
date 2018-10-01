#include "../pch.h"

#include <EngineCore/Core/System.hpp>


TEST(Engine, listFiles) {

	char* dirName = "ListDir";
	
	std::vector<Engine::System::FileDesc> fileList = Engine::System::listFiles(dirName);
	
}

TEST(Engine, sortFilesTest) {

	
	std::vector<Engine::System::FileDesc> fileList;

	fileList.push_back(Engine::System::FileDesc({ "A", false }));
	fileList.push_back(Engine::System::FileDesc({ "B", false }));
	fileList.push_back(Engine::System::FileDesc({ "C", true }));
	fileList.push_back(Engine::System::FileDesc({ "D", true }));

	Engine::System::sortFiles(fileList, true);

	if (fileList[0].name != "C" && fileList[0].isDirectory != true) {
		FAIL();
	}

	if (fileList[1].name != "D" && fileList[0].isDirectory != true) {
		FAIL();
	}

	if (fileList[2].name != "A" && fileList[0].isDirectory != false) {
		FAIL();
	}

	if (fileList[3].name != "B" && fileList[0].isDirectory != false) {
		FAIL();
	}

	Engine::System::sortFiles(fileList, false);

	if (fileList[0].name != "A" && fileList[0].isDirectory != false) {
		FAIL();
	}

	if (fileList[1].name != "B" && fileList[0].isDirectory != false) {
		FAIL();
	}

	if (fileList[2].name != "C" && fileList[0].isDirectory != true) {
		FAIL();
	}

	if (fileList[3].name != "D" && fileList[0].isDirectory != true) {
		FAIL();
	}

}