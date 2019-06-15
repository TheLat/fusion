#ifndef ANIM_HEADER
#define ANIM_HEADER

#include <vector>
#include "timer.h"


class animation_int {
public:
	int* itarg;
	int start, end;
	bool wrap, done;
	double duration;
	double time;
};

class animation_float {
public:
	double* ftarg;
	double start, end;
	bool wrap, done;
	double duration;
	double time;
};

class animation_engine {
private:
	vector<animation_float> animf;
public:
	unsigned create_animf(double* targ, double start, double end, double duration, bool wrap);
	void tick(double delta);
	bool is_done(unsigned index);
};

#endif