#include "sound.h"

#include <stdio.h>
#include <iostream>
#include <fstream>


void soundengine::init_sounds() {
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;
	result = FMOD::System_Create(&system);
	result = system->getVersion(&version);
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);

	ifstream f("../resources/data/sounds.dat");
	string line;
	while (f.is_open()) {
		while (std::getline(f, line)) {
			result = system->createSound((string("../resources/") + line).c_str(), FMOD_DEFAULT, 0, &sounds[line]);
		}
		f.close();
	}
}

void soundengine::play_sound(string s) {
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	if (sounds[s] == 0) {
		result = system->createSound((string("../resources/") + s).c_str(), FMOD_DEFAULT, 0, &sounds[s]);
	}
	result = system->playSound(sounds[s], 0, false, &channel);
}

void soundengine::play_sound_blocking(string s) {
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	bool playing = false;
	if (sounds[s] == 0) {
		result = system->createSound((string("../resources/") + s).c_str(), FMOD_DEFAULT, 0, &sounds[s]);
	}
	result = system->playSound(sounds[s], 0, false, &channel);
	result = channel->isPlaying(&playing);
	while (playing) {
		result = channel->isPlaying(&playing);
	}
}

void soundengine::play_music(string s) {
	FMOD_RESULT       result;
	if (sounds[s] == 0) {
		result = system->createSound((string("../resources/") + s).c_str(), FMOD_DEFAULT, 0, &sounds[s]);
	}
	if (music) {
		// TODO: Add fade-out.
		music->stop();
	}
	result = system->playSound(sounds[s], 0, false, &music);
}