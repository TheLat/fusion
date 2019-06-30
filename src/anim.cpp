#include "anim.h"
#include "sound.h"
#include "graphics.h"

mutex safetyi, safetyf, safetys;

extern bool safe_getline(ifstream &f, string& s);
extern soundengine se;
extern graphics g;


frame animation_engine::spline_frame(double t, vector<frame> &frames) {
	frame temp;
	vector<frame> new_frames;
	if (frames.size() > 1) {
		for (unsigned i = 0; i < frames.size() - 1; ++i) {
			temp.x1 = frames[i].x1 + (t*(frames[i + 1].x1 - frames[i].x1));
			temp.x2 = frames[i].x2 + (t*(frames[i + 1].x2 - frames[i].x2));
			temp.y1 = frames[i].y1 + (t*(frames[i + 1].y1 - frames[i].y1));
			temp.y2 = frames[i].y2 + (t*(frames[i + 1].y2 - frames[i].y2));
			new_frames.push_back(temp);
		}
		return spline_frame(t, new_frames);
	}
	return frames[0];
}

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

unsigned animation_engine::create_anim_scene(string scene, int attacker, int defender) {
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
			if (s == "HIDE_SPRITE") {
				elem.type = HIDE_SPRITE;
				s = line;
				s.erase(s.find(" "), s.length());
				line.erase(0, line.find(" ") + 1);
				elem.index = stoi(s);
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
                if (s == "ATTACKER")
                    elem.index = -attacker;
                else if (s == "DEFENDER")
                    elem.index = -defender;
                else if (s == "SCREEN_EFFECTS")
                    elem.index = -2;
                else if (s == "SCREEN")
                    elem.index = -1;
                else
                    elem.index = stoi(s);
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
				if (s.find("+") != -1) {
					s.erase(s.find("+"), 1);
					elem.end = stof(s) + t;
				}
				else {
					elem.end = stof(s);
				}
				t = elem.end;
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
                }
                elem.done = false;
                temp.elements.push_back(elem);
                elem.frames.clear();
                continue;
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
	quad* q = 0;
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
				case HIDE_SPRITE:
					q = &(g.draw_list[anims[i].sprites[anims[i].elements[j].index]]);
					q->height = 0.0;
					q->width = 0.0;
					q->x = -1.0;
					q->y = -1.0;
					break;
	            case PLAY_SOUND:
	                se.play_sound(string("../resources/") + anims[i].elements[j].resource);
	                break;
	            case SPLINE_TRANSFORM:
	                if (anims[i].elements[j].done)
	                    continue;
	                done = false;
					frame temp_frame;
					if (anims[i].elements[j].index >= 0)
	                    q = &(g.draw_list[anims[i].sprites[anims[i].elements[j].index]]);
	                else if (anims[i].elements[j].index == -1)
	                    q = &(g.r_quad);
	                else if (anims[i].elements[j].index == -2)
	                    q = &(g.r_effects);
	                else {
	                    q = &(g.draw_list[-anims[i].elements[j].index]); // hack to allow access to the draw list
	                }
	                double t = (anims[i].current_time + delta - anims[i].elements[j].start);
	                double t2 = (anims[i].elements[j].end - anims[i].elements[j].start);
	                t /= t2;
	                if (t < 0.0)
	                    t = 0.0;
	                if (t > 1.0)
	                    t = 1.0;
					temp_frame = spline_frame(t, anims[i].elements[j].frames);
					q->x = temp_frame.x1;
					q->y = temp_frame.y1;
					q->width = temp_frame.x2;
					q->height = temp_frame.y2;
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
		animf.clear();
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
		animi.clear();
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
		anims.clear();
	}
	safetys.unlock();
}