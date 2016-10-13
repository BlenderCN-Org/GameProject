#include "Console.hpp"

#include <sstream>

void defaultConsoleFunction(Console* con, const std::vector<std::string> & arg) {
	
	con->print(">");

	con->print(arg[0]);
	con->print(" is not a recognized command\n");
}

void listItemsFunc(Console* con, const std::vector<std::string> & arg) {

	con->listItems();
}

void changeFullscreenMode(Console* con, const std::vector<std::string> & arg) {
	
	if ( arg.size() == 2 )
	{
		con->print(">");

		con->print("Changing fullscreen mode");
		con->print("\n");

		con->dispSettings->setFullscreenMode((FullscreenMode)std::atoi(arg[1].c_str()));
		con->dispSettings->apply();
	}
	else
	{
		con->print(">");

		con->print(arg[0]);
		con->print(" takes exactly 1 parameter: ");
		con->print(std::to_string(arg.size()));
		con->print(" given\n");
	}
}

void setRes(Console* con, const std::vector<std::string> & arg) {

	if ( arg.size() == 3 )
	{
		con->print(">");

		con->print("Changing resolution");
		con->print("\n");

		con->dispSettings->setResolution(std::atoi(arg[1].c_str()), std::atoi(arg[2].c_str()));
		//con->dispSettings->setFullscreenMode((FullscreenMode)std::atoi(arg[1].c_str()));
		con->dispSettings->apply();
	} else
	{
		con->print(">");

		con->print(arg[0]);
		con->print(" takes exactly 2 parameter: ");
		con->print(std::to_string(arg.size()));
		con->print(" given\n");
	}
}

void reset_fun(Console* con, const std::vector<std::string> & arg) {
	con->reset = true;
}

Console::Console() {

	reset = false;
	setDefaultCommand(defaultConsoleFunction);
	maxCommands = 20;

	addItem("commandCount", "defines the command history count", &maxCommands, console_item_type_t::CTYPE_UINT);

	addItem("listItems", "List all funcionts and their information", &listItemsFunc, console_item_type_t::CTYPE_FUNCTION);
	addItem("fullscreenMode", "Sets fullscreen mode FULLSCREEN(0), WINDOWED_FULLSCREEN(1), WINDOWED(2), WINDOWED_BORDERLESS(3)", &changeFullscreenMode, console_item_type_t::CTYPE_FUNCTION);
	addItem("setResolution", "Sets screen resolution", &setRes, console_item_type_t::CTYPE_FUNCTION);
	addItem("reset", "Resets opengl", &reset_fun, console_item_type_t::CTYPE_FUNCTION);

	line = "";
}

Console::~Console() {
}

void Console::addItem(const std::string & strName, const std::string &description, void * pointer, console_item_type_t type) {
	console_item_t item;
	item.name = strName;
	item.description = description;
	item.type = type;
	item.variable_pointer = pointer;
	
	m_itemsList.push_back(item);

}

void Console::removeItem(const std::string strName) {}

void Console::keyPress(char c) {
	//printf("KeyPress\n");
	m_commandLine.push_back(c);
	printf("\r>%s", m_commandLine.c_str());
}

void Console::backSpace() {
	//printf("BackSpace\n");
	if (m_commandLine.size() > 0)
	{
		m_commandLine.pop_back();
		printf("\r>%s ", m_commandLine.c_str());
	}
}

void Console::print(std::string text) {
	printf("%s", text.c_str());
	
	for ( size_t i = 0; i < text.size(); i++ )
	{
		line.push_back(text[i]);
		if ( text[i] == '\n' )
		{
			addToHistory(line);
			line = "";
		}
	}

}

void Console::execute() {
	if (m_commandLine.length() > 0) {
		parseCommandLine();
	}
	m_commandLine = "";
}

void Console::setDefaultCommand(console_function func) {
	defaultCommand = func;
}

void Console::listItems() {
	std::list<console_item_t>::const_iterator iter;

	for (iter = m_itemsList.begin(); iter != m_itemsList.end(); ++iter)
	{
		print(iter->name);
		print(" ");
		switch (iter->type)
		{
		case CTYPE_UCHAR:
			print("Type: uchar");
			break;
		case CTYPE_CHAR:
			print("Type: char");
			break;
		case CTYPE_UINT:
			print("Type: uint");
			break;
		case CTYPE_INT:
			print("Type: int");
			break;
		case CTYPE_FLOAT:
			print("Type: float");
			break;
		case CTYPE_STRING:
			print("Type: string");
			break;
		case CTYPE_FUNCTION:
			print("Type: function");
			break;
		default:
			break;
		}
		print("\t");
		print(iter->description);
		print("\n");
	}
}

void Console::setVisible(bool v) {
}

std::string Console::getText() {
	return m_commandLine;
}

std::string Console::getHistory()
{
	std::string str = "";
	for ( size_t i = 0; i < historyBuffer.size(); i++ )
	{
		str += historyBuffer[i];
	}
	return str;
}

void Console::addToHistory(std::string h)
{
	if ( historyBuffer.size() > 25 )
		historyBuffer.erase(historyBuffer.begin());
	historyBuffer.push_back(h);
}

bool Console::parseCommandLine() {
	std::ostringstream out; // more info here
	std::string::size_type index = 0;
	std::vector<std::string> arguments;
	std::list<console_item_t>::const_iterator iter;

	// add to text buffer
	//if (command_echo_enabled)
	//{
	//	print(m_commandLine);
	//}

	// add to commandline buffer
	m_commandBuffer.push_back(m_commandLine);
	if (m_commandBuffer.size() > maxCommands) m_commandBuffer.erase(m_commandBuffer.begin());

	// tokenize
	while (index != std::string::npos)
	{
		// push word
		std::string::size_type next_space = m_commandLine.find(' ', index);
		arguments.push_back(m_commandLine.substr(index, next_space));

		// increment index
		if (next_space != std::string::npos) index = next_space + 1;
		else break;
	}

	// execute (look for the command or variable)
	for (iter = m_itemsList.begin(); iter != m_itemsList.end(); ++iter)
	{
		if (iter->name == arguments[0])
		{
			switch (iter->type)
			{
			case CTYPE_UINT:
				if (arguments.size() > 2)return false;
				else if (arguments.size() == 1)
				{
					out.str(""); // clear stringstream
					out << (*iter).name << " = " << *((unsigned int *)(*iter).variable_pointer);
					print(out.str());
					return true;
				} else if (arguments.size() == 2)
				{
					*((unsigned int *)(*iter).variable_pointer) = (unsigned int)atoi(arguments[1].c_str());
					return true;
				}
				break;
			case CTYPE_INT:
				if (arguments.size() > 2)return false;
				else if (arguments.size() == 1)
				{
					out.str(""); // clear stringstream
					out << (*iter).name << " = " << *((int *)(*iter).variable_pointer);
					print(out.str());
					return true;
				} else if (arguments.size() == 2)
				{
					*((int *)(*iter).variable_pointer) = (int)atoi(arguments[1].c_str());
					return true;
				}
				break;
			case CTYPE_FUNCTION:
				(*iter).function(this, arguments);
				return true;
				break;
			default:
				defaultCommand(this, arguments);
				return false;
				break;
			}
		}
	}
	
	defaultCommand(this, arguments);

	return false;
}
