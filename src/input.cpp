#include "input.h"
#include <vector>
#ifdef __APPLE__
#include <Carbon/Carbon.h>
#else
// SOMETHING
#endif

input::input() {
    for (unsigned i = 0; i < 16*16; ++i) {
        keyMapBool[i] = false;
    }
}

void input::tick() {
    GetKeys((BigEndianUInt32*) &keyMap);
    for (unsigned i = 0; i < 16; ++i)  {
        keyMapBool[(i*16) + 0] = (keyMap[i] & 1) != 0;
        keyMapBool[(i*16) + 1] = (keyMap[i] & 2) != 0;
        keyMapBool[(i*16) + 2] = (keyMap[i] & 4) != 0;
        keyMapBool[(i*16) + 3] = (keyMap[i] & 8) != 0;
        keyMapBool[(i*16) + 4] = (keyMap[i] & 16) != 0;
        keyMapBool[(i*16) + 5] = (keyMap[i] & 32) != 0;
        keyMapBool[(i*16) + 6] = (keyMap[i] & 64) != 0;
        keyMapBool[(i*16) + 7] = (keyMap[i] & 128) != 0;
    }
}