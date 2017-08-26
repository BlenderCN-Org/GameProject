#ifndef INPUTHANDLING_HPP
#define INPUTHANDLING_HPP

#include "Window.hpp"

void processRawMouseEvents(BaseWindow * wnd, RAWMOUSE mouseEvents);
void processRawKeyboardEvents(BaseWindow * wnd, RAWKEYBOARD keyBoardEvents);

// maybe not needed
void processRawHidEvents(BaseWindow* wnd, PRAWINPUT rawInputEvent);

void setupDirectInput(BaseWindow* wnd);

void enableXinput(bool enable);
void processXInput(BaseWindow* wnd);

#endif