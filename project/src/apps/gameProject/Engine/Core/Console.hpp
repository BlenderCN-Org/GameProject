#ifndef CONSOLE_HPP
#define CONSOLE_HPP

/// Internal Includes
#include "../Graphics/Gui.hpp"
#include "../Graphics/Gui/Panel.hpp"
#include "../Graphics/Gui/Label.hpp"
#include "../Graphics/Gui/TextArea.hpp"
#include "../Graphics/Textures/Texture2D.hpp"

/// External Includes
#include <Core/FormattedString.hpp>

/// Std Includes
#include <vector>
#include <map>

namespace Engine {
	namespace Core {

		const glm::vec4 CONSOLE_INFO_COLOR      (0.638272F, 0.638272F, 0.638272F, 1.000000F); // [0.638272, 0.638272, 0.638272, 1.000000]
		const glm::vec4 CONSOLE_WARNING_COLOR   (1.000000F, 0.637106F, 0.000000F, 1.000000F); // [1.000000, 0.637106, 0.000000, 1.000000]
		const glm::vec4 CONSOLE_ERROR_COLOR     (1.000000F, 0.002383F, 0.000000F, 1.000000F); // [1.000000, 0.002383, 0.000000, 1.000000]
		const glm::vec4 CONSOLE_CRITICAL_COLOR  (0.615178F, 0.000000F, 0.000000F, 1.000000F); // [0.615178, 0.000000, 0.000000, 1.000000]

		const float CONSOLE_CURSOR_BLINK_INTERVAL = 0.5F; // time in seconds

		const unsigned char CONSOLE_STRING_LENGTH = 250;
		typedef char ConsoleString[CONSOLE_STRING_LENGTH + 1]; // + 1 for null termination

		enum class LogSeverity {
			LOG_INFO,
			LOG_WARNING,
			LOG_ERROR,
			LOG_CRITICAL
		};

		typedef void(*_pfnConFunc)(int argc, char *argv[]);

		struct ConsoleCommand {
			char* command;
			char* description;
			_pfnConFunc func;
		};

		class Console {
		public:

			Console();
			virtual ~Console();

			void initGraphics(Engine::Graphics::CGui* gui);

			void setVisible(bool visible);

			bool isVisible() const;

			void putChar(char chr);
			void backSpace();
			void execute();

			void registerCommand(ConsoleCommand conCmd);

			void update(float dt);
			void updateSize(int w, int h);

			void print(FormattedString str, LogSeverity severity = LogSeverity::LOG_INFO);

			const FormattedString getCommandString() const;
			const std::vector<FormattedString>& getConsoleLog() const;

		private:

			bool initialized;

			int countCharacters(ConsoleString& cmd, char c);
			void processCommand(int argc, char *argv[]);

			void updateCursor(float dt);
			void checkToggle();
			void updateGraphics();

			void checkCommand(ConsoleString& cmd);

			bool consoleVisible;

			unsigned char cmdTypeIndex;

			ConsoleString command;
			std::vector<FormattedString> consoleLog;
			std::map<std::string, ConsoleCommand> consoleCommands;

			float cursorBlinkSpeed;
			bool cursorVisible;

			Engine::Theme::GuiTheme* themeOverride;

			Engine::Graphics::CGui* pGui;
			Engine::Graphics::Gui::Panel* consolePanel;
			Engine::Graphics::Gui::TextArea* consoleTextArea;
			Engine::Graphics::Gui::Label* consoleText;

			Engine::Graphics::Texture::Texture2D* consoleBg;

			float consolePosY;
			float consoleTargetY;

			int width;
			int height;

		};

		extern Console* gConsole;

	}
}

#endif