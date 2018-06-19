
/// Internal Includes
#include "Editor.hpp"
#include "../Engine/Input/Input.hpp"

/// External Includes

/// Std Includes

Editor::Editor()
	: mouseDownInGui(false)
	, pGameEditAccess(nullptr) {

	editorGui = new Engine::Graphics::CGui();

}

Editor::~Editor() {

	if (nullptr != pGameEditAccess) {
		delete pGameEditAccess;
	}

	delete objList;

	delete editorGui;

}

void Editor::start(IEdit* pEditClass, IMap** ppMap) {

	if (nullptr != pGameEditAccess) {
		delete pGameEditAccess;
	}
	pGameEditAccess = pEditClass;
	ppActiveMap = ppMap;

}

bool Editor::mouseInGui() {

	bool inGui = false;

	if (!pressing) {
		inGui = editorGui->mouseHitGui();
	}

	// force in gui while pressing down
	// to account for press move outside
	// this prevents moving the camera
	if (mouseDownInGui) {
		inGui = true;
	}

	Engine::Input::Input* in = Engine::Input::Input::GetInput();
	if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_MOUSE_L_CLICK]) == false) {
		mouseDownInGui = false;
		pressing = false;
	}
	if (in->wasPressedThisFrame(Engine::Input::KeyBindings[Engine::Input::KEYBIND_MOUSE_L_CLICK])) {
		if (inGui) {
			mouseDownInGui = true;
		}
		pressing = true;
	}

	return inGui;
}

void Editor::update(float dt) {
	editorGui->update(dt);

	Engine::Input::Input* in = Engine::Input::Input::GetInput();

	// @ TODO add check for moving around to not tirgger by accedent
	if (in->releasedThisFrame(EditorKeyBinds::editorSave)) {
		pGameEditAccess->save();
		printf("Saving changes\n");
	}

}

void Editor::render() {
	
}
