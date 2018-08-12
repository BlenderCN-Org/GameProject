#ifndef GUI_INPUT_MAPPING_HPP
#define GUI_INPUT_MAPPING_HPP

#include "../Input/Input.hpp"
#include "../Input/ScanCodes.hpp"

namespace GuiInput {

	Engine::Input::InputMapping INPUT_BACK = { ScanCodes::SCAN_CODE_ESC, Mouse::MOUSE_NONE, GamePad::BTN_B, MoveDirections::DIR_NONE };
	Engine::Input::InputMapping INPUT_DELETE = { ScanCodes::SCAN_CODE_BCKSPACE, Mouse::MOUSE_NONE, GamePad::BTN_X, MoveDirections::DIR_NONE };
	Engine::Input::InputMapping INPUT_ENTER = { ScanCodes::SCAN_CODE_ENTER, Mouse::MOUSE_NONE, GamePad::BTN_A, MoveDirections::DIR_NONE };

	Engine::Input::InputMapping INPUT_UP = { ScanCodes::SCAN_CODE_NONE, Mouse::MOUSE_SCROLL_1, GamePad::BTN_D_UP, MoveDirections::DIR_POS };
	Engine::Input::InputMapping INPUT_DOWN = { ScanCodes::SCAN_CODE_NONE, Mouse::MOUSE_SCROLL_1, GamePad::BTN_D_DOWN, MoveDirections::DIR_NEG };

	Engine::Input::InputMapping INPUT_LEFT = { ScanCodes::SCAN_CODE_NONE, Mouse::MOUSE_NONE, GamePad::BTN_D_LEFT, MoveDirections::DIR_NONE };
	Engine::Input::InputMapping INPUT_RIGHT = { ScanCodes::SCAN_CODE_NONE, Mouse::MOUSE_NONE, GamePad::BTN_D_RIGHT, MoveDirections::DIR_NONE };

}

#endif