#ifndef SOUND_HEADER
#define SOUND_HEADER

#include "fmod/fmod.hpp"
#include <map>
#include <string>

using namespace std;
#define SOUND_CHANNELS 8

class soundengine {
private:
	std::map<string, FMOD::Sound*> sounds;
	FMOD::System *system;
	FMOD::Channel *music1, *music2;
	FMOD::Channel *channels[8];
	int channel_index;
public:
	string last_music;
	bool holder;
	void init_sounds();
	void play_sound(string s);
	void play_sound_blocking(string s);
	void play_music(string s);
	void play_cry(string s, bool blocking=false);
	void mute_music(bool partial);
	void unmute_music();
	void update_volumes();
	float get_sfx_volume();
	float get_music_volume();
};

#endif