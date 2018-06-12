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

	Engine::Interfaces::IAssetManager* getAssetManager() const;

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

	Engine::Core::Settings engineSettings;

	Engine::Core::Library renderEngineLib;
	Engine::Core::Console* console;
	IRenderEngine* renderEngine;
	IWindow* gameWindow;
	Engine::AssetManager* assetManager;

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