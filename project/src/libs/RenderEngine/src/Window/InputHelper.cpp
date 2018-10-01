#include "InputHelper.hpp"

#include <wbemidl.h>
#include <oleauto.h>
#include <cwchar>
#include <cmath>

#include <glm/glm.hpp>

#include <iostream>

template <typename T>
void SafeRealease(T** p) {
	if (*p)
		(*p)->Release();
	*p = 0;
}

BOOL IsXInputDevice(const GUID * pGuidProductFromDirectInput) {
	IWbemLocator*           pIWbemLocator = NULL;
	IEnumWbemClassObject*   pEnumDevices = NULL;
	IWbemClassObject*       pDevices[20] = { 0 };
	IWbemServices*          pIWbemServices = NULL;
	BSTR                    bstrNamespace = NULL;
	BSTR                    bstrDeviceID = NULL;
	BSTR                    bstrClassName = NULL;
	DWORD                   uReturned = 0;
	bool                    bIsXinputDevice = false;
	UINT                    iDevice = 0;
	VARIANT                 var;
	HRESULT                 hr;

	// CoInit if needed
	hr = CoInitialize(NULL);
	bool bCleanupCOM = SUCCEEDED(hr);

	// Create WMI
	hr = CoCreateInstance(__uuidof(WbemLocator),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IWbemLocator),
		(LPVOID*)&pIWbemLocator);
	if (FAILED(hr) || pIWbemLocator == NULL)
		goto LCleanup;

	bstrNamespace = SysAllocString(L"\\\\.\\root\\cimv2"); if (bstrNamespace == NULL) goto LCleanup;
	bstrClassName = SysAllocString(L"Win32_PNPEntity");   if (bstrClassName == NULL) goto LCleanup;
	bstrDeviceID = SysAllocString(L"DeviceID");          if (bstrDeviceID == NULL)  goto LCleanup;

	// Connect to WMI
	hr = pIWbemLocator->ConnectServer(bstrNamespace, NULL, NULL, 0L,
		0L, NULL, NULL, &pIWbemServices);
	if (FAILED(hr) || pIWbemServices == NULL)
		goto LCleanup;

	// Switch security level to IMPERSONATE.
	CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

	hr = pIWbemServices->CreateInstanceEnum(bstrClassName, 0, NULL, &pEnumDevices);
	if (FAILED(hr) || pEnumDevices == NULL)
		goto LCleanup;

	// Loop over all devices
	for (;; ) {
		// Get 20 at a time
		hr = pEnumDevices->Next(10000, 20, pDevices, &uReturned);
		if (FAILED(hr))
			goto LCleanup;
		if (uReturned == 0)
			break;

		for (iDevice = 0; iDevice < uReturned; iDevice++) {
			// For each device, get its device ID
			hr = pDevices[iDevice]->Get(bstrDeviceID, 0L, &var, NULL, NULL);
			if (SUCCEEDED(hr) && var.vt == VT_BSTR && var.bstrVal != NULL) {
				// Check if the device ID contains "IG_".  If it does, then it's an XInput device
				// This information can not be found from DirectInput
				if (wcsstr(var.bstrVal, L"IG_")) {
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr(var.bstrVal, L"VID_");
					if (strVid && swscanf_s(strVid, L"VID_%4X", &dwVid) != 1)
						dwVid = 0;
					WCHAR* strPid = wcsstr(var.bstrVal, L"PID_");
					if (strPid && swscanf_s(strPid, L"PID_%4X", &dwPid) != 1)
						dwPid = 0;

					// Compare the VID/PID to the DInput device
					DWORD dwVidPid = MAKELONG(dwVid, dwPid);
					if (dwVidPid == pGuidProductFromDirectInput->Data1) {
						bIsXinputDevice = true;
						goto LCleanup;
					}
				}
			}
			VariantClear(&var);
			SafeRealease(&pDevices[iDevice]);
		}
	}

LCleanup:
	if (bstrNamespace)
		SysFreeString(bstrNamespace);
	if (bstrDeviceID)
		SysFreeString(bstrDeviceID);
	if (bstrClassName)
		SysFreeString(bstrClassName);
	for (iDevice = 0; iDevice < 20; iDevice++)
		SafeRealease(&pDevices[iDevice]);
	SafeRealease(&pEnumDevices);
	SafeRealease(&pIWbemLocator);
	SafeRealease(&pIWbemServices);

	if (bCleanupCOM)
		CoUninitialize();

	return bIsXinputDevice;
}

bool getControllerState(int , ControllerState& state) {

	XINPUT_STATE xState;
	if (XInputGetState(0, &xState) == ERROR_SUCCESS) {

		state.buttons = xState.Gamepad.wButtons;
		state.lTrigger = xState.Gamepad.bLeftTrigger;
		state.rTrigger = xState.Gamepad.bRightTrigger;

		state.lThumb = glm::vec2(xState.Gamepad.sThumbLX, xState.Gamepad.sThumbLY) / 32767.0F;
		state.rThumb = glm::vec2(xState.Gamepad.sThumbRX, xState.Gamepad.sThumbRY) / 32767.0F;

		return true;
	}

	return false;
}

// @ TODO - Cleanup

bool RadialDeadZoneCompare(glm::vec2& axis, glm::vec2 oldAxis, float deadzone) {

	float newAxisLenAbs = glm::abs(glm::length(axis));
	float oldAxisLenAbs = glm::abs(glm::length(oldAxis));

	//printf("D: %f - %f =%f\n", newAxisLenAbs, oldAxisLenAbs, newAxisLenAbs - oldAxisLenAbs);
	if (newAxisLenAbs > deadzone) {
		axis = glm::normalize(axis) * ((glm::length(axis) - deadzone) / (1.0F - deadzone));
		return true;
	}

	axis = glm::vec2(0);
	if (oldAxisLenAbs > deadzone && newAxisLenAbs < deadzone) {
		return true;
	}
	

	return false;
}

// @ TODO - Cleanup
bool OutsideDeadZoneCompare(SHORT oldValue, SHORT &newValue, SHORT deadzone) {

	INT oldAbs = abs(oldValue);
	INT newAbs = abs(newValue);

	// new value outsize of deadzone detect as input
	if (newAbs > deadzone) {
		return true;
	}
	if (oldAbs > deadzone && newAbs < deadzone) {
		newValue = 0;
		return true;
	}

	newValue = 0;
	// no change
	return false;
}

// @ TODO - Cleanup
bool XInputStateChangedThisFrame(ControllerState* oldState, ControllerState* newState) {
	bool buttonStateChange = false;
	bool axisStateChange = false;
	bool completeStateChange = false;

	if (oldState->buttons != newState->buttons) {
		buttonStateChange = true;
	}

	if (oldState->lTrigger != oldState->lTrigger) {
		axisStateChange = true;
	}
	if (oldState->rTrigger != oldState->rTrigger) {
		axisStateChange = true;
	}

	const float deadzone = 0.25F;

	if (RadialDeadZoneCompare(newState->rThumb, oldState->rThumb, deadzone)) {
		axisStateChange = true;
	}

	if (RadialDeadZoneCompare(newState->lThumb, oldState->lThumb, deadzone)) {
		axisStateChange = true;
	}

	if (isnan(newState->rThumb.x)) {
		throw "fatal error";
	}


	//if (OutsideDeadZoneCompare(oState.sThumbLX, nState.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {
	//	axisStateChange = true;
	//}
	//if (OutsideDeadZoneCompare(oState.sThumbLY, nState.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {
	//	axisStateChange = true;
	//}
	//
	//if (OutsideDeadZoneCompare(oState.sThumbRX, nState.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) {
	//	axisStateChange = true;
	//}
	//if (OutsideDeadZoneCompare(oState.sThumbRY, nState.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) {
	//	axisStateChange = true;
	//}

	newState->lastSent = (buttonStateChange || axisStateChange);

	if (oldState->lastSent != newState->lastSent) {
		completeStateChange = true;
	}

	*oldState = *newState;

	//newState->Gamepad = nState;
	//oldState->Gamepad = newState->Gamepad;
	return (buttonStateChange || axisStateChange || completeStateChange);
}
