#ifndef INPUTHELPER_HPP
#define INPUTHELPER_HPP

#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "hid.lib")
#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#pragma warning( push , 3 )
#include <glm/glm.hpp>
#pragma warning( pop )

namespace XBoneMapping {

	const int BTN_A = 0x1000;
	const int BTN_B = 0x2000;
	const int BTN_X = 0x4000;
	const int BTN_Y = 0x8000;

	const int BTN_BACK = 0x0020;
	const int BTN_START = 0x0010;

	const int BTN_D_UP = 0x0001;
	const int BTN_D_LEFT = 0x0004;
	const int BTN_D_RIGHT = 0x0008;
	const int BTN_D_DOWN = 0x0002;

	const int BTN_BUMP_RIGHT = 0x0200;
	const int BTN_BUMP_LEFT = 0x0100;

	const int BTN_THUMB_RIGHT = 0x0080;
	const int BTN_THUMB_LEFT = 0x0040;

}

struct ControllerState {
	glm::vec2 rThumb;
	glm::vec2 lThumb;
	
	float lTrigger;
	float rTrigger;

	int buttons;

	bool lastSent;

};

BOOL IsXInputDevice(const GUID* pGuidProductFromDirectInput);

bool getControllerState(int id, ControllerState& state);

bool XInputStateChangedThisFrame(ControllerState *oldState, ControllerState *newState);

#endif