#include "Core.hpp"
#include "CoreGlobals.hpp"

#include "System\Sys.hpp"

#include <AssetLib\AssetLib.hpp>

int Core::width = 0;
int Core::heigth = 0;

Core::~Core() {
	freeResources();
}

void Core::init() {
	initSys();

	running = true;
	hadReset = false;
	// Render Engine
	if (renderEngineLib.loadLibrary("RenderEngine.dll\0"))
		printf("Loaded RenderEngine.dll\n");
	else {
		printf("Failed to load Renderer\n");
		throw;
	}

	CreateRenderEngineProc rProc = (CreateRenderEngineProc)renderEngineLib.getProcAddress("CreateRenderEngine");

	width = 1280;
	heigth = 720;

	RenderEngineCreateInfo reci;
	reci.stype = SType::sRenderEngineCreateInfo;
	reci.createRenderWindow = true;
	reci.renderEngineType = RenderEngineType::eRenderOpenGL;
	reci.pNext = nullptr;

	renderEngine = rProc();
	renderEngine->init(reci);
	renderEngine->updateViewPort(1280, 720);

	window = renderEngine->getMainWindow();

	input = Input::getInput();

	input->setupCallbacks(window);

	// Editor

	if (editorLib.loadLibrary("Editor_Wrapper.dll\0")) {
		printf("Loaded Editor_Wrapper.dll\n");

		CreateEditorProc eProc = (CreateEditorProc)editorLib.getProcAddress("CreateEditor");
		if (eProc != nullptr) {
			editor = eProc();
			if (editor) {
				extHandler = new ExtensionHandler();
				extHandler->loadExtensions(editor);
			}
		}
	} else {
		printf("Failed to load Editor\n");
		//throw;
	}

	gCore = this;
	gRenderEngine = renderEngine;

}

void Core::freeResources() {

	if (editor) {
		extHandler->unloadExtension(editor);
		delete extHandler;
		editor->releaseEditor();
	}
	renderEngine->release();

	renderEngineLib.unloadLibrary();
	input->release();

	deinitSys();

	gCore = nullptr;
	gRenderEngine = nullptr;

}

bool Core::isRunning() {
	return running;
}

bool Core::hadGraphicsReset() const {
	return renderEngine->getGraphicsReset();
}

bool Core::editorAvaible() const {
	return (nullptr != editor);
}

bool Core::isInEditor() const {
	return editor->getStatus() == EditorStatus::RUNNING;
}

void Core::startEditor() {
	if (editorAvaible() && editor->initializeEditor(renderEngine)) {
		editor->setAssetManager((IAssetManager*)gAssetManager);
		editor->startEditor();
	}
}

void Core::stopEditor() {
	editor->stopEditor();
}

void Core::update(float dt) {
	// reset input states, clear for next frame
	input->reset();
	window->pollMessages();
	EditorStatus status = editor->getStatus();
	if (editor && status == EditorStatus::RUNNING || status == EditorStatus::HIDDEN) {
		editor->update();
	}
	// poll messages and update camera
	running = window->isVisible() || status == EditorStatus::RUNNING || status == EditorStatus::HIDDEN;

	// we are changing status
	if (currentEditorStatus != status) {

		switch (status) {
			case STARTING:
				break;
			case RUNNING:
				window->showWindow(false);
				input->setupCallbacks(editor->getEditorWindow());
				break;
			case HIDDEN:
				window->showWindow(true);
				input->setupCallbacks(window);
				break;
			case STOPPING:
				break;
			case STOPPED:
				break;
			default:
				break;
		}

		currentEditorStatus = status;
	}

	if (window->isVisible() == false && status == EditorStatus::HIDDEN) {
		stopEditor();
	}

	// window size change event
	if (input->sizeChange) {
		int w = 0;
		int h = 0;
		input->getWindowSize(w, h);

		renderEngine->updateViewPort(w, h);
		width = w;
		heigth = h;
	}

	if (input->consoleKeyWasPressed()) {
		input->toggleConsole();
	}

	hadReset = renderEngine->getGraphicsReset();
	if (hadReset) return;
}

void Core::render(glm::mat4 viewMat) {

	renderEngine->renderDebugFrame();
	hadReset = renderEngine->getGraphicsReset();
	if (hadReset) return;

	if (isInEditor()) {
		editor->postPixels(width, heigth);
	}

}

void Core::swap() {
	// swap
	window->swapBuffers();
}

IRenderEngine * Core::getRenderEngine() {
	return renderEngine;
}
