#ifndef SCAN_CODES_HPP
#define SCAN_CODES_HPP

namespace InputMode {
	const int KEYBOARD_INPUT = 0;
	const int MOUSE_INPUT = 1;
	const int GAMEPAD_INPUT = 2;
}

namespace Mouse {
	const int MOUSE_NONE = -1;
	const int MOUSE_CLICK_1 = 0;
	const int MOUSE_CLICK_2 = 1;
	const int MOUSE_CLICK_3 = 2;
	const int MOUSE_CLICK_4 = 3;
	const int MOUSE_CLICK_5 = 4;

	const int MOUSE_SCROLL_1 = 5;
	const int MOUSE_SCROLL_2 = 6;
}

namespace ScanCodes {

	const int SCAN_CODE_NONE = -1;

	const int SCAN_CODE_ESC = 1;
	const int SCAN_CODE_1 = 2;
	const int SCAN_CODE_2 = 3;
	const int SCAN_CODE_3 = 4;
	const int SCAN_CODE_4 = 5;
	const int SCAN_CODE_5 = 6;
	const int SCAN_CODE_6 = 7;
	const int SCAN_CODE_7 = 8;
	const int SCAN_CODE_8 = 9;
	const int SCAN_CODE_9 = 10;
	const int SCAN_CODE_0 = 11;
	const int SCAN_CODE_PLUS = 12;
	const int SCAN_CODE_BSLHS = 13;
	const int SCAN_CODE_BCKSPACE = 14;

	const int SCAN_CODE_A = 30;
	const int SCAN_CODE_S = 31;
	const int SCAN_CODE_D = 32;
	const int SCAN_CODE_F = 33;
	const int SCAN_CODE_G = 34;
	const int SCAN_CODE_H = 35;
	const int SCAN_CODE_J = 36;
	const int SCAN_CODE_K = 37;
	const int SCAN_CODE_L = 38;
	const int SCAN_CODE_Ö = 39;
	const int SCAN_CODE_Ä = 40;
	const int SCAN_CODE_STAR = 47;

	const int SCAN_CODE_ENTER = 28;

	const int SCAN_CODE_Q = 16;
	const int SCAN_CODE_W = 17;
	const int SCAN_CODE_E = 18;
	const int SCAN_CODE_R = 19;
	const int SCAN_CODE_T = 20;
	const int SCAN_CODE_Y = 21;
	const int SCAN_CODE_U = 22;
	const int SCAN_CODE_I = 23;
	const int SCAN_CODE_O = 24;
	const int SCAN_CODE_P = 25;
	const int SCAN_CODE_Å = 26;
	const int SCAN_CODE_EXP = 27;


	const int SCAN_CODE_TAB = 15;
}

namespace GamePad {
	const int BTN_NONE = -1;

	const int BTN_A = 1;
	const int BTN_B = 2;
	const int BTN_X = 3;
	const int BTN_Y = 4;

	const int BTN_BACK = 5;
	const int BTN_START = 6;
	
	const int BTN_D_UP = 7;
	const int BTN_D_LEFT = 8;
	const int BTN_D_RIGHT = 9;
	const int BTN_D_DOWN = 10;

	const int BTN_BUMP_RIGHT = 11;
	const int BTN_BUMP_LEFT = 12;

	const int BTN_THUMB_RIGHT = 13;
	const int BTN_THUMB_LEFT = 14;

	const int AXIS_R_THUMB_X = 15;
	const int AXIS_R_THUMB_Y = 16;
	const int AXIS_L_THUMB_X = 17;
	const int AXIS_L_THUMB_Y = 18;
	const int AXIS_TRIG_L = 19;
	const int AXIS_TRIG_R = 20;
}

namespace MoveDirections {
	const int DIR_NONE = 0;
	const int DIR_POS = 1;
	const int DIR_NEG = -1;
}

namespace ModifierKeys {

	const int MOD_KEY_SHIFT = 1;
	const int MOD_KEY_CTRL = 2;
	const int MOD_KEY_ALT = 4;
	const int MOD_KEY_SUPER = 8;

}

#endif