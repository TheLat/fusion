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
#include <dinput.h>
#include <dinputd.h>
LPDIRECTINPUT8          g_pDI = NULL;
LPDIRECTINPUTDEVICE8    g_pJoystick = NULL;
struct DI_ENUM_CONTEXT
{
	DIJOYCONFIG* pPreferredJoyCfg;
	bool bPreferredJoyCfgValid;
};

#endif

bool use_controller = false;

extern bool safe_getline(ifstream &f, string& s);
timer tim2;

input::input() {
    time_index = tim2.create();
    tim2.update(time_index);
    for (unsigned i = 0; i < 16*16; ++i) {
        key_down[i] = false;
        key_press[i] = false;
    }
#ifdef __APPLE__
    i_up = 246;
    i_down = 245;
    i_left = 243;
    i_right = 244;
    i_confirm = 0;
    i_cancel = 1;
    i_start = 6;
    i_select = 7;
	ifstream f("../osx_keybinds.dat");
#else
	i_up = 38;
	i_down = 40;
	i_left = 37;
	i_right = 39;
	i_confirm = 65;
	i_cancel = 83;
	i_start = 90;
	i_select = 88;
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
		if (!key_down[i] && GetAsyncKeyState(i) != 0)
			key_press[i] = true;
		key_down[i] = GetAsyncKeyState(i) != 0;
	}
#endif
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

unsigned char input::get_pressed_key(bool& pressed) {
	tick();
	pressed = false;
	for (int i = 0; i < 16 * 16; ++i) {
		if (key_press[i]) {
			pressed = true;
			return unsigned char(i);
		}
	}
	return 0;
}

bool input::get_and_set_key(int mapping) {
	unsigned char key;
	bool got_input;
	key = get_pressed_key(got_input);
	while (got_input) {
		key = get_pressed_key(got_input);
	}
	while (!got_input) {
		key = get_pressed_key(got_input);
	}
	return set_key(key, mapping);
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
}

#ifdef __APPLE__
#else
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance,
	VOID* pContext)
{
	DI_ENUM_CONTEXT* pEnumContext = (DI_ENUM_CONTEXT*)pContext;
	HRESULT hr;

	// Skip anything other than the perferred Joystick device as defined by the control panel.  
	// Instead you could store all the enumerated Joysticks and let the user pick.
	if (pEnumContext->bPreferredJoyCfgValid &&
		!IsEqualGUID(pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance))
		return DIENUM_CONTINUE;

	// Obtain an interface to the enumerated Joystick.
	hr = g_pDI->CreateDevice(pdidInstance->guidInstance, &g_pJoystick, NULL);

	// If it failed, then we can't use this Joystick. (Maybe the user unplugged
	// it while we were in the middle of enumerating it.)
	if (FAILED(hr))
		return DIENUM_CONTINUE;

	// Stop enumeration. Note: we're just taking the first Joystick we get. You
	// could store all the enumerated Joysticks and let the user pick.
	return DIENUM_STOP;
}

BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi,
	VOID* pContext)
{
	HWND hDlg = (HWND)pContext;

	static int nSliderCount = 0;  // Number of returned slider controls
	static int nPOVCount = 0;     // Number of returned POV controls

								  // For axes that are returned, set the DIPROP_RANGE property for the
								  // enumerated axis in order to scale min/max values.
	if (pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin = -1000;
		diprg.lMax = +1000;

		// Set the range for the axis
		if (FAILED(g_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph)))
			return DIENUM_STOP;

	}
	return DIENUM_CONTINUE;
}

#endif

void input::init_controller() {
#ifdef __APPLE__
#else
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
	// TODO: Controller input bindings
	use_controller = true;
#endif
}

input::~input() {
#ifdef __APPLE__
#else
	if (use_controller)
		g_pJoystick->Unacquire();
#endif
}