#include "anim.h"

unsigned animation_engine::create_animf(double* targ, double start, double end, double duration) {
	animation_float temp;
	temp.start = start;
	temp.end = end;
	temp.duration = duration;
	temp.ftarg = targ;
	temp.time = 0.0;
	temp.done = false;
	if (start == end)
		temp.done = true;
	animf.push_back(temp);
	return animf.size() - 1;
}

unsigned animation_engine::create_animi(int* targ, int start, int end, double duration) {
	animation_int temp;
	temp.start = start;
	temp.end = end;
	temp.duration = duration;
	temp.itarg = targ;
	temp.time = 0.0;
	temp.done = false;
	if (start == end)
		temp.done = true;
	animi.push_back(temp);
	return animi.size() - 1;
}

void animation_engine::tick(double delta) {
	unsigned i;
	for (i = 0; i < animf.size(); ++i) {
		if (animf[i].done)
			continue;
		animf[i].time += delta;
		if (animf[i].time > animf[i].duration)
			animf[i].time = animf[i].duration;
		*(animf[i].ftarg) = animf[i].start + ((animf[i].time / animf[i].duration) * (animf[i].end - animf[i].start));
		if (animf[i].time == animf[i].duration)
			animf[i].done = true;
	}
	for (i = 0; i < animi.size(); ++i) {
		if (animi[i].done)
			continue;
		animi[i].time += delta;
		if (animi[i].time > animi[i].duration)
			animi[i].time = animi[i].duration;
		*(animi[i].itarg) = int(double(animi[i].start) + ((animi[i].time / animi[i].duration) * (double(animi[i].end - animi[i].start))));
		if (animi[i].time == animi[i].duration)
			animi[i].done = true;
	}
	purge();
}

bool animation_engine::is_donef(unsigned index) {
	if (index >= animf.size()) {
		return true;
	}
	return animf[index].done;
}

bool animation_engine::is_donei(unsigned index) {
	if (index >= animi.size()) {
		return true;
	}
	return animi[index].done;
}

void animation_engine::purge() {
	bool found;
	found = false;
	for (unsigned i = 0; i < animf.size(); ++i) {
		if (!animf[i].done) {
			found = true;
			break;
		}
	}
	if (!found) {
		animf.empty();
	}
	found = false;
	for (unsigned i = 0; i < animi.size(); ++i) {
		if (!animi[i].done) {
			found = true;
			break;
		}
	}
	if (!found) {
		animi.empty();
	}
}