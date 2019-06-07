#include "timer.h"
#include <windows.h>
#include <vector>


vector<LARGE_INTEGER> holders;
LARGE_INTEGER freq;

timer::timer() {
	QueryPerformanceFrequency(&freq);
}

unsigned timer::create() {
	unsigned ret = holders.size();
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	holders.push_back(temp);
	return ret;
}

void timer::update(unsigned index) {
	QueryPerformanceCounter(&(holders[index]));
}

double timer::delta(unsigned index) {
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	double f = 1.0 / freq.QuadPart;
	f = (temp.QuadPart - holders[index].QuadPart) * f;
	return f;
}