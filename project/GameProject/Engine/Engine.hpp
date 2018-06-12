#ifndef ENGINE_HPP
#define ENGINE_HPP 
/// Internal Includes
#include "AssetManager.hpp"
#include "ThreadManager.hpp"

#include "Core/LibraryLoader.hpp"
#include "Core/Console.hpp"
#include "Core/Settings.hpp"
#include "Interfaces/IAssetManager.hpp"
#include "Utils/MemoryBuffer.hpp"

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

	void setCursor(char* cursorImgPath);

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

	ThreadManager* getThreadManager();

private:

	Engine::Graphics::CGui* cursorGui;
	Engine::Graphics::Gui::Cursor* cursor;
	Engine::Graphics::Texture::Texture2D* cursorTexture;

	ThreadManager* threadManager;
	PhysicsEngine* physEngine;

	StaticObject* groundPlane;

	void physicsLoop();

	Core::Settings engineSettings;

	Core::Library renderEngineLib;
	Core::Console* console;
	IRenderEngine* renderEngine;
	IWindow* gameWindow;
	AssetManager* assetManager;

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