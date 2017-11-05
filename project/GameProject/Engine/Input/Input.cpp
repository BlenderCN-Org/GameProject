/// Internal Includes
#include "Input.hpp"

///External Includes

/// Std Includes
#include <iostream>

namespace Engine {
	namespace Input {

		Input* Input::singleton = nullptr;
		KeyBind KeyBindings[KEYBINDS_NAME::KEYBINDS_LENGTH];

		void Input::keyCallback(IWindow* window, int scancode, int action, int mods) {
			if (!singleton->consoleActive && action == 2) {
				return;
			}

			//singleton->keyMap[InputEvent{ scancode, false }] = (action == ACTION_BUTTON_DOWN);

			singleton->modkey = mods;
			if (singleton->consoleActive) {
				if ((scancode == 28 || scancode == 284) && action == ACTION_BUTTON_DOWN) {
					//singleton->console->print("\n>");
					//gConsole->execute();
				} else if (scancode == 14 && (action == ACTION_BUTTON_DOWN || action == 2)) {
					//gConsole->removeLastChar();
					//singleton->console->backSpace();
				}
				printf("Scancode %d with modkey %d\n", scancode, mods);
			}

			if (action == ACTION_BUTTON_UP) {
				singleton->releaseMap[InputEvent{ scancode, false }] = true;
			} else if (action == ACTION_BUTTON_DOWN) {
				singleton->pressedMap[InputEvent{ scancode, false }] = true;
			}
		}

		void Input::mouseButtonCallback(IWindow* window, int button, int action, int mods) {
			if (!singleton->consoleActive && action == 2) {
				return;
			}

			//printf("Mouse callback\n");

			//singleton->keyMap[InputEvent{ button, true }] = (action == ACTION_BUTTON_DOWN);
			if (action == ACTION_BUTTON_UP) {
				singleton->releaseMap[InputEvent{ button, true }] = true;
			} else if (action == ACTION_BUTTON_DOWN) {
				singleton->pressedMap[InputEvent{ button, true }] = true;
			}
			//printf("MouseButton %d with modkey %d\n", button, mods);
		}

		void Input::cursorPosCallback(IWindow * window, int x, int y) {
			if (singleton->blockInput) {
				return;
			}
			singleton->xDelta = float(x) - float(singleton->oldX);
			singleton->yDelta = float(y) - float(singleton->oldY);

			singleton->xPos = int(x);
			singleton->yPos = int(y);

			singleton->blockInput = true;
			//printf("Pos (%d,%d)\n", int(x), int(y));
			//printf("Delta (%f,%f)\n", singleton->xDelta, singleton->yDelta);
		}

		void Input::scrollCallback(IWindow * window, int xoffset, int yoffset) {
			singleton->scrollX = float(xoffset);
			singleton->scrollY = float(yoffset);

			//printf("Scroll (%f,%f)\n", float(xoffset), float(yoffset));
		}

		void Input::characterCallback(IWindow * window, unsigned int codepoint) {
			//if (singleton->consoleActive && gConsole) {
			//	//printf("%c", codepoint);
			//	gConsole->putChar(codepoint);
			//	//singleton->console->keyPress(codepoint);
			//}
		}

		void Input::sizeCallback(IWindow * window, int w, int h) {
			singleton->winW = w;
			singleton->winH = h;
			singleton->sizeChange = true;
			//printf("%d,%d\n", w, h);
			//glViewport(0, 0, w, h);
		}

		void Input::focusCallback(IWindow * window, bool focus) {
			singleton->focus = focus;
			//printf("focus %d\n", focus);
			if (focus == false) {
				singleton->keyMap.clear();
				//printf("Clearing key mappings\n");
			}
		}

		// todo decide if this is good or bad
		void Input::mouseDeltaCallback(IWindow * window, float dx, float dy) {
			//singleton->xDelta = dx * 4.4f;
			//singleton->yDelta = dy * 4.4f;

			//printf("Delta (%f,%f)\n", dx, dy);
		}

		Input* Input::GetInput() {
			if (singleton == nullptr)
				singleton = new Input();
			return singleton;
		}

		void Input::Release() {
			if (singleton != nullptr) {
				singleton->saveKeyBinds();
				singleton->keyConf.write("Config/keybinds.ini");
				delete singleton;
			}
			singleton = nullptr;
		}

		//void Input::attachConsole(Console * con) {
		//	console = con;
		//}

		void Input::clearCallbacks() {
			if (window) {
				window->setWindowKeyboardCallback(nullptr);
				window->setWindowMouseButtonCallback(nullptr);
				window->setWindowMouseMoveCallback(nullptr);
				window->setWindowScrollCallback(nullptr);
				window->setWindowCharacterCallback(nullptr);
				window->setWindowResizeCallback(nullptr);

				window->setWindowFocusCallback(focusCallback);
			}
		}

		void Input::setupCallbacks(IWindow * wnd) {
			clearCallbacks();
			window = wnd;

			window->setWindowKeyboardCallback(keyCallback);
			window->setWindowMouseButtonCallback(mouseButtonCallback);
			window->setWindowMouseMoveCallback(cursorPosCallback);
			window->setWindowScrollCallback(scrollCallback);
			window->setWindowCharacterCallback(characterCallback);
			window->setWindowResizeCallback(sizeCallback);

			window->setWindowFocusCallback(focusCallback);

			//window->setWindowMouseDeltaCallback(mouseDeltaCallback);
		}

		bool Input::isKeyBindPressed(const KeyBind & keyBind, bool includeMods) {
			auto it = keyMap.find(InputEvent{ keyBind.code, keyBind.mouse });
			bool pressed = false;
			if (it != keyMap.end()) {
				pressed = it->second;
				if (includeMods) {
					pressed &= (keyBind.mod == modkey);
				}
			}
			return pressed & focus;
		}

		bool Input::releasedThisFrame(const KeyBind & keyBind, bool includeMods) {
			auto it = releaseMap.find(InputEvent{ keyBind.code, keyBind.mouse });
			bool released = false;

			if (it != releaseMap.end()) {
				released = it->second;
				if (includeMods) {
					released &= (keyBind.mod == modkey);
				}
			}
			return released & focus;
		}

		bool Input::wasPressedThisFrame(const KeyBind & keyBind, bool includeMods) {
			auto it = pressedMap.find(InputEvent{ keyBind.code, keyBind.mouse });
			bool pressed = false;

			if (it != pressedMap.end()) {
				pressed = it->second;
				bool isHeldDown = isKeyBindPressed(keyBind, includeMods);
				if (isHeldDown) {
					pressed = false;
				}
				if (includeMods) {
					pressed &= (keyBind.mod == modkey);
				}
			}
			return pressed & focus;
		}

		void Input::getCursorDelta(float & x, float & y) {
			x = xDelta;
			y = yDelta;
		}

		bool Input::consoleKeyWasPressed() {
			auto& it = keyMap.find(InputEvent{ 41, false });
			//keyMap[InputEvent{ 41, false }] = false;
			bool pressed = false;
			if (it != keyMap.end()) {
				pressed = it->second;
				it->second = false;
			}
			return pressed;
		}

		void Input::toggleConsole() {
			//printf("Console toggle\n");
			/*if (consoleActive) {
				console->backSpace();
			}*/

			consoleActive = !consoleActive;
		}

		bool Input::consoleIsActive() {
			return consoleActive;
		}

		void Input::reset() {
			//printf("Pos (%d,%d)\n", int(xPos), int(yPos));
			//printf("Delta (%f,%f)\n", singleton->xDelta, singleton->yDelta);
			blockInput = false;
			xDelta = 0.0f;
			yDelta = 0.0;
			scrollX = 0.0;
			scrollY = 0.0;

			sizeChange = false;
			oldX = xPos;
			oldY = yPos;

			std::map<InputEvent, bool>::iterator it = pressedMap.begin();
			std::map<InputEvent, bool>::iterator eit = pressedMap.end();

			for (it; it != eit; it++) {
				keyMap[it->first] = it->second;
			}

			it = releaseMap.begin();
			eit = releaseMap.end();

			for (it; it != eit; it++) {
				if (it->second) {
					keyMap[it->first] = false;
				}
			}

			releaseMap.clear();
			pressedMap.clear();
		}

		void Input::getWindowSize(int & w, int & h) {
			w = winW;
			h = winH;
		}

		void Input::getMousePos(int & x, int & y) {
			window->getCursorPos(x, y);
		}

		void Input::getState(int &mx, int &my, int &mb, int &sc) {
			mx = xPos;
			my = yPos;

			if (keyMap[InputEvent{ 0, true }] == true)
				mb += 1;
			if (keyMap[InputEvent{ 1, true }] == true)
				mb += 2;
			if (keyMap[InputEvent{ 2, true }] == true)
				mb += 4;
			if (keyMap[InputEvent{ 3, true }] == true)
				mb += 8;
			if (keyMap[InputEvent{ 4, true }] == true)
				mb += 16;
			sc = (int)scrollY;
		}

		void Input::print() {
			// show content:
			//for ( std::map<InputEvent, bool>::iterator it = keyMap.begin(); it != keyMap.end(); ++it )
			//	std::cout << it->first.code << " " << it->first.mouse << " => " << it->second << '\n';
			if (!releaseMap.empty())
				for (std::map<InputEvent, bool>::iterator it = releaseMap.begin(); it != releaseMap.end(); ++it)
					std::cout << it->first.code << " " << it->first.mouse << " => " << it->second << '\n';
		}

		Input::Input() {
			consoleActive = false;
			window = nullptr;
			keyMap.clear();
			modkey = 0;

			xPos = 0;
			yPos = 0;

			xDelta = 0.0f;
			yDelta = 0.0f;

			scrollX = 0.0f;
			scrollY = 0.0f;

			keyConf.read("Config/keybinds.ini");
			loadkeyBinds();

			winH = winW = 0;
			sizeChange = false;
			focus = true;
		}

		void Input::loadkeyBinds() {
			//std::string bindingData = keyConf.getString("", KeyBindsStrings[KEYBIND_FORWARD], "17:0:0");
			//KeyBindings[KEYBINDS_NAME::KEYBIND_FORWARD] = KeyBind::create(bindingData);
			//
			//bindingData = keyConf.getString("", KeyBindsStrings[KEYBIND_BACKWARD], "31:0:0");
			//KeyBindings[KEYBINDS_NAME::KEYBIND_BACKWARD] = KeyBind::create(bindingData);
			//
			//bindingData = keyConf.getString("", KeyBindsStrings[KEYBIND_LEFT], "30:0:0");
			//KeyBindings[KEYBINDS_NAME::KEYBIND_LEFT] = KeyBind::create(bindingData);
			//
			//bindingData = keyConf.getString("", KeyBindsStrings[KEYBIND_RIGHT], "32:0:0");
			//KeyBindings[KEYBINDS_NAME::KEYBIND_RIGHT] = KeyBind::create(bindingData);

			std::string bindingData = "";
			for (size_t i = 0; i < KEYBINDS_NAME::KEYBINDS_LENGTH; i++) {
				bindingData = keyConf.getString("", KeyBindsStrings[i], KeyBindsDefault[i]);
				KeyBindings[i] = KeyBind::create(bindingData);
			}
		}

		void Input::saveKeyBinds() {
			for (size_t i = 0; i < KEYBINDS_NAME::KEYBINDS_LENGTH; i++) {
				keyConf.addString("", KeyBindsStrings[i], KeyBindings[i].toString());
			}
			//keyConf.addString("", KeyBindsStrings[KEYBIND_FORWARD], KeyBindings[KEYBINDS_NAME::KEYBIND_FORWARD].toString());
			//keyConf.addString("", KeyBindsStrings[KEYBIND_BACKWARD], KeyBindings[KEYBINDS_NAME::KEYBIND_BACKWARD].toString());
			//keyConf.addString("", KeyBindsStrings[KEYBIND_LEFT], KeyBindings[KEYBINDS_NAME::KEYBIND_LEFT].toString());
			//keyConf.addString("", KeyBindsStrings[KEYBIND_RIGHT], KeyBindings[KEYBINDS_NAME::KEYBIND_RIGHT].toString());
		}
	}
}