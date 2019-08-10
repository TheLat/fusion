#include "input.h"
#include "timer.h"
#ifdef __APPLE__
#include <Carbon/Carbon.h>
#else
// SOMETHING
#endif
timer tim2;

input::input() {
    time_index = tim2.create();
    tim2.update(time_index);
    for (unsigned i = 0; i < 16*16; ++i) {
        key_down[i] = false;
        key_press[i] = false;
    }
    i_up = 246;
    i_down = 245;
    i_left = 243;
    i_right = 244;
    i_confirm = 0;
    i_cancel = 1;
    i_start = 6;
    i_select = 7;
}

void input::tick() {
    double deltat = tim2.delta(time_index);
	while (deltat < 1.0/120.0)
		deltat = tim2.delta(time_index);
	tim2.update(time_index);
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