#ifndef TIMER_HEADER
#define TIMER_HEADER

using namespace std;

class timer {
	double speedup;
public:
	unsigned create();
	void update(unsigned index);
	double delta(unsigned index);
	unsigned get_current_time();
	timer();
};

#endif