#ifndef INPUT_HPP
#define INPUT_HPP

#include <RenderEngine/IWindow.hpp>
#include <Config\Config.hpp>
#include "../Core/Console.hpp"

#include <map>
#include <sstream>

namespace Engine {
	namespace Input {

		struct InputEvent {
			int code;
			bool mouse;

			inline friend bool operator< (const InputEvent& lhs, const InputEvent& rhs) {
				int l = lhs.code;
				int r = rhs.code;
				if (lhs.mouse)
					l = -(l + 1);
				if (rhs.mouse)
					r = -(r + 1);
				return (l < r);
			};

			inline friend bool operator> (const InputEvent& lhs, const InputEvent& rhs) { return rhs < lhs; }
			inline friend bool operator<=(const InputEvent& lhs, const InputEvent& rhs) { return !(lhs > rhs); }
			inline friend bool operator>=(const InputEvent& lhs, const InputEvent& rhs) { return !(lhs < rhs); }

			inline friend bool operator== (const InputEvent& lhs, const InputEvent& rhs) { return (lhs.code == rhs.code) && (lhs.mouse == lhs.mouse); };
			inline friend bool operator!=(const InputEvent& lhs, const InputEvent& rhs) { return !(lhs == rhs); }
		};

		struct KeyBind {
			int code;
			int mod;
			bool mouse;

			static inline KeyBind create(std::string data) {
				std::stringstream ss(data);
				int code, mod;
				bool mouse;
				std::string buff;
				std::getline(ss, buff, ':');
				code = std::stoi(buff);

				std::getline(ss, buff, ':');
				mouse = std::stoi(buff) == 1 ? true : false;

				std::getline(ss, buff, ':');
				mod = std::stoi(buff);
				KeyBind kb{ code, mod, mouse };

				return kb;
			}

			inline std::string toString() {
				std::string res = std::to_string(code) + ":" + std::to_string(mouse) + ":" + std::to_string(mod);

				return res;
			}
		};

		class Input {
		public:
			Input();

			static Input* GetInput();
			static void Release();

			void attachConsole(Core::Console* con);
			void clearCallbacks();
			void setupCallbacks(IWindow* wnd); //Must call this when window is created to make GLFW callback functions work!

			bool isKeyBindPressed(const KeyBind &keyBind, bool includeMods = true);
			bool releasedThisFrame(const KeyBind &keyBind, bool includeMods = true);
			bool wasPressedThisFrame(const KeyBind &keyBind, bool includeMods = true);

			void getCursorDelta(float &x, float &y);

			bool consoleKeyWasPressed();
			void toggleConsole();
			bool consoleIsActive();

			void reset();

			void getWindowSize(int &w, int &h);

			bool sizeChange;

			void getMousePos(int &x, int &y);

			void getState(int &mx, int &my, int &mb, int &sc);

			void print();

		private:

			Config keyConf;
			bool consoleActive;

			bool blockInput;
			bool focus;

			static Input* singleton;

			void loadkeyBinds();
			void saveKeyBinds();

			IWindow* window;

			std::map<InputEvent, bool> keyMap;
			std::map<InputEvent, bool> releaseMap;
			std::map<InputEvent, bool> pressedMap;
			int modkey;

			int xPos;
			int yPos;

			int oldY;
			int oldX;

			float xDelta;
			float yDelta;

			float scrollX;
			float scrollY;

			int winW;
			int winH;

			Core::Console* console;

			static void keyCallback(IWindow* window, int scancode, int action, int mods);
			static void mouseButtonCallback(IWindow* window, int button, int action, int mods);
			static void cursorPosCallback(IWindow* window, int x, int y);
			static void scrollCallback(IWindow* window, int xoffset, int yoffset);
			static void characterCallback(IWindow* window, unsigned int codepoint);
			static void sizeCallback(IWindow* window, int w, int h);

			static void focusCallback(IWindow* window, bool focus);

			static void mouseDeltaCallback(IWindow* window, float dx, float dy);
		};

		enum KEYBINDS_NAME {
			KEYBIND_FORWARD = 0,
			KEYBIND_BACKWARD,
			KEYBIND_LEFT,
			KEYBIND_RIGHT,
			KEYBIND_ENTER,
			KEYBIND_MOUSE_L_CLICK,

			KEYBINDS_LENGTH
		};

		static const char* KeyBindsStrings[] =
		{ "KeyAction_Forward",
			"KeyAction_Backward",
			"KeyAction_Left",
			"KeyAction_Right",
			"KeyAction_Enter",
			"KeyAction_Mouse_L_Click"
		};

		static const char* KeyBindsDefault[] =
		{ "17:0:0",
		"31:0:0",
		"30:0:0",
		"32:0:0",
		"28:0:0",
		"0:1:0"
		};

		extern KeyBind KeyBindings[];

	}
}
#endif