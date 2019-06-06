#ifndef SOUND_HEADER
#define SOUND_HEADER

#include "fmod/fmod.hpp"
#include <map>
#include <string>

using namespace std;

class soundengine {
private:
	std::map<string, FMOD::Sound*> sounds;
	FMOD::System     *system;
public:
	bool holder;
	void init_sounds();
};

#endif