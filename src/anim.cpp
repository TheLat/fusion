#include "anim.h"

mutex safetyi, safetyf, safetys;

extern bool safe_getline(ifstream &f, string& s);

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
	safetyf.lock();
	animf.push_back(temp);
	safetyf.unlock();
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
	safetyi.lock();
	animi.push_back(temp);
	safetyi.unlock();
	return animi.size() - 1;
}

unsigned animation_engine::create_anim_scene(string scene, unsigned sprite1, unsigned sprite2) {
    animation_scene temp;
    scene_element elem;
    string line, s;
    double t = 0.0;
    temp.current_time = 0.0;
    printf("\nCreating animation %s", scene.c_str());
    ifstream f((string("../resources/animations/") + scene + string(".dat")).c_str());
    while (f.is_open()) {
        while (safe_getline(f, line)) {
            printf("\nGetting line: %s", line.c_str());
            s = line;
            s.erase(s.find(" "), s.length());
            line.erase(0, line.find(" ") + 1);
            if (s == "MUTE_MUSIC") {
                elem.type = MUTE_MUSIC;
                if (line.find("+") != -1) {
                    line.erase(line.find("+"), 1);
                    elem.start = stof(line) + t;
                }
                else {
                    elem.start = stof(line);
                }
                t = elem.start;
                temp.elements.push_back(elem);
                continue;
            }
            if (s == "UNMUTE_MUSIC") {
                elem.type = UNMUTE_MUSIC;
                if (line.find("+") != -1) {
                    line.erase(line.find("+"), 1);
                    elem.start = stof(line) + t;
                }
                else {
                    elem.start = stof(line);
                }
                t = elem.start;
                temp.elements.push_back(elem);
                continue;
            }
            if (s == "CREATE_SPRITE") {
                elem.type = CREATE_SPRITE;
                s = line;
                s.erase(s.find(" "), s.length());
                line.erase(0, line.find(" ") + 1);
                elem.resource = s;
                s = line;
                s.erase(s.find(" "), s.length());
                line.erase(0, line.find(" ") + 1);
                if (s.find("+") != -1) {
                    s.erase(s.find("+"), 1);
                    elem.start = stof(s) + t;
                }
                else {
                    elem.start = stof(s);
                }
                t = elem.start;
                s = line;
                s.erase(s.find(" "), s.length());
                line.erase(0, line.find(" ") + 1);
                elem.x1 = stof(s);
                s = line;
                s.erase(s.find(" "), s.length());
                line.erase(0, line.find(" ") + 1);
                elem.y1 = stof(s);
                s = line;
                s.erase(s.find(" "), s.length());
                line.erase(0, line.find(" ") + 1);
                elem.x2 = stof(s);
                s = line;
                elem.y2 = stof(s);
                temp.elements.push_back(elem);
                continue;
            }
            if (s == "PLAY_SOUND") {
                elem.type = PLAY_SOUND;
                s = line;
                s.erase(s.find(" "), s.length());
                line.erase(0, line.find(" ") + 1);
                elem.resource = s;
                s = line;
                if (s.find("+") != -1) {
                    s.erase(s.find("+"), 1);
                    elem.start = stof(s) + t;
                }
                else {
                    elem.start = stof(s);
                }
                t = elem.start;
                temp.elements.push_back(elem);
                continue;
            }
        }
        f.close();
    }
    temp.done = false;
    safetys.lock();
	printf("\nAnimating scenes: %i.", anims.size());
    anims.push_back(temp);
	printf("\nAnimating scenes: %i.", anims.size());
    safetys.unlock();
    return anims.size() - 1;
}

void animation_engine::tick(double delta) {
	unsigned i;
	bool done;
	safetyf.lock();
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
	safetyf.unlock();
	safetyi.lock();
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
	safetyi.unlock();
	safetys.lock();
	for (i = 0; i < anims.size(); ++i) {
	    if (anims[i].done)
	        continue;
	    for (unsigned j = 0; j < anims[i].elements.size(); ++j) {
	        done = true;
	        printf("\nCURRENT TIME: %f", anims[i].current_time);
	        if ((anims[i].elements[j].start < anims[i].current_time + delta) && (anims[i].elements[j].start >= anims[i].current_time)) {
	            // TODO: ANIMATE
	            printf("\nELEMENT: %f", anims[i].elements[j].start);
	        }
	        else if (anims[i].elements[j].start < anims[i].current_time) {
	            done = false;
	        }
	        anims[i].done = done;
	        anims[i].current_time += delta;
	    }
	}
	safetys.unlock();
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

bool animation_engine::is_dones(unsigned index) {
	if (index >= anims.size()) {
		return true;
	}
	return anims[index].done;
}

void animation_engine::purge() {
	bool found;
	found = false;
	safetyf.lock();
	for (unsigned i = 0; i < animf.size(); ++i) {
		if (!animf[i].done) {
			found = true;
			break;
		}
	}
	if (!found) {
		animf.empty();
	}
	safetyf.unlock();
	safetyi.lock();
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
	safetyi.unlock();
	safetys.lock();
	found = false;
	for (unsigned i = 0; i < anims.size(); ++i) {
		if (!anims[i].done) {
			found = true;
			break;
		}
	}
	if (!found) {
		anims.empty();
	}
	safetys.unlock();
}