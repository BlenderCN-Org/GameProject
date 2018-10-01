// project includes
#include "InputHandling.hpp"
#include "InputHelper.hpp"

#include <glm/glm.hpp>

// std includes
#include <iostream>
#include <vector>

// system includes
#include <hidsdi.h>
#include <Xinput.h>
#include <dinput.h>

#define RI_MOUSE_HWHEEL 0x0800

std::vector<GUID> directInputDevices;

void processRawMouseEvents(BaseWindow * wnd, RAWMOUSE mouseEvents) {
	if (wnd->inputProcessedByControllers)
		return;
	if (wnd->mouseButtonCallback) {
		int button = -1;
		int action = 0;
		int mods = wnd->modkeys;
		switch (mouseEvents.usButtonFlags) {
		case(RI_MOUSE_BUTTON_1_DOWN):
		{
			button = 0;
			action = ACTION_BUTTON_DOWN;
			break;
		}
		case(RI_MOUSE_BUTTON_1_UP):
		{
			button = 0;
			action = ACTION_BUTTON_UP;
			break;
		}
		case(RI_MOUSE_BUTTON_2_DOWN):
		{
			button = 1;
			action = ACTION_BUTTON_DOWN;
			break;
		}
		case(RI_MOUSE_BUTTON_2_UP):
		{
			button = 1;
			action = ACTION_BUTTON_UP;
			break;
		}
		case(RI_MOUSE_BUTTON_3_DOWN):
		{
			button = 2;
			action = ACTION_BUTTON_DOWN;
			break;
		}
		case(RI_MOUSE_BUTTON_3_UP):
		{
			button = 2;
			action = ACTION_BUTTON_UP;
			break;
		}
		case(RI_MOUSE_BUTTON_4_DOWN):
		{
			button = 3;
			action = ACTION_BUTTON_DOWN;
			break;
		}
		case(RI_MOUSE_BUTTON_4_UP):
		{
			button = 3;
			action = ACTION_BUTTON_UP;
			break;
		}
		case(RI_MOUSE_BUTTON_5_DOWN):
		{
			button = 4;
			action = ACTION_BUTTON_DOWN;
			break;
		}
		case(RI_MOUSE_BUTTON_5_UP):
		{
			button = 4;
			action = ACTION_BUTTON_UP;
			break;
		}
		default:
			break;
		}
		if (button != -1)
			wnd->mouseButtonCallback(wnd, button, action, mods);
		//printf("mouse button callback\n");

		//std::cout << button;
	}

	if (wnd->mouseMoveCallback) {
		POINT pt;
		GetCursorPos(&pt);

		ScreenToClient(wnd->getWindowHandle(), &pt);
		wnd->mouseMoveCallback(wnd, pt.x, pt.y);

		//printf("move callback\n");
	}

	if (wnd->mouseDeltaCallback) {

		float mouseDX = (float)mouseEvents.lLastX;
		float mouseDY = (float)mouseEvents.lLastY;

		wnd->mouseDeltaCallback(wnd, mouseDX, mouseDY);
		//printf("delta callback\n");
	}

	if (wnd->scrollCallback) {
		short scrollX = 0;
		short scrollY = 0;

		if (RI_MOUSE_WHEEL == mouseEvents.usButtonFlags) {
			scrollX = (short)mouseEvents.usButtonData;
		}
		if (RI_MOUSE_HWHEEL == mouseEvents.usButtonFlags) {
			scrollY = (short)mouseEvents.usButtonData;
		}

		wnd->scrollCallback(wnd, (int)scrollX, (int)scrollY);
	}

	if (wnd->cursorLock && wnd->hasFocus) {
		RECT rec;
		GetWindowRect(wnd->getWindowHandle(), &rec);

		int posX = (rec.right - rec.left) / 2;
		posX += rec.left;

		int posY = (rec.bottom - rec.top) / 2;
		posY += rec.top;

		SetCursorPos(posX, posY);
	}
}

void processRawKeyboardEvents(BaseWindow * wnd, RAWKEYBOARD keyBoardEvents) {
	if (wnd->inputProcessedByControllers)
		return;
	if (keyBoardEvents.Flags == RI_KEY_MAKE) {
		if (keyBoardEvents.VKey == VK_SHIFT) {
			wnd->modkeys |= MODKEY_SHIFT;
		} else if (keyBoardEvents.VKey == VK_CONTROL) {
			wnd->modkeys |= MODKEY_CTRL;
		} else if (keyBoardEvents.VKey == VK_MENU) {
			wnd->modkeys |= MODKEY_ALT;
		} else if (keyBoardEvents.VKey == VK_LWIN || keyBoardEvents.VKey == VK_RWIN) {
			wnd->modkeys |= MODKEY_SUPER;
		}
	} else {
		if (keyBoardEvents.VKey == VK_SHIFT) {
			wnd->modkeys &= ~MODKEY_SHIFT;
		} else if (keyBoardEvents.VKey == VK_CONTROL) {
			wnd->modkeys &= ~MODKEY_CTRL;
		} else if (keyBoardEvents.VKey == VK_MENU) {
			wnd->modkeys &= ~MODKEY_ALT;
		} else if (keyBoardEvents.VKey == VK_LWIN || keyBoardEvents.VKey == VK_RWIN) {
			wnd->modkeys &= ~MODKEY_SUPER;
		}
	}

	if (wnd->keyCallback) {
		int action = keyBoardEvents.Message == WM_KEYUP ? ACTION_BUTTON_UP : ACTION_BUTTON_DOWN;

		int mods = wnd->modkeys;

		wnd->keyCallback(wnd, keyBoardEvents.MakeCode, action, mods);
		//printf("keyCallback\n");
	}
}

// this is probably not the way to do controllers using XInput and DirectInputInstead
void processRawHidEvents(BaseWindow* , PRAWINPUT raw) {

	//std::cout << "GamePad/joystic\n";

	UINT bufferSize = 0;

	HANDLE heap = GetProcessHeap();

	PHIDP_PREPARSED_DATA preparsedData = nullptr;
	HIDP_CAPS caps = { 0 };

	GetRawInputDeviceInfo(raw->header.hDevice, RIDI_PREPARSEDDATA, NULL, &bufferSize);
	preparsedData = (PHIDP_PREPARSED_DATA)HeapAlloc(heap, 0, bufferSize);
	GetRawInputDeviceInfo(raw->header.hDevice, RIDI_PREPARSEDDATA, preparsedData, &bufferSize);

	HidP_GetCaps(preparsedData, &caps);

	std::cout << "Nr buttons: " << caps.NumberInputButtonCaps << std::endl;
	std::cout << "Nr indices: " << caps.NumberInputDataIndices << std::endl;
	std::cout << "Nr values: " << caps.NumberInputValueCaps << std::endl;

	HeapFree(heap, 0, preparsedData);
}

BOOL _stdcall DIEnumDevicesCallback(
	LPCDIDEVICEINSTANCE lpddi,
	LPVOID 
) {
	// only gets first gamepad/joystick (lazy solution)
	if (IsXInputDevice(&lpddi->guidProduct))
		return DIENUM_STOP;

	directInputDevices.push_back(lpddi->guidInstance);

	return DIENUM_STOP;
}
void setupDirectInput(BaseWindow * wnd) {
	//HRESULT res = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8W, (LPVOID*)&wnd->dinput8dev, NULL);
	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8W, (LPVOID*)&wnd->dinput8dev, NULL);

	wnd->dinput8dev->EnumDevices(DI8DEVCLASS_GAMECTRL, DIEnumDevicesCallback, nullptr, DIEDFL_ALLDEVICES);

	if (!directInputDevices.empty()) {
		wnd->dinput8dev->CreateDevice(directInputDevices[0], &wnd->inputDevice, nullptr);

		wnd->inputDevice->SetDataFormat(&c_dfDIJoystick2);
		wnd->inputDevice->SetCooperativeLevel(wnd->getWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	}
}

void enableXinput(bool enable) {
	XInputEnable(enable);
}

// @ TODO - Cleanup
void processXInput(BaseWindow * wnd) {
	wnd->inputProcessedByControllers = false;

	//XINPUT_CAPABILITIES caps;
	//
	//XInputGetCapabilities(0, 0, &caps);

	// makes sure xinput device is always priority
	if (wnd->controllerAxisCallback && wnd->controllerButtonCallback) {
		ControllerState controllerState;
		if (getControllerState(0, controllerState) == true) {
			if (XInputStateChangedThisFrame(&wnd->lastState, &controllerState)) {
				wnd->inputProcessedByControllers = true;
				//std::cout << "State changed!" << std::endl;

				//std::cout << "Button State: " << state.Gamepad.wButtons << std::endl;
				//std::cout << "Left Trigger: " << (int)state.Gamepad.bLeftTrigger << std::endl;
				//std::cout << "Right Trigger: " << (int)state.Gamepad.bRightTrigger << std::endl;
				//std::cout << "R Thumb X: " << (float)(state.Gamepad.sThumbRX) / 32767.0F << std::endl;
				//std::cout << "R Thumb Y: " << (float)(state.Gamepad.sThumbRY) / 32767.0F << std::endl;
				//std::cout << "L Thumb X: " << (float)(state.Gamepad.sThumbLX) / 32767.0F << std::endl;
				//std::cout << "L Thumb Y: " << (float)(state.Gamepad.sThumbLY) / 32767.0F << std::endl;

				//std::cout << "R Thumb X: " << state.rThumb.x * 255.0F << std::endl;
				//std::cout << "R Thumb Y: " << state.rThumb.y * 255.0F << std::endl;
				//std::cout << "L Thumb X: " << state.lThumb.x * 255.0F << std::endl;
				//std::cout << "L Thumb Y: " << state.lThumb.y * 255.0F << std::endl;

				wnd->controllerAxisCallback(wnd, GAMEPAD_AXIS_RX, controllerState.rThumb.x * 255.0F);
				wnd->controllerAxisCallback(wnd, GAMEPAD_AXIS_RY, controllerState.rThumb.y * 255.0F);
				wnd->controllerAxisCallback(wnd, GAMEPAD_AXIS_LX, controllerState.lThumb.x * 255.0F);
				wnd->controllerAxisCallback(wnd, GAMEPAD_AXIS_LY, controllerState.lThumb.y * 255.0F);
				wnd->controllerAxisCallback(wnd, GAMEPAD_AXIS_LT, controllerState.lTrigger);
				wnd->controllerAxisCallback(wnd, GAMEPAD_AXIS_RT, controllerState.rTrigger);

				//std::cout << std::hex << state.buttons ;
				//
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_A));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_B));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_X));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_Y));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_START));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_BACK));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_D_UP));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_D_DOWN));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_D_LEFT) );
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_D_RIGHT) );
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_BUMP_LEFT));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_BUMP_RIGHT));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_THUMB_LEFT));
				//std::cout << std::endl << ((state.buttons & XBoneMapping::BTN_THUMB_RIGHT));
				//std::cout << std::endl;

				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_A, (controllerState.buttons & XBoneMapping::BTN_A) == XBoneMapping::BTN_A);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_B, (controllerState.buttons & XBoneMapping::BTN_B) == XBoneMapping::BTN_B);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_X, (controllerState.buttons & XBoneMapping::BTN_X) == XBoneMapping::BTN_X);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_Y, (controllerState.buttons & XBoneMapping::BTN_Y) == XBoneMapping::BTN_Y);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_START, (controllerState.buttons & XBoneMapping::BTN_START) == XBoneMapping::BTN_START);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_BACK, (controllerState.buttons & XBoneMapping::BTN_BACK) == XBoneMapping::BTN_BACK);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_DP_UP, (controllerState.buttons & XBoneMapping::BTN_D_UP) == XBoneMapping::BTN_D_UP);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_DP_DOWN, (controllerState.buttons & XBoneMapping::BTN_D_DOWN) == XBoneMapping::BTN_D_DOWN);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_DP_LEFT, (controllerState.buttons & XBoneMapping::BTN_D_LEFT) == XBoneMapping::BTN_D_LEFT);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_DP_RIGHT, (controllerState.buttons & XBoneMapping::BTN_D_RIGHT) == XBoneMapping::BTN_D_RIGHT);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_LT, (controllerState.buttons & XBoneMapping::BTN_BUMP_LEFT) == XBoneMapping::BTN_BUMP_LEFT);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_RT, (controllerState.buttons & XBoneMapping::BTN_BUMP_RIGHT) == XBoneMapping::BTN_BUMP_RIGHT);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_L_THUMB, (controllerState.buttons & XBoneMapping::BTN_THUMB_LEFT) == XBoneMapping::BTN_THUMB_LEFT);
				wnd->controllerButtonCallback(wnd, GAMEPAD_BTN_R_THUMB, (controllerState.buttons & XBoneMapping::BTN_THUMB_RIGHT) == XBoneMapping::BTN_THUMB_RIGHT);
			}

			// return to prevent double controller processing
			return;
		}

		// legacy controller support
		if (wnd->inputDevice) {
			wnd->inputDevice->Acquire();

			wnd->inputDevice->Poll();

			DIJOYSTATE2 state { 0 };

			wnd->inputDevice->GetDeviceState(sizeof(DIJOYSTATE2), &state);

			int* dptr = (int*)&state;

			std::cout << "state: " << std::endl;

			for (size_t i = 0; i < sizeof(DIJOYSTATE2) / sizeof(int); i++) {
				std::cout << std::hex << (int)dptr[i] << " ";
				if (i % 4 == 0)
					std::cout << std::endl;
			}
			std::cout << std::endl;

			//std::cout << "Rx: " << state.lRx << std::endl;
			//std::cout << "Ry: " << state.lRy << std::endl;
			//std::cout << "Rz: " << state.lRz << std::endl;
			//std::cout << "x: " << state.lX << std::endl;
			//std::cout << "y: " << state.lY << std::endl;
			//std::cout << "z: " << state.lZ << std::endl;
			//
			//std::cout << "POW: " << state.rgdwPOV << std::endl;
			//std::cout << "slider: " << state.rglSlider << std::endl;
			//
			//std::cout << "buttons: ";
			//for (size_t i = 0; i < 32; i++) {
			//	std::cout << state.rgbButtons[i] << " ";
			//}
			//std::cout << std::endl;

			wnd->inputProcessedByControllers = true;
			return;
		}
	}

	//if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
	//	std::cout << "A button pressed" << std::endl;
	//}
}