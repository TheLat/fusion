#ifndef SOUND_HEADER
#define SOUND_HEADER

#ifdef __SWITCH__
#include <SDL.h>
#include <SDL_mixer.h>
#else
#include "fmod/fmod.hpp"
#endif
#include <map>
#include <string>

using namespace std;
#define SOUND_CHANNELS 8

class soundengine {
private:
#ifdef __SWITCH__
	// TODO:  Switch sound map and sound system
	std::map<string, Mix_Music*> music;
	std::map<string, Mix_Chunk*> sounds;
#else
	std::map<string, FMOD::Sound*> sounds;
	FMOD::System *system;
	FMOD::Channel *music1, *music2;
	FMOD::Channel *channels[8];
#endif
	int channel_index;
public:
	string last_music;
	bool holder;
	void init_sounds();
	bool is_playing(int chan);
	int play_sound(string s);
	void play_sound_blocking(string s);
	void play_music(string s);
	void play_cry(string s, bool blocking=false);
	void mute_music(bool partial);
	void unmute_music();
	void update_volumes();
	float get_sfx_volume();
	float get_music_volume();
	void cleanup();
};

#endif