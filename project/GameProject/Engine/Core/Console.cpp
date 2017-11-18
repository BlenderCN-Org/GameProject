/// Internal Includes
#include "Console.hpp"
#include "../Input/Input.hpp"

namespace Engine {
	namespace Core {

		Console::Console()
			: consoleVisible(false)
			, cmdTypeIndex(0U)
			, command()
			, consoleLog()
			, cursorBlinkSpeed(0)
			, cursorVisible(false)
			, consoleBg(nullptr)
			, consoleGui(nullptr)
			, consoleHistory(nullptr)
			, consolePanel(nullptr)
			, consoleText(nullptr)
			, width(0)
			, height(0)
			, consolePosY(-100)
			, consoleTargetY(-100) {

			consoleGui = new Engine::Graphics::CGui();
			consolePanel = new Engine::Graphics::Gui::Panel();
			consoleTextArea = new Engine::Graphics::Gui::TextArea();
			consoleHistory = new Engine::Graphics::Gui::Label();
			consoleText = new Engine::Graphics::Gui::Label();

			consoleBg = new Engine::Graphics::Texture::Texture2D();
			consoleBg->singleColor(0.192869F, 0.246195F, 0.168075F, 0.900000F);

			consolePanel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
			consoleTextArea->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
			consoleHistory->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
			consoleText->setAnchorPoint(Engine::Graphics::GuiAnchor::BOTTOM);

			consoleGui->setVisible(true);
			consoleGui->setPosition(0, int(consolePosY));

			consolePanel->setPosition(0, 0);
			consolePanel->setSize(0, 0);
			consolePanel->setVisible(true);
			consolePanel->setTexture(consoleBg);

			consoleHistory->setPosition(0, 0);
			consoleHistory->setSize(0, 0);
			consoleHistory->setVisible(true);

			consoleTextArea->setPosition(0, 0);
			consoleTextArea->setSize(0, 0);
			consoleTextArea->setVisible(true);

			consoleText->setPosition(0, 0);
			consoleText->setSize(0, 0);
			consoleText->setVisible(true);

			consoleGui->addGuiItem(consolePanel);
			consolePanel->addGuiItem(consoleTextArea);
			//consolePanel->addGuiItem(consoleHistory);
			consolePanel->addGuiItem(consoleText);

			Core::FormattedString str = "Foobar";
			str.formatString(0, 6, glm::vec4(1, 0, 0, 0));

			str += " This is a test";

			consoleText->setText(str);

		}

		Console::~Console() {
			delete consoleBg;
			delete consoleGui;
			delete consoleTextArea;
			delete consoleHistory;
			delete consolePanel;
			delete consoleText;

			consoleBg = nullptr;
			consoleGui = nullptr;
			consoleHistory = nullptr;
			consolePanel = nullptr;
			consoleText = nullptr;

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
				str.formatString(0, -1, CONSOLE_INFO_COLOR);
			} else if (severity == LogSeverity::LOG_WARNING) {
				str.formatString(0, -1, CONSOLE_WARNING_COLOR);
			} else if (severity == LogSeverity::LOG_ERROR) {
				str.formatString(0, -1, CONSOLE_ERROR_COLOR);
			} else if (severity == LogSeverity::LOG_CRITICAL) {
				str.formatString(0, -1, CONSOLE_CRITICAL_COLOR);
			}

			//consoleLog.push_back(str);
			consoleTextArea->addText(str);
		}

		void Console::update(float dt) {

			updateCursor(dt);

			checkToggle();

			if (consoleVisible) {
				consoleGui->update(dt);
				updateGraphics();
			}
		}

		void Console::updateSize(int w, int h) {
			consolePanel->setSize(w, (h / 2));
			consoleHistory->setSize(w, (h / 2) - 30);
			consoleTextArea->setSize(w, (h / 2) - 30);
			consoleText->setSize(w, 30);

			width = w;
			height = h;
		}

		void Console::render() {
			if (consoleVisible) {
				consoleGui->render();
			}
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

				consoleGui->setPosition(0, (int)consolePosY);
			} else {
				setVisible(Input::Input::GetInput()->consoleIsActive());
			}
		}
		void Console::updateGraphics() {
			const std::vector<Core::FormattedString>& log = getConsoleLog();

			Core::FormattedString fString = "";

			for (size_t i = 0; i < log.size(); i++) {
				fString += log[i];
				fString += "\n";
			}
			//consoleTextArea->setText(fString);
			//consoleHistory->setText(fString);
			consoleText->setText(getCommandString());
		}

		void Console::checkCommand(ConsoleString& cmd) {
			print("Command not valid", Core::LogSeverity::LOG_ERROR);
		}

	}
}