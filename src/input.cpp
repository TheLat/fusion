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
	if (!f.is_open()) {
		f.close();
		return;
	}
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
    up = key_down[i_up];
    down = key_down[i_down];
    left = key_down[i_left];
    right = key_down[i_right];
    confirm = key_down[i_confirm];
    cancel = key_down[i_cancel];
    start = key_down[i_start];
    select = key_down[i_select];
}

void input::keypress(bool &up, bool &down, bool &left, bool &right, bool &confirm, bool &cancel, bool &start, bool &select) {
    up = key_press[i_up];
    down = key_press[i_down];
    left = key_press[i_left];
    right = key_press[i_right];
    confirm = key_press[i_confirm];
    cancel = key_press[i_cancel];
    start = key_press[i_start];
    select = key_press[i_select];
    for (unsigned i = 0; i < 16*16; ++i) {
        key_press[i] = false;
    }
}