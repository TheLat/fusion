#include "timer.h"
#include <string>
#include <vector>
#include <fstream>
extern void log(const char *format, const char *value = 0);
#ifdef __APPLE__
#include <sys/time.h>
#endif
#ifdef __SWITCH__
#include <switch.h> 
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
vector<timeval> holders;
#endif
#ifdef __SWITCH__
vector<u64> holders;
#endif
#ifdef _WIN32
vector<LARGE_INTEGER> holders;
LARGE_INTEGER freq;
#endif

extern bool safe_getline(ifstream &f, string& s);

timer::timer() {
	string line;
	speedup = 1.0;
	ifstream f((string("../speedup.txt")));
	if (f.is_open()) {
		safe_getline(f, line);
		speedup = stof(line);
	}
	f.close();
#ifdef _WIN32
	QueryPerformanceFrequency(&freq);
#endif
}

unsigned timer::create() {
    unsigned ret = holders.size();
#ifdef __APPLE__
    timeval temp;
    gettimeofday(&temp, NULL);
#endif
#ifdef __SWITCH__
	u64 temp = armGetSystemTick();
#endif
#ifdef _WIN32
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
#endif
	holders.push_back(temp);
	return ret;
}

void timer::update(unsigned index) {
#ifdef __APPLE__
    gettimeofday(&(holders[index]), NULL);
#endif
#ifdef __SWITCH__
	holders[index] = armGetSystemTick();
#endif
#ifdef _WIN32
	QueryPerformanceCounter(&(holders[index]));
#endif
}

double timer::delta(unsigned index) {
#ifdef __APPLE__
    timeval temp;
    gettimeofday(&temp, NULL);
    double f = (temp.tv_sec - holders[index].tv_sec);
    f += (temp.tv_usec - holders[index].tv_usec)/1000000.0;
    return f * speedup;
#endif
#ifdef __SWITCH__
	u64 elapsed = armGetSystemTick() - holders[index];
	return (elapsed * 625.0 / 12.0) / 1000000000.0;
#endif
#ifdef _WIN32
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	double f = 1.0 / freq.QuadPart;
	f = (temp.QuadPart - holders[index].QuadPart) * f;
	return f * speedup;
#endif
}

unsigned timer::get_current_time() {
#ifdef __APPLE__
	timeval temp;
	gettimeofday(&temp, NULL);
	double f = temp.tv_sec;
	f += temp.tv_usec / 1000000.0;
#endif
#ifdef __SWITCH__
	double f = time(NULL);
	f /= 10000000.0;
#endif
#ifdef _WIN32
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	double f = 1.0 / freq.QuadPart;
	f = (temp.QuadPart) * f;
#endif
	return unsigned(f*1000000.0);
}