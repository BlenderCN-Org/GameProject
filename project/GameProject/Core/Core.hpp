#ifndef CORE_HPP
#define CORE_HPP

// project includes
#include "RenderEngine/IRenderEngine.hpp"
#include "Input/Input.hpp"
#include "System/LibraryLoader.hpp"

#include <Editor_Wrapper\IEditor.hpp>
#include "Extensions\ExtensionHandler.hpp"

class Core
{
public:

	~Core();

	void init();
	void freeResources();

	bool isRunning();
	bool hadGraphicsReset() const;

	bool editorAvaible() const;
	bool isInEditor() const;

	void startEditor();
	void stopEditor();

	void update(float dt);

	void render(glm::mat4 viewMat);
	void swap();

	IRenderEngine* getRenderEngine();
		
	static int width;
	static int heigth;

	EditorStatus currentEditorStatus;

private:

	// core variables
	Lib renderEngineLib;

	bool hadReset;
	bool running;
	int fps;

	IWindow* window;

	IRenderEngine* renderEngine;
	Input* input;

	ExtensionHandler* extHandler = nullptr;

	Lib editorLib;
	IEditor* editor;
};

#endif