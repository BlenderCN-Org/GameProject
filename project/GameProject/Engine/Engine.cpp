/// Internal Includes

#include "Engine.hpp"
#include "Input/Input.hpp"
#include "Core/System.hpp"
#include "Graphics/Graphics.hpp"

CEngine::CEngine() {

	Engine::System::initSys();

	// Render Engine
	if (renderEngineLib.loadLibrary("RenderEngine.dll\0")) {
		//gConsole->print("Loaded RenderEngine.dll\n");
	} else {
		//gConsole->print("Failed to load Renderer\n");
		throw;
	}
	CreateRenderEngineProc rProc = (CreateRenderEngineProc)renderEngineLib.getProcAddress("CreateRenderEngine");

	windowWidth = 1280;
	windowHeight = 720;

	consoleTargetY = float(-(windowHeight / 2));
	consolePosY = float(-(windowHeight / 2));

	RenderEngineCreateInfo reci;
	reci.stype = SType::sRenderEngineCreateInfo;
	reci.createRenderWindow = true;
	reci.renderEngineType = RenderEngineType::eRenderOpenGL;
	reci.pNext = nullptr;

	renderEngine = rProc();
	renderEngine->init(reci);
	renderEngine->updateViewPort(windowWidth, windowHeight);

	gRenderEngine = renderEngine;

	gameWindow = renderEngine->getMainWindow();

	Input::Input* input = Input::Input::GetInput();

	input->setupCallbacks(gameWindow);

	gameWindow->setWindowSize(windowWidth, windowHeight);
	running = true;

	consoleGui = new Engine::Graphics::CGui();
	consolePanel = new Engine::Graphics::Gui::Panel();
	consoleHistory = new Engine::Graphics::Gui::Label();
	consoleText = new Engine::Graphics::Gui::Label();

	consoleBg = new Engine::Graphics::Texture::Texture2D();
	consoleBg->singleColor(0.192869F, 0.246195F, 0.168075F, 0.700000F);

	consolePanel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	consoleHistory->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	consoleText->setAnchorPoint(Engine::Graphics::GuiAnchor::BOTTOM);

	consoleGui->setVisible(true);
	consoleGui->setPosition(0, int(consolePosY));

	consolePanel->setPosition(0, 0);
	consolePanel->setSize(windowWidth, (windowHeight / 2));
	consolePanel->setVisible(true);
	consolePanel->setTexture(consoleBg);

	consoleHistory->setPosition(0, 0);
	consoleHistory->setSize(windowWidth, (windowHeight / 2) - 35);
	consoleHistory->setVisible(true);

	consoleText->setPosition(0, 0);
	consoleText->setSize(windowWidth, 35);
	consoleText->setVisible(true);

	consoleGui->addGuiItem(consolePanel);
	consolePanel->addGuiItem(consoleHistory);
	consolePanel->addGuiItem(consoleText);

	Core::FormattedString str = "Foobar";
	str.formatString(0, 6, glm::vec4(1, 0, 0, 0));

	str += " This is a test";

	consoleText->setText(str);

}

CEngine::~CEngine() {

	delete consoleBg;
	delete consoleGui;
	delete consoleHistory;
	delete consolePanel;
	delete consoleText;

	gRenderEngine = nullptr;

	Input::Input::Release();

	renderEngine->release();
	renderEngineLib.unloadLibrary();

	Engine::System::deinitSys();
}

void CEngine::registerDataParser(Interfaces::IDataParser* dataParser, DataParsersTypes parserType) {
	dataParsers[parserType].push_back(dataParser);
}

const bool CEngine::isRunning() const {
	return running;
}

void CEngine::update(const float dt) {
	Input::Input::GetInput()->reset();
	gameWindow->pollMessages();

	if (Input::Input::GetInput()->sizeChange) {
		int w = 0;
		int h = 0;
		Input::Input::GetInput()->getWindowSize(w, h);
		renderEngine->updateViewPort(w, h);
		consolePanel->setSize(w, (h / 2));
		consoleHistory->setSize(w, (h / 2) - 35);

		windowHeight = h;
		windowWidth = w;

	}

	if (Input::Input::GetInput()->consoleKeyWasPressed()) {
		Input::Input::GetInput()->toggleConsole();
		if (consoleTargetY == 0) {
			consoleTargetY = float(-(windowHeight / 2));
		} else {
			consoleTargetY = 0;
			console.setVisible(Input::Input::GetInput()->consoleIsActive());
		}
	}
	if (consolePosY != consoleTargetY) {
		if (consoleTargetY == 0) {
			consolePosY += 30;
			if (consolePosY > 0) {
				consolePosY = consoleTargetY;
			}
		} else {
			consolePosY -= 30;
			if (consolePosY < consoleTargetY) {
				consolePosY = consoleTargetY;
			}
		}

		consoleGui->setPosition(0, (int)consolePosY);
	} else {
		console.setVisible(Input::Input::GetInput()->consoleIsActive());
	}

	running = gameWindow->isVisible();
}

void CEngine::clearBackBuffer() {
	renderEngine->clear();
}

void CEngine::clearDebug() {
	renderEngine->renderDebugFrame();
}

void CEngine::presentFrame() {

	if (console.isVisible()) {
		// render console
		consoleGui->render();
	}
	gameWindow->swapBuffers();
}