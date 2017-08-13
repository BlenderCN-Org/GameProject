#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <cstdint>

#define MAX_CONSOLE_LINES 10000
#define HISTORY_LENGTH 1000
#define COMMAND_LENGTH 200
#define MESSAGE_LENGTH 200

struct Message {
	char msg[MESSAGE_LENGTH];
};

struct Command {
	char cmd[COMMAND_LENGTH];
};

class Console {

public:
	
	Console();
	virtual ~Console();

	void putChar(const char character);
	void removeLastChar();

	void print(const char* format, ...);
	void print(Message &msg);

	void execute();

private:

	int32_t objectsInHistory;
	int32_t currentMessageCount;
	int32_t messagesStart;

	Command currentCommand;
	int32_t currentCharIndex;

	Command* cmdHistory;
	Message* consoleMessages;

};

#endif