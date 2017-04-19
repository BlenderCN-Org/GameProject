#ifndef INPUTHELPER_HPP
#define INPUTHELPER_HPP

#include <Windows.h>
#include <Xinput.h>

BOOL IsXInputDevice(const GUID* pGuidProductFromDirectInput);

bool XInputStateChangedThisFrame(PXINPUT_STATE oldState, PXINPUT_STATE newState);

#endif