#include "Console.hpp"
#include <memory>
#include <stdarg.h>

Console::Console()
	: objectsInHistory(0)
	, currentMessageCount(0)
	, currentCommand()
	, currentCharIndex(0)
	, cmdHistory(nullptr)
	, consoleMessages(nullptr)
{
	memset(currentCommand.cmd, 0, COMMAND_LENGTH);
	cmdHistory = new Command[HISTORY_LENGTH];
	consoleMessages = new Message[MAX_CONSOLE_LINES];
	currentCharIndex = 0;
	objectsInHistory = 0;
	currentMessageCount = 0;
}

Console::~Console() {
	delete[] cmdHistory;
	delete[] consoleMessages;
}

void Console::putChar(const char character) {
	if (currentCharIndex != COMMAND_LENGTH) {
		currentCommand.cmd[currentCharIndex++] = character;
		print("%s\n", currentCommand.cmd);
	}
}

void Console::removeLastChar() {
	if (currentCharIndex != 0) {
		currentCommand.cmd[--currentCharIndex] = '\0';
		print("%s\n", currentCommand.cmd);
	}
}

void Console::print(const char* format, ...) {
	va_list vl;
	va_start(vl, format);
	Message msg;
	memset(msg.msg, 0, MESSAGE_LENGTH);
	vsnprintf_s(msg.msg, MESSAGE_LENGTH, MESSAGE_LENGTH-1, format, vl);
	va_end(vl);
	print(msg);
}

void Console::print(Message &msg) {

	if (currentMessageCount == MAX_CONSOLE_LINES - 1) {
		consoleMessages[messagesStart++] = msg;
	}
	else {
		consoleMessages[currentMessageCount++] = msg;
	}

	printf("%s", msg.msg);

}

void Console::execute() {
	printf("Execute\n");
	print(currentCommand.cmd);
	memset(currentCommand.cmd, 0, currentCharIndex);
	currentCharIndex = 0;
}

const int32_t Console::getStringLength(const char* str) const {
	int32_t len = 0;
	while (str[len] != '\0') {
		len++;
	}
	return len;
}