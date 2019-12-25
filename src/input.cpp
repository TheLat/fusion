#include "input.h"
#include "timer.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#ifdef __APPLE__
#include <Carbon/Carbon.h>
#else
#include <Windows.h>
#endif

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
