/// Internal Includes
#include "Console.hpp"
#include "../Graphics/Graphics.hpp"
#include "../Input/Input.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <sstream>
#include <string>

namespace Engine {
	namespace Core {

		Console* gConsole = nullptr;

		void test(int argv, char* argc[]) {

			for (int i = 0; i < argv; i++) {
				gConsole->print(argc[i], LogSeverity::LOG_WARNING);
			}

		}

		Console::Console()
			: consoleVisible(false)
			, cmdTypeIndex(0U)
			, command()
			, consoleLog()
			, cursorBlinkSpeed(0)
			, cursorVisible(false)
			, consoleBg(nullptr)
			, pGui(nullptr)
			, consolePanel(nullptr)
			, consoleText(nullptr)
			, consoleTextArea(nullptr)
			, themeOverride(nullptr)
			, width(0)
			, height(0)
			, consolePosY(-100)
			, consoleTargetY(-100)
			, initialized(false) {

			ConsoleCommand testConCmd;
			testConCmd.command = "test";
			testConCmd.description = "";
			testConCmd.func = test;

			registerCommand(testConCmd);

		}

		Console::~Console() {

			if (pGui) {
				pGui->removeGuiItem(consolePanel);
			}

			if (initialized) {
				delete themeOverride;
				delete consoleBg;
				delete consoleTextArea;
				delete consolePanel;
				delete consoleText;

				consoleBg = nullptr;
				consolePanel = nullptr;
				consoleText = nullptr;
			}

		}

		void Console::initGraphics(Engine::Graphics::CGui* gui) {

			if (gui) {

				Engine::Graphics::GuiInfo guiInfo;
				guiInfo.pAssetManager = gui->getAssetManager();

				themeOverride = new Engine::Theme::GuiTheme();

				consolePanel = new Engine::Graphics::Gui::Panel(guiInfo);
				consoleTextArea = new Engine::Graphics::Gui::TextArea(guiInfo);
				consoleText = new Engine::Graphics::Gui::Label(guiInfo);

				consoleBg = new Engine::Graphics::Texture::Texture2D();
				consoleBg->singleColor(0.192869F, 0.246195F, 0.168075F, 0.900000F);
				themeOverride->panel.textureNormal = consoleBg;

				consolePanel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
				consoleTextArea->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
				consoleText->setAnchorPoint(Engine::Graphics::GuiAnchor::BOTTOM);

				consolePanel->setPosition(0, int(consolePosY));
				consolePanel->setThemeOverride(themeOverride);
				consolePanel->setSize(0, 0);
				consolePanel->setVisible(false);
				consolePanel->setZIndex(1000);

				consoleTextArea->setPosition(0, 0);
				consoleTextArea->setSize(0, 0);
				consoleTextArea->setVisible(true);
				consoleTextArea->setThemeOverride(themeOverride);

				consoleText->setPosition(0, 0);
				consoleText->setSize(0, 0);
				consoleText->setVisible(true);

				consolePanel->addGuiItem(consoleTextArea);
				consolePanel->addGuiItem(consoleText);

				for (size_t i = 0; i < consoleLog.size(); i++) {
					consoleTextArea->addText(consoleLog[i]);
				}

				gui->addGuiItem(consolePanel);

				pGui = gui;

				initialized = true;
			}
		}

		void Console::setVisible(bool visible) {
			consoleVisible = visible;
		}

		bool Console::isVisible() const {
			return consoleVisible;
		}

		void Console::putChar(char chr) {
			if (cmdTypeIndex < CONSOLE_STRING_LENGTH) {
				command[cmdTypeIndex] = chr;
				cmdTypeIndex++;
			}
		}

		void Console::backSpace() {
			if (cmdTypeIndex > 0) {
				cmdTypeIndex--;
			}
			command[cmdTypeIndex] = '\0';
		}

		void Console::execute() {
			print(command);
			checkCommand(command);
			cmdTypeIndex = 0;
			memset(command, 0, sizeof(ConsoleString));
		}

		void Console::print(FormattedString str, LogSeverity severity) {

			if (severity == LogSeverity::LOG_INFO) {
				str.formatString(size_t(0), size_t(-1), CONSOLE_INFO_COLOR);
			} else if (severity == LogSeverity::LOG_WARNING) {
				str.formatString(size_t(0), size_t(-1), CONSOLE_WARNING_COLOR);
			} else if (severity == LogSeverity::LOG_ERROR) {
				str.formatString(size_t(0), size_t(-1), CONSOLE_ERROR_COLOR);
			} else if (severity == LogSeverity::LOG_CRITICAL) {
				str.formatString(size_t(0), size_t(-1), CONSOLE_CRITICAL_COLOR);
			}

			consoleLog.push_back(str);
			if (consoleTextArea) {
				consoleTextArea->addText(str);
			}
		}

		void Console::registerCommand(ConsoleCommand conCmd) {
			consoleCommands[conCmd.command] = conCmd;
		}

		void Console::update(float dt) {

			updateCursor(dt);

			checkToggle();

			consolePanel->setVisible(consoleVisible);

			if (consoleVisible) {
				updateGraphics();
			}
		}

		void Console::updateSize(int w, int h) {
			consolePanel->setSize(w, (h / 2));
			consoleTextArea->setSize(w, (h / 2) - 30);
			consoleText->setSize(w, 30);

			width = w;
			height = h;
		}

		const FormattedString Console::getCommandString() const {
			FormattedString fString = "> ";
			fString += command;

			if (cursorVisible) {
				fString += "|";
			}

			return fString;
		}

		const std::vector<FormattedString>& Console::getConsoleLog() const {
			return consoleLog;
		}

		int Console::countCharacters(ConsoleString& cmd, char c) {

			int count = 0;

			int idx = 0;

			while (cmd[idx] != 0) {
				if (cmd[idx] == c) {
					count++;
				}
				idx++;
			}
			return count;
		}

		void Console::processCommand(int argc, char *argv[]) {

			std::string cmd = argv[0];

			if (consoleCommands.count(cmd) == 1) {
				ConsoleCommand com = consoleCommands[cmd];

				if (nullptr != com.func) {
					com.func(argc, argv);
				}

			} else {
				print("Command not found!", LogSeverity::LOG_ERROR);
			}

		}

		void Console::updateCursor(float dt) {
			cursorBlinkSpeed -= dt;

			if (cursorBlinkSpeed < 0.0F) {
				cursorBlinkSpeed += CONSOLE_CURSOR_BLINK_INTERVAL;
				cursorVisible = !cursorVisible;
			}
		}
		void Console::checkToggle() {

			if (Input::Input::GetInput()->consoleKeyWasPressed()) {
				Input::Input::GetInput()->toggleConsole();
				memset(command, 0, sizeof(ConsoleString));
				cmdTypeIndex = 0;
				if (consoleTargetY == 0) {
					consoleTargetY = float(-(height / 2));
				} else {
					consoleTargetY = 0;
					setVisible(Input::Input::GetInput()->consoleIsActive());
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

				updateSize(width, height);
				consolePanel->setSize(width, (height / 2) + (int)consolePosY);

			} else {
				setVisible(Input::Input::GetInput()->consoleIsActive());
			}
		}
		void Console::updateGraphics() {
			//const std::vector<Core::FormattedString>& log = getConsoleLog();
			//
			//Core::FormattedString fString = "";
			//
			//for (size_t i = 0; i < log.size(); i++) {
			//	fString += log[i];
			//	fString += "\n";
			//}

			consoleText->setText(getCommandString());
		}

		void Console::checkCommand(ConsoleString& cmd) {

			std::stringstream ss;

			int spaceCount = countCharacters(cmd, ' ') + 1;

			char** args = new char*[spaceCount];

			int argc = 1;
			//int idx = 0;
			//
			//args[0] = cmd;
			// @ TODO
			// @ REFACTOR
			//while (cmd[idx] != 0) {
			//
			//	if (cmd[idx] == ' ') {
			//		cmd[idx] = '\0';
			//		args[argc] = &cmd[idx+1];
			//		argc++;
			//	}
			//	idx++;
			//}

			processCommand(argc, args);

			delete []args;
		}
	}
}