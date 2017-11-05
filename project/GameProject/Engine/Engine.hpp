
/// Internal Includes
#include "Core/LibraryLoader.hpp"
#include "Interfaces/IDataParser.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Utils/MemoryBuffer.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <map>
#include <vector>

using namespace Engine;

enum class DataParsersTypes {
	MESH_PARSER,
	TEXURE_PARSER,
	// todo fill with more types
};

class CEngine {

public:

	CEngine();
	virtual ~CEngine();

	void registerDataParser(Interfaces::IDataParser* parser, DataParsersTypes parserType);

	//void testCallDataParsers() {
	//	std::map<DataParsersTypes, std::vector<Interfaces::IDataParser*>>::iterator it = dataParsers.begin();
	//	std::map<DataParsersTypes, std::vector<Interfaces::IDataParser*>>::iterator eit = dataParsers.end();

	//	for (it; it != eit; it++) {
	//		size_t size = it->second.size();

	//		for (size_t i = 0; i < size; i++) {
	//			Core::MemoryBuffer memBuff;
	//			it->second[i]->load(memBuff, nullptr);
	//		}
	//	}
	//}

	const bool isRunning() const;

	void update(const float dt);

	void clearBackBuffer();
	void clearDebug();
	void presentFrame();


private:

	Core::Library renderEngineLib;
	IRenderEngine* renderEngine;
	IWindow* gameWindow;

	std::map<DataParsersTypes, std::vector<Interfaces::IDataParser*>> dataParsers;

	Physics::PhysicsEngine physEngine;

	int windowWidth;
	int windowHeight;

	bool pauseFlag;
	bool running;
};