#ifndef TIMER_HEADER
#define TIMER_HEADER

using namespace std;

class timer {
public:
	unsigned create();
	void update(unsigned index);
	double delta(unsigned index);
	timer();
};

#endif