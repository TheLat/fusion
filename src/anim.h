#ifndef ANIM_HEADER
#define ANIM_HEADER

#include <vector>
#include <mutex>
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
	vector<animation_int> animi;
public:
	unsigned create_animf(double* targ, double start, double end, double duration);
	unsigned create_animi(int* targ, int start, int end, double duration);
	void tick(double delta);
	bool is_donef(unsigned index);
	bool is_donei(unsigned index);
	void purge();
};

#endif