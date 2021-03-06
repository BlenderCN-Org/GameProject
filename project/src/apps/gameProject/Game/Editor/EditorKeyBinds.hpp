#ifndef EDITOR_KEY_BINDS_HPP
#define EDITOR_KEY_BINDS_HPP

/// Internal Includes
#include "../../Engine/Input/Input.hpp"
#include "../../Engine/Input/ScanCodes.hpp"

/// External Includes

/// Std Includes

namespace EditorKeyBinds {
	const Engine::Input::KeyBind editorSave { ScanCodes::SCAN_CODE_S, ModifierKeys::MOD_KEY_CTRL, InputMode::KEYBOARD_INPUT };
	const Engine::Input::KeyBind editorOpen { ScanCodes::SCAN_CODE_O, ModifierKeys::MOD_KEY_CTRL, InputMode::KEYBOARD_INPUT };

	//const Engine::Input::KeyBind toolSelect { ScanCodes::SCAN_CODE_TAB, 0, InputMode::KEYBOARD_INPUT };

	const Engine::Input::InputMapping editorToolSelect = { ScanCodes::SCAN_CODE_TAB, Mouse::MOUSE_NONE, GamePad::BTN_BUMP_LEFT, MoveDirections::DIR_NONE };

}

#endif