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
	FMOD::Channel *music1 = 0, *music2 = 0;
	string last_music;
public:
	bool holder;
	void init_sounds();
	void play_sound(string s);
	void play_sound_blocking(string s);
	void play_music(string s);
	void play_cry(string s);
};

#endif