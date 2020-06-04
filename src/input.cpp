#include "input.h"
#include "timer.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#ifdef __APPLE__
#include <Carbon/Carbon.h>
#else
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#define UNBOUND 200
#include <dinput.h>
#include <dinputd.h>
LPDIRECTINPUT8          g_pDI = NULL;
LPDIRECTINPUTDEVICE8    g_pJoystick = NULL;
struct DI_ENUM_CONTEXT
{
	DIJOYCONFIG* pPreferredJoyCfg;
	bool bPreferredJoyCfgValid;
};
extern const char* windowTitle;
#endif

bool use_controller = false;

extern bool safe_getline(ifstream &f, string& s);
extern bool has_menu();
timer tim2;

input::input() {
	time_index = tim2.create();
	tim2.update(time_index);
	repeater1 = tim2.create();
	tim2.update(repeater1);
	repeater2 = tim2.create();
	tim2.update(repeater2);
    for (unsigned i = 0; i < 16*16; ++i) {
        key_down[i] = false;
        key_press[i] = false;
    }
#ifdef __APPLE__
    i_up = 246;
    i_down = 245;
    i_left = 243;
    i_right = 244;
    i_confirm = 68;
    i_cancel = 101;
    i_start = 65;
    i_select = 54;
	ifstream f("../osx_keybinds.dat");
#else
	i_up = 38;
	i_down = 40;
	i_left = 37;
	i_right = 39;
	i_confirm = 13;
	i_cancel = 27;
	i_start = 219;
	i_select = 221;
	ifstream f("../windows_keybinds.dat");
#endif
	string line;
	u_up = i_up;
	u_down = i_down;
	u_right = i_right;
	u_left = i_left;
	u_start = i_start;
	u_select = i_select;
	u_confirm = i_confirm;
	u_cancel = i_cancel;
	if (!f.is_open()) {
		f.close();
	}
	else {
		safe_getline(f, line);
		i_up = stoi(line);
		safe_getline(f, line);
		i_down = stoi(line);
		safe_getline(f, line);
		i_left = stoi(line);
		safe_getline(f, line);
		i_right = stoi(line);
		safe_getline(f, line);
		i_confirm = stoi(line);
		safe_getline(f, line);
		i_cancel = stoi(line);
		safe_getline(f, line);
		i_start = stoi(line);
		safe_getline(f, line);
		i_select = stoi(line);
		f.close();
	}
	u_up = i_up;
	u_down = i_down;
	u_right = i_right;
	u_left = i_left;
	u_start = i_start;
	u_select = i_select;
	u_confirm = i_confirm;
	u_cancel = i_cancel;
#ifdef __APPLE__
	ifstream f2("../osx_user_keybinds.dat");
#else
	ifstream f2("../windows_user_keybinds.dat");
#endif
	if (!f2.is_open()) {
		f2.close();
	}
	else {
		safe_getline(f2, line);
		u_up = stoi(line);
		safe_getline(f2, line);
		u_down = stoi(line);
		safe_getline(f2, line);
		u_left = stoi(line);
		safe_getline(f2, line);
		u_right = stoi(line);
		safe_getline(f2, line);
		u_confirm = stoi(line);
		safe_getline(f2, line);
		u_cancel = stoi(line);
		safe_getline(f2, line);
		u_start = stoi(line);
		safe_getline(f2, line);
		u_select = stoi(line);
		f2.close();
	}
#ifdef __APPLE__
#else
	init_controller();
#endif
}

void input::tick() {
#ifdef __APPLE__
    GetKeys((BigEndianUInt32*) &keyMap);
    for (unsigned i = 0; i < 16; ++i)  {
        if (!key_down[(i*16) + 0] && (keyMap[i] & 1) != 0)
            key_press[(i*16) + 0] = true;
        key_down[(i*16) + 0] = (keyMap[i] & 1) != 0;
        if (!key_down[(i*16) + 1] && (keyMap[i] & 2) != 0)
            key_press[(i*16) + 1] = true;
        key_down[(i*16) + 1] = (keyMap[i] & 2) != 0;
        if (!key_down[(i*16) + 2] && (keyMap[i] & 4) != 0)
            key_press[(i*16) + 2] = true;
        key_down[(i*16) + 2] = (keyMap[i] & 4) != 0;
        if (!key_down[(i*16) + 3] && (keyMap[i] & 8) != 0)
            key_press[(i*16) + 3] = true;
        key_down[(i*16) + 3] = (keyMap[i] & 8) != 0;
        if (!key_down[(i*16) + 4] && (keyMap[i] & 16) != 0)
            key_press[(i*16) + 4] = true;
        key_down[(i*16) + 4] = (keyMap[i] & 16) != 0;
        if (!key_down[(i*16) + 5] && (keyMap[i] & 32) != 0)
            key_press[(i*16) + 5] = true;
        key_down[(i*16) + 5] = (keyMap[i] & 32) != 0;
        if (!key_down[(i*16) + 6] && (keyMap[i] & 64) != 0)
            key_press[(i*16) + 6] = true;
        key_down[(i*16) + 6] = (keyMap[i] & 64) != 0;
        if (!key_down[(i*16) + 7] && (keyMap[i] & 128) != 0)
            key_press[(i*16) + 7] = true;
        key_down[(i*16) + 7] = (keyMap[i] & 128) != 0;
    }
#else
	if (use_controller) {
		HRESULT hr;
		TCHAR strText[512] = { 0 }; // Device state text
		DIJOYSTATE2 cs;           // DInput controller state 
		hr = g_pJoystick->Acquire();
		hr = g_pJoystick->Poll();
		if (FAILED(hr)) {
			hr = g_pJoystick->Acquire();
		}
		else {
			if (FAILED(hr = g_pJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &cs))) {
				printf("\nUnable to get controller state.");
			}
			else {
				if (FAILED(g_pJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &cs))) {
					printf("\nUnable to get controller state.");
				}
				else {
					controller[0] = cs.lX;
					controller[1] = cs.lY;
					controller[2] = cs.lZ;
					controller[3] = cs.lRx;
					controller[4] = cs.lRy;
					controller[5] = cs.lRz;
					controller[6] = cs.rglSlider[0];
					controller[7] = cs.rglSlider[1];
					controller[8] = cs.lVX;
					controller[9] = cs.lVY;
					controller[10] = cs.lVZ;
					controller[11] = cs.lVRx;
					controller[12] = cs.lVRy;
					controller[13] = cs.lVRz;
					controller[14] = cs.rglVSlider[0];
					controller[15] = cs.rglVSlider[1];
					controller[16] = cs.lAX;
					controller[17] = cs.lAY;
					controller[18] = cs.lAZ;
					controller[19] = cs.lARx;
					controller[20] = cs.lARy;
					controller[21] = cs.lARz;
					controller[22] = cs.rglASlider[0];
					controller[23] = cs.rglASlider[1];
					controller[24] = cs.lFX;
					controller[25] = cs.lFY;
					controller[26] = cs.lFZ;
					controller[27] = cs.lFRx;
					controller[28] = cs.lFRy;
					controller[29] = cs.lFRz;
					controller[30] = cs.rglFSlider[0];
					controller[31] = cs.rglFSlider[1];
					controller[32] = cs.rgdwPOV[0];
					if (controller[32] != -1) {
						switch (controller[32]) {
						case 0:
							controller[32] = 1000;
							controller[33] = 0;
							controller[34] = 0;
							controller[35] = 0;
							break;
						case 4500:
							controller[32] = 1000;
							controller[33] = 1000;
							controller[34] = 0;
							controller[35] = 0;
							break;
						case 9000:
							controller[32] = 0;
							controller[33] = 1000;
							controller[34] = 0;
							controller[35] = 0;
							break;
						case 13500:
							controller[32] = 0;
							controller[33] = 1000;
							controller[34] = 1000;
							controller[35] = 0;
							break;
						case 18000:
							controller[32] = 0;
							controller[33] = 0;
							controller[34] = 1000;
							controller[35] = 0;
							break;
						case 22500:
							controller[32] = 0;
							controller[33] = 0;
							controller[34] = 1000;
							controller[35] = 1000;
							break;
						case 27000:
							controller[32] = 0;
							controller[33] = 0;
							controller[34] = 0;
							controller[35] = 1000;
							break;
						case 31500:
							controller[32] = 1000;
							controller[33] = 0;
							controller[34] = 0;
							controller[35] = 1000;
							break;
						default:
							break;
						}
					}
					else {
						controller[32] = 0;
						controller[33] = 0;
						controller[34] = 0;
						controller[35] = 0;
					}
					for (unsigned i = 0; i < 128; ++i) {
						controller[36 + i] = cs.rgbButtons[i] ? 1000 : 0;
					}
				}
			}
		}
	}
	for (unsigned i = 0; i < 16 * 16; ++i) {
		bool got_input;
		got_input = (GetAsyncKeyState(i) != 0) && (FindWindow(NULL, windowTitle) == GetForegroundWindow());
		if (use_controller) {
			if (i == i_up) {
				if (c_up != UNBOUND && c_up >= 0) {
					if (controller[c_up] > 500) {
						got_input = true;
					}
				}
				if (c_up != UNBOUND && c_up < 0) {
					if (controller[-(c_up + 1)] < -500) {
						got_input = true;
					}
				}
			}
			if (i == i_down) {
				if (c_down != UNBOUND && c_down >= 0) {
					if (controller[c_down] > 500) {
						got_input = true;
					}
				}
				if (c_down != UNBOUND && c_down < 0) {
					if (controller[-(c_down + 1)] < -500) {
						got_input = true;
					}
				}
			}
			if (i == i_left) {
				if (c_left != UNBOUND && c_left >= 0) {
					if (controller[c_left] > 500) {
						got_input = true;
					}
				}
				if (c_left != UNBOUND && c_left < 0) {
					if (controller[-(c_left + 1)] < -500) {
						got_input = true;
					}
				}
			}
			if (i == i_right) {
				if (c_right != UNBOUND && c_right >= 0) {
					if (controller[c_right] > 500) {
						got_input = true;
					}
				}
				if (c_right != UNBOUND && c_right < 0) {
					if (controller[-(c_right + 1)] < -500) {
						got_input = true;
					}
				}
			}
			if (i == i_confirm) {
				if (c_confirm != UNBOUND && c_confirm >= 0) {
					if (controller[c_confirm] > 500) {
						got_input = true;
					}
				}
				if (c_confirm != UNBOUND && c_confirm < 0) {
					if (controller[-(c_confirm + 1)] < -500) {
						got_input = true;
					}
				}
			}
			if (i == i_cancel) {
				if (c_cancel != UNBOUND && c_cancel >= 0) {
					if (controller[c_cancel] > 500) {
						got_input = true;
					}
				}
				if (c_cancel != UNBOUND && c_cancel < 0) {
					if (controller[-(c_cancel + 1)] < -500) {
						got_input = true;
					}
				}
			}
			if (i == i_start) {
				if (c_start != UNBOUND && c_start >= 0) {
					if (controller[c_start] > 500) {
						got_input = true;
					}
				}
				if (c_start != UNBOUND && c_start < 0) {
					if (controller[-(c_start + 1)] < -500) {
						got_input = true;
					}
				}
			}
			if (i == i_select) {
				if (c_select != UNBOUND && c_select >= 0) {
					if (controller[c_select] > 500) {
						got_input = true;
					}
				}
				if (c_select != UNBOUND && c_select < 0) {
					if (controller[-(c_select + 1)] < -500) {
						got_input = true;
					}
				}
			}
		}
		if (!key_down[i] && got_input)
			key_press[i] = true;
		key_down[i] = got_input;
	}
#endif
	if (has_menu()) {
		bool any_input = false;
		for (unsigned i = 0; i < 16 * 16 * 2; ++i) {
			if (key_down[i]) {
				any_input = true;
			}
		}
		if (any_input) {
			if (tim2.delta(repeater1) < 0.75) {
				tim2.update(repeater2);
			}
			else {
				if (tim2.delta(repeater2) > 0.1) {
					tim2.update(repeater2);
					for (unsigned i = 0; i < 16 * 16 * 2; ++i) {
						key_down[i] = false;
						key_press[i] = false;
					}
				}
			}
		}
		else {
			tim2.update(repeater1);
			tim2.update(repeater2);
		}
	}
	else {
		tim2.update(repeater1);
		tim2.update(repeater2);
	}
}

void input::keydown(bool &up, bool &down, bool &left, bool &right, bool &confirm, bool &cancel, bool &start, bool &select) {
    up = key_down[i_up] || key_down[u_up];
    down = key_down[i_down] || key_down[u_down];
    left = key_down[i_left] || key_down[u_left];
    right = key_down[i_right] || key_down[u_right];
    confirm = key_down[i_confirm] || key_down[u_confirm];
    cancel = key_down[i_cancel] || key_down[u_cancel];
    start = key_down[i_start] || key_down[u_start];
    select = key_down[i_select] || key_down[u_select];
}

void input::keypress(bool &up, bool &down, bool &left, bool &right, bool &confirm, bool &cancel, bool &start, bool &select) {
    up = key_press[i_up] || key_press[u_up];
    down = key_press[i_down] || key_press[u_down];
    left = key_press[i_left] || key_press[u_left];
    right = key_press[i_right] || key_press[u_right];
    confirm = key_press[i_confirm] || key_press[u_confirm];
    cancel = key_press[i_cancel] || key_press[u_cancel];
    start = key_press[i_start] || key_press[u_start];
    select = key_press[i_select] || key_press[u_select];
    for (unsigned i = 0; i < 16*16; ++i) {
        key_press[i] = false;
    }
}
int input::get_button_pressed(bool& pressed) {
	pressed = false;
#ifdef __APPLE__
#else
	if (use_controller) {
		for (int i = 0; i < 164; ++i) {
			if (controller[i] > 500) {
				pressed = true;
				return i;
			}
			else if (controller[i] < -500) {
				pressed = true;
				return (-i) - 1;
			}
		}
	}
#endif
	return 0;
}

unsigned char input::get_pressed_key(bool& pressed) {
	pressed = false;
	for (int i = 0; i < 16 * 16; ++i) {
		if (key_press[i]) {
			pressed = true;
			return char(i);
		}
	}
	return 0;
}

bool input::get_and_set_key(int mapping) {
	unsigned char key;
	int button;
	bool got_input, got_input2;
	got_input = false;
	got_input2 = false;
	tick();
	key = get_pressed_key(got_input);
#ifdef __APPLE__
#else
	if (use_controller)
		button = get_button_pressed(got_input2);
#endif
	while (got_input || got_input2) {
		tick();
		key = get_pressed_key(got_input);
#ifdef __APPLE__
#else
		if (use_controller)
			button = get_button_pressed(got_input2);
#endif
	}
	while (!got_input && !got_input2) {
		tick();
		key = get_pressed_key(got_input);
#ifdef __APPLE__
#else
		if (use_controller)
			button = get_button_pressed(got_input2);
#endif
	}
#ifdef __APPLE__
	return set_key(key, mapping);
#else
	if (got_input2)
		return set_button(button, mapping);	
	else
		return set_key(key, mapping);
#endif
}

bool input::set_button(int button, int mapping) {
	switch (mapping) {
	case 0:
		c_up = button;
		break;
	case 1:
		c_down = button;
		break;
	case 2:
		c_right = button;
		break;
	case 3:
		c_left = button;
		break;
	case 4:
		c_confirm = button;
		break;
	case 5:
		c_cancel = button;
		break;
	case 6:
		c_start = button;
		break;
	case 7:
		c_select = button;
		break;
	default:
		return false;
	}
	return true;
}


bool input::set_key(unsigned char key, int mapping) {
	if (mapping == 0) {
		if (key == i_down)
			return false;
		if (key == i_right)
			return false;
		if (key == i_left)
			return false;
		if (key == i_confirm)
			return false;
		if (key == i_cancel)
			return false;
		if (key == i_start)
			return false;
		if (key == i_select)
			return false;
	}
	if (mapping == 1) {
		if (key == i_up)
			return false;
		if (key == i_right)
			return false;
		if (key == i_left)
			return false;
		if (key == i_confirm)
			return false;
		if (key == i_cancel)
			return false;
		if (key == i_start)
			return false;
		if (key == i_select)
			return false;
	}
	if (mapping == 2) {
		if (key == i_up)
			return false;
		if (key == i_down)
			return false;
		if (key == i_left)
			return false;
		if (key == i_confirm)
			return false;
		if (key == i_cancel)
			return false;
		if (key == i_start)
			return false;
		if (key == i_select)
			return false;
	}
	if (mapping == 3) {
		if (key == i_up)
			return false;
		if (key == i_down)
			return false;
		if (key == i_right)
			return false;
		if (key == i_confirm)
			return false;
		if (key == i_cancel)
			return false;
		if (key == i_start)
			return false;
		if (key == i_select)
			return false;
	}
	if (mapping == 4) {
		if (key == i_up)
			return false;
		if (key == i_down)
			return false;
		if (key == i_right)
			return false;
		if (key == i_left)
			return false;
		if (key == i_cancel)
			return false;
		if (key == i_start)
			return false;
		if (key == i_select)
			return false;
	}
	if (mapping == 5) {
		if (key == i_up)
			return false;
		if (key == i_down)
			return false;
		if (key == i_right)
			return false;
		if (key == i_left)
			return false;
		if (key == i_confirm)
			return false;
		if (key == i_start)
			return false;
		if (key == i_select)
			return false;
	}
	if (mapping == 6) {
		if (key == i_up)
			return false;
		if (key == i_down)
			return false;
		if (key == i_right)
			return false;
		if (key == i_left)
			return false;
		if (key == i_confirm)
			return false;
		if (key == i_cancel)
			return false;
		if (key == i_select)
			return false;
	}
	if (mapping == 7) {
		if (key == i_up)
			return false;
		if (key == i_down)
			return false;
		if (key == i_right)
			return false;
		if (key == i_left)
			return false;
		if (key == i_confirm)
			return false;
		if (key == i_cancel)
			return false;
		if (key == i_start)
			return false;
	}
	switch (mapping) {
	case 0:
		u_up = key;
		break;
	case 1:
		u_down = key;
		break;
	case 2:
		u_right = key;
		break;
	case 3:
		u_left = key;
		break;
	case 4:
		u_confirm = key;
		break;
	case 5:
		u_cancel = key;
		break;
	case 6:
		u_start = key;
		break;
	case 7:
		u_select = key;
		break;
	default:
		return false;
	}
	return true;
}

void input::save_bindings() {
#ifdef __APPLE__
	ofstream f("../osx_user_keybinds.dat");
#else
	ofstream f("../windows_user_keybinds.dat");
#endif
	f << int(u_up) << string("\n");
	f << int(u_down) << string("\n");
	f << int(u_left) << string("\n");
	f << int(u_right) << string("\n");
	f << int(u_confirm) << string("\n");
	f << int(u_cancel) << string("\n");
	f << int(u_start) << string("\n");
	f << int(u_select);
	f.close();
#ifdef __APPLE__
#else
	ofstream f2("../controller_keybinds.dat");
	f2 << int(c_up) << string("\n");
	f2 << int(c_down) << string("\n");
	f2 << int(c_left) << string("\n");
	f2 << int(c_right) << string("\n");
	f2 << int(c_confirm) << string("\n");
	f2 << int(c_cancel) << string("\n");
	f2 << int(c_start) << string("\n");
	f2 << int(c_select);
	f2.close();
#endif
}

#ifdef __APPLE__
#else
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance,
	VOID* pContext)
{
	DI_ENUM_CONTEXT* pEnumContext = (DI_ENUM_CONTEXT*)pContext;
	HRESULT hr;
	if (pEnumContext->bPreferredJoyCfgValid &&
		!IsEqualGUID(pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance))
		return DIENUM_CONTINUE;
	hr = g_pDI->CreateDevice(pdidInstance->guidInstance, &g_pJoystick, NULL);
	if (FAILED(hr))
		return DIENUM_CONTINUE;
	return DIENUM_STOP;
}

BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi,
	VOID* pContext)
{
	HWND hDlg = (HWND)pContext;

	static int nSliderCount = 0;
	static int nPOVCount = 0;
	if (pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin = -1000;
		diprg.lMax = +1000;
		if (FAILED(g_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph)))
			return DIENUM_STOP;

	}
	return DIENUM_CONTINUE;
}

#endif

void input::init_controller() {
#ifdef __APPLE__
#else
	c_up = UNBOUND;
	c_down = UNBOUND;
	c_right = UNBOUND;
	c_left = UNBOUND;
	c_confirm = UNBOUND;
	c_cancel = UNBOUND;
	c_start = UNBOUND;
	c_select = UNBOUND;
	HRESULT hr;
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDI, NULL))) {
		printf("Unable to create DirectInput8 device.  Contining without controller.\n");
		return;
	}

	DIJOYCONFIG PreferredJoyCfg = { 0 };
	DI_ENUM_CONTEXT enumContext;
	enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
	enumContext.bPreferredJoyCfgValid = false;

	IDirectInputJoyConfig8* pJoyConfig = NULL;
	if (FAILED(hr = g_pDI->QueryInterface(IID_IDirectInputJoyConfig8, (void**)&pJoyConfig))) {
		printf("Unable to query interface.  Continuing without controller.\n");
		return;
	}
	PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
	if (SUCCEEDED(pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE)))
		enumContext.bPreferredJoyCfgValid = true;
	if (pJoyConfig) {
		pJoyConfig->Release();
		pJoyConfig = NULL;
	}
	if (FAILED(hr = g_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, &enumContext, DIEDFL_ALLDEVICES))) {
		printf("Unable to enumerate devices.  Continuing without controller.\n");
		return;
	}
	if (g_pJoystick == NULL) {
		printf("Unable to find controller.  Continuing without controller.\n");
		return;
	}
	if (FAILED(g_pJoystick->SetDataFormat(&c_dfDIJoystick2))) {
		printf("Unable to set controller format.  Continuing without controller.\n");
		return;
	}
	if (FAILED(g_pJoystick->EnumObjects(EnumObjectsCallback, (VOID*)0, DIDFT_ALL))) {
		printf("Unable to enumerate controller's objects.  Continuing without controller.\n");
		return;
	}
	printf("Controller detected.\n");
	use_controller = true;
	ifstream f("../controller_keybinds.dat");
	string line;
	if (!f.is_open()) {
		f.close();
	}
	else {
		safe_getline(f, line);
		c_up = stoi(line);
		safe_getline(f, line);
		c_down = stoi(line);
		safe_getline(f, line);
		c_left = stoi(line);
		safe_getline(f, line);
		c_right = stoi(line);
		safe_getline(f, line);
		c_confirm = stoi(line);
		safe_getline(f, line);
		c_cancel = stoi(line);
		safe_getline(f, line);
		c_start = stoi(line);
		safe_getline(f, line);
		c_select = stoi(line);
		f.close();
	}
#endif
}

input::~input() {
#ifdef __APPLE__
#else
	if (use_controller)
		g_pJoystick->Unacquire();
#endif
}
