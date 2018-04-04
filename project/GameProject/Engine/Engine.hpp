#ifndef ENGINE_HPP
#define ENGINE_HPP 
/// Internal Includes
#include "Core/LibraryLoader.hpp"
#include "Core/Console.hpp"
#include "Core/Settings.hpp"
#include "Interfaces/IDataParser.hpp"
#include "Interfaces/IAssetManager.hpp"
#include "Utils/MemoryBuffer.hpp"

#include "AssetManager.hpp"

#include "ThreadManager.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

#include <PhysicsEngine/PhysicsEngine.hpp>
#include <PhysicsEngine/Shapes/PlaneShape.hpp>
#include <PhysicsEngine/Shapes/SphereShape.hpp>

/// Std Includes
#include <map>
#include <vector>
#include <thread>

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

	void close();

	bool setAssetDataFolder(const char* folderPath);

	const bool isRunning() const;

	void update(const float dt);

	void lockCursor(bool lock);

	void clearBackBuffer();
	void clearDebug();
	void presentFrame();

	void writeDepth(float depthValue, glm::mat4 worldMat, glm::mat4 mdl);

	void renderFullQuad();

	Interfaces::IAssetManager* getAssetManager() const;

	PhysicsEngine* getPhysEngine();

private:

	ThreadManager* threadManager;
	PhysicsEngine physEngine;

	StaticObject* groundPlane;

	void physicsLoop();

	Core::Settings engineSettings;

	Core::Library renderEngineLib;
	Core::Console* console;
	IRenderEngine* renderEngine;
	IWindow* gameWindow;
	AssetManager* assetManager;

	std::map<DataParsersTypes, std::vector<Interfaces::IDataParser*>> dataParsers;

	int windowWidth;
	int windowHeight;

	bool pauseFlag;
	bool running;

	IShaderObject* depthWriteShader;
	int depthValueLoc;
	int depthVpMatLoc;
	int depthMdlMatLoc;

	IMesh* fullQuad;

	std::thread* physicsThread;

};

#endif