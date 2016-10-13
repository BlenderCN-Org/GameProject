#include "Input.hpp"

#include <iostream>

Input* Input::singleton = nullptr;
KeyBind KeyBindings[KEYBINDS_NAME::KEYBINDS_LENGTH];

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	if (!singleton->consoleActive && action == GLFW_REPEAT)
		return;

	singleton->keyMap[InputEvent{scancode, false}] = (action == GLFW_PRESS);
	singleton->modkey = mods;
	if (singleton->consoleActive)
	{
		if ( (scancode == 28 || scancode == 284) && action == GLFW_PRESS)
		{
			singleton->console->print("\n>");
			singleton->console->execute();
		}
		else if (scancode == 14 && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			singleton->console->backSpace();
		}
	}
	printf("Scancode %d with modkey %d\n", scancode, mods);
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (!singleton->consoleActive && action == GLFW_REPEAT)
		return;

	singleton->keyMap[InputEvent{ button, true}] = (action == 1);
	//printf("MouseButton %d with modkey %d\n", button, mods);
}

void Input::cursorPosCallback(GLFWwindow * window, double x, double y)
{
	singleton->xDelta = float(x) - float(singleton->xPos);
	singleton->yDelta = float(y) - float(singleton->yPos);

	singleton->xPos = int(x);
	singleton->yPos = int(y);

	//printf("Pos (%d,%d)\n", int(x), int(y));
	//printf("Delta (%f,%f)\n", singleton->xDelta, singleton->yDelta);

}

void Input::scrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	singleton->scrollX = float(xoffset);
	singleton->scrollY = float(yoffset);

	//printf("Scroll (%f,%f)\n", float(xoffset), float(yoffset));
}

void Input::characterCallback(GLFWwindow * window, unsigned int codepoint) {
	if (singleton->consoleActive && singleton->console)
	{
		//printf("%c", codepoint);
		singleton->console->keyPress(codepoint);
	}
}

void Input::sizeCallback(GLFWwindow * window, int w, int h) {

	singleton->winW = w;
	singleton->winH = h;
	singleton->sizeChange = true;
	printf("%d,%d\n", w, h);
	//glViewport(0, 0, w, h);
}

Input* Input::getInput()
{
	if (singleton == nullptr)
		singleton = new Input();
	return singleton;
}

void Input::release()
{
	if (singleton != nullptr)
	{
		singleton->saveKeyBinds();
		singleton->keyConf.write("Config/keybinds.ini");
		delete singleton;
	}
	singleton = nullptr;
}

void Input::attachConsole(Console * con) {
	console = con;
}

void Input::setupCallbacks(GLFWwindow * wnd)
{
	window = wnd;
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCharCallback(window, characterCallback);
	glfwSetWindowSizeCallback(window, sizeCallback);

	glfwGetWindowSize(wnd, &winW, &winH);

}

bool Input::isKeyBindPressed(KeyBind & keyBind)
{
	bool pressed = keyMap[InputEvent{ keyBind.code, keyBind.mouse }];

	pressed &= (keyBind.mod == modkey);

	return pressed;
}

void Input::getCursorDelta(float & x, float & y)
{
	x = xDelta;
	y = yDelta;
}

bool Input::consoleKeyWasPressed() {
	bool pressed = keyMap[InputEvent{ 41, false}];
	keyMap[InputEvent{ 41, false }] = false;
	return pressed;
}

void Input::toggleConsole() {
	//printf("Console toggle\n");
	if (consoleActive) {
		console->backSpace();
	}

	consoleActive = !consoleActive;
}

bool Input::consoleIsActive() {
	return consoleActive;
}

void Input::reset()
{
	xDelta = 0.0f;
	yDelta = 0.0;
	scrollX = 0.0;
	scrollY = 0.0;

	sizeChange = false;
}

void Input::getWindowSize(int & w, int & h) {
	w = winW;
	h = winH;
}

void Input::getMousePos(int & x, int & y)
{
	double xx, yy;
	glfwGetCursorPos(window, &xx, &yy);
	x = (int)xx;
	y = (int)yy;
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

void Input::print()
{
	// show content:
	for ( std::map<InputEvent, bool>::iterator it = keyMap.begin(); it != keyMap.end(); ++it )
		std::cout << it->first.code << " " << it->first.mouse << " => " << it->second << '\n';
}

Input::Input()
{
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

}

void Input::loadkeyBinds()
{
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
	for (size_t i = 0; i < KEYBINDS_NAME::KEYBINDS_LENGTH; i++)
	{
		bindingData = keyConf.getString("", KeyBindsStrings[i], KeyBindsDefault[i]);
		KeyBindings[i] = KeyBind::create(bindingData);
	}

}

void Input::saveKeyBinds()
{
	for (size_t i = 0; i < KEYBINDS_NAME::KEYBINDS_LENGTH; i++)
	{
		keyConf.addString("", KeyBindsStrings[i], KeyBindings[i].toString());
	}
	//keyConf.addString("", KeyBindsStrings[KEYBIND_FORWARD], KeyBindings[KEYBINDS_NAME::KEYBIND_FORWARD].toString());
	//keyConf.addString("", KeyBindsStrings[KEYBIND_BACKWARD], KeyBindings[KEYBINDS_NAME::KEYBIND_BACKWARD].toString());
	//keyConf.addString("", KeyBindsStrings[KEYBIND_LEFT], KeyBindings[KEYBINDS_NAME::KEYBIND_LEFT].toString());
	//keyConf.addString("", KeyBindsStrings[KEYBIND_RIGHT], KeyBindings[KEYBINDS_NAME::KEYBIND_RIGHT].toString());
}

