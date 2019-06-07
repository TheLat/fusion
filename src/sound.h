#ifndef SOUND_HEADER
#define SOUND_HEADER

#include "fmod/fmod.hpp"
#include <map>
#include <string>

using namespace std;

class soundengine {
private:
	std::map<string, FMOD::Sound*> sounds;
	FMOD::System *system;
	FMOD::Channel *music = 0;
	FMOD::Sound* last_music = 0;
public:
	bool holder;
	void init_sounds();
	void play_sound(string s);
	void play_sound_blocking(string s);
	void play_music(string s);
};

#endif