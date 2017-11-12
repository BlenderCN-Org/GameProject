#ifndef ENGINE_HPP
#define ENGINE_HPP 
/// Internal Includes
#include "Core/LibraryLoader.hpp"
#include "Core/Console.hpp"
#include "Interfaces/IDataParser.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Utils/MemoryBuffer.hpp"
#include "Graphics/Gui.hpp"
#include "Graphics/Gui/Panel.hpp"
#include "Graphics/Gui/Label.hpp"
#include "Graphics/Textures/Texture2D.hpp"

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
	Core::Console console;
	IRenderEngine* renderEngine;
	IWindow* gameWindow;

	std::map<DataParsersTypes, std::vector<Interfaces::IDataParser*>> dataParsers;

	Physics::PhysicsEngine physEngine;

	int windowWidth;
	int windowHeight;

	bool pauseFlag;
	bool running;

	Engine::Graphics::CGui* consoleGui;
	Engine::Graphics::Gui::Panel* consolePanel;
	Engine::Graphics::Gui::Label* consoleHistory;
	Engine::Graphics::Gui::Label* consoleText;

	Engine::Graphics::Texture::Texture2D* consoleBg;

	float consolePosY;
	float consoleTargetY;

};

#endif