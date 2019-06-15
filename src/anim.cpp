#include "anim.h"

unsigned animation_engine::create_animf(double* targ, double start, double end, double duration, bool wrap) {
	animation_float temp;
	temp.start = start;
	temp.end = end;
	temp.duration = duration;
	temp.ftarg = targ;
	temp.time = 0.0;
	temp.wrap = wrap;
	temp.done = false;
	if (start == end)
		temp.done = true;
	animf.push_back(temp);
	return animf.size() - 1;
}

void animation_engine::tick(double delta) {
	printf("\nCalling anim tick with %f", delta);
	for (unsigned i = 0; i < animf.size(); ++i) {
		if (animf[i].done)
			continue;
		animf[i].time += delta;
		if (animf[i].time > animf[i].duration)
			animf[i].time = animf[i].duration;
		*(animf[i].ftarg) = animf[i].start + ((animf[i].time / animf[i].duration) * (animf[i].end - animf[i].start));
		if (animf[i].time == animf[i].duration)
			animf[i].done = true;
	}
}

bool animation_engine::is_done(unsigned index) {
	return animf[index].done;
}