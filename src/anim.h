#ifndef ANIM_HEADER
#define ANIM_HEADER

#include <vector>
#include <mutex>
#include <string>
#include <fstream>
#include "timer.h"

enum SCENE_TYPE {
    MUTE_MUSIC = 0,
    UNMUTE_MUSIC,
    CREATE_SPRITE,
    PLAY_SOUND,
    SPLINE_TRANSFORM,
	HIDE_SPRITE
};

class frame {
public:
    double x1, x2, y1, y2;
};

class scene_element {
public:
    SCENE_TYPE type;
    string resource;
    vector<frame> frames;
    bool done;
    int index;
    double start, end, x1, y1, x2, y2;
};

class animation_scene {
public:
    double current_time;
    vector<scene_element> elements;
    vector<unsigned> sprites;
    bool done;
};

class animation_int {
public:
	int* itarg;
	int start, end;
	bool done;
	double duration;
	double time;
};

class animation_float {
public:
	double* ftarg;
	double start, end;
	bool done;
	double duration;
	double time;
};

class animation_engine {
private:
	vector<animation_float> animf;
	vector<animation_int> animi;
	vector<animation_scene> anims;
public:
	frame spline_frame(double t, vector<frame> &frames);
	unsigned create_animf(double* targ, double start, double end, double duration);
	unsigned create_animi(int* targ, int start, int end, double duration);
	unsigned create_anim_scene(string scene, unsigned attacker = 0, unsigned defender = 0);
	void tick(double delta);
	bool is_donef(unsigned index);
	bool is_donei(unsigned index);
	bool is_dones(unsigned index);
	void purge();
};

#endif