#ifndef WIN32_INIT_HPP
#define WIN32_INIT_HPP

#include "Platform.hpp"

#ifdef _WIN32
LPCWSTR const WindowClassName = L"OpenGlWindowClass";

bool registerWindowClass();
bool unregisterWindowClass();

#endif


#endif