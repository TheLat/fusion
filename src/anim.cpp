#include "anim.h"
#include "sound.h"
#include "graphics.h"

mutex safetyi, safetyf, safetys;

extern bool safe_getline(ifstream &f, string& s);
extern soundengine se;
extern graphics g;

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
    ifstream f((string("../resources/animations/") + scene + string(".dat")).c_str());
    while (f.is_open()) {
        while (safe_getline(f, line)) {
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
                elem.end = elem.start;
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
                elem.end = elem.start;
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
                elem.end = elem.start;
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
                elem.end = elem.start;
                temp.elements.push_back(elem);
                continue;
            }
            if (s == "SPLINE_TRANSFORM") {
                elem.type = SPLINE_TRANSFORM;
                s = line;
                s.erase(s.find(" "), s.length());
                line.erase(0, line.find(" ") + 1);
                elem.index = stoi(s); // handle attacker, defender, and screen
                s = line;
                s.erase(s.find(" "), s.length());
                line.erase(0, line.find(" ") + 1);
                elem.start = stof(s);
                s = line;
                s.erase(s.find(" "), s.length());
                line.erase(0, line.find(" ") + 1);
                elem.end = stof(s);
                while (line.find(" ") != -1) {
                    frame temp_frame;
                    s = line;
                    s.erase(s.find(" "), s.length());
                    line.erase(0, line.find(" ") + 1);
                    temp_frame.x1 = stof(s);
                    s = line;
                    s.erase(s.find(" "), s.length());
                    line.erase(0, line.find(" ") + 1);
                    temp_frame.y1 = stof(s);
                    s = line;
                    s.erase(s.find(" "), s.length());
                    line.erase(0, line.find(" ") + 1);
                    temp_frame.x2 = stof(s);
                    s = line;
                    if (s.find(" ") != -1) {
                        s.erase(s.find(" "), s.length());
                        line.erase(0, line.find(" ") + 1);
                    }
                    temp_frame.y2 = stof(s);
                    elem.frames.push_back(temp_frame);
                    elem.frames.empty();
                }
                elem.done = false;
                temp.elements.push_back(elem);
            }
        }
        f.close();
    }
    temp.done = false;
    safetys.lock();
    anims.push_back(temp);
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
	    done = true;
	    for (unsigned j = 0; j < anims[i].elements.size(); ++j) {
	        if ((anims[i].elements[j].start <= anims[i].current_time + delta) && (anims[i].elements[j].end >= anims[i].current_time)) {
	            switch (anims[i].elements[j].type) {
	            case MUTE_MUSIC:
	                // TODO
	                break;
	            case UNMUTE_MUSIC:
	                // TODO
	                break;
	            case CREATE_SPRITE:
	                anims[i].sprites.push_back(g.push_quad_load(anims[i].elements[j].x1, anims[i].elements[j].y1, anims[i].elements[j].x2, anims[i].elements[j].y2, string("../resources/") + anims[i].elements[j].resource));
	                break;
	            case PLAY_SOUND:
	                se.play_sound(string("../resources/") + anims[i].elements[j].resource);
	                break;
	            case SPLINE_TRANSFORM:
	                if (anims[i].elements[j].done)
	                    continue;
	                quad* q = &(g.draw_list[anims[i].sprites[anims[i].elements[j].index]]);
	                double t = (anims[i].current_time + delta - anims[i].elements[j].start);
	                double t2 = (anims[i].elements[j].end - anims[i].elements[j].start);
	                t /= t2;
	                if (t < 0.0)
	                    t = 0.0;
	                if (t > 1.0)
	                    t = 1.0;
	                q->x = anims[i].elements[j].frames[0].x1 + (t*(anims[i].elements[j].frames[1].x1 - anims[i].elements[j].frames[0].x1));
	                q->y = anims[i].elements[j].frames[0].y1 + (t*(anims[i].elements[j].frames[1].y1 - anims[i].elements[j].frames[0].y1));
	                q->width = anims[i].elements[j].frames[0].x2 + (t*(anims[i].elements[j].frames[1].x2 - anims[i].elements[j].frames[0].x2));
	                q->height = anims[i].elements[j].frames[0].y2 + (t*(anims[i].elements[j].frames[1].y2 - anims[i].elements[j].frames[0].y2));
	                if (t == 1.0)
	                    anims[i].elements[j].done = true;
	                break;
	            }
	            continue;
	        }
	        if (anims[i].elements[j].end > anims[i].current_time) {
	            done = false;
	        }
	    }
	    anims[i].done = done;
	    anims[i].current_time += delta;
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