/// Internal Includes
#include "Console.hpp"

namespace Engine {
	namespace Core {

		Console::Console() : consoleVisible(false) {}

		Console::~Console() {

		}

		void Console::setVisible(bool visible) {
			consoleVisible = visible;
		}

		bool Console::isVisible() const {
			return consoleVisible;
		}

	}
}