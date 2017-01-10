#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <string>
#include <vector>
#include <list>

#include "Settings\DisplaySettings.hpp"

enum console_item_type_t {
	CTYPE_UCHAR,        // variable: unsigned char
	CTYPE_CHAR,         // variable: char
	CTYPE_UINT,         // variable: unsigned char
	CTYPE_INT,          // variable: int
	CTYPE_FLOAT,        // variable: float
	CTYPE_STRING,       // variable: std::string
	CTYPE_FUNCTION      // function
};

class Console;
typedef void(*console_function)(Console*, const std::vector<std::string> &);

class Console {
public:

	Console();
	virtual ~Console();

	void addItem(const std::string & strName, const std::string &description, void *pointer, console_item_type_t type);
	void removeItem(const std::string strName);

	void keyPress(char c);
	void backSpace();

	void print(std::string text);

	void execute();

	void setDefaultCommand(console_function func);

	void listItems();

	void setVisible(bool v);

	std::string getText();

	std::string getHistory();

	DisplaySettings* dispSettings;

	bool reset;

private:

	void addToHistory(std::string h);

	typedef struct {
		std::string name;
		std::string description;

		console_item_type_t type;

		union {
			void *variable_pointer;
			console_function function;
		};
	} console_item_t;

	bool parseCommandLine();

	std::string line;
	std::vector<std::string> historyBuffer;
	std::vector<std::string> m_commandBuffer;
	size_t maxCommands;

	std::list<console_item_t> m_itemsList;

	std::string	m_commandLine;

	console_function defaultCommand;
};

#endif