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
	string s1;
	string s2;
	unsigned long long clock_start = 0;
	unsigned int slen = 0;
	float freq = 0.0;
	int outputrate = 0;
	if (last_music == s) {
		return;
	}
	if (music1) {
		// TODO: Add fade-out.
		music1->stop();
		music1 = 0;
	}
	if (music2) {
		// TODO: Add fade-out.
		music2->stop();
		music2 = 0;
	}
	if (s.find("|") != -1) {
		s1 = s;
		s2 = s;
		s1.erase(s1.find("|"), s1.length());
		s2.erase(0, s2.find("|") + 1);
		if (sounds[s1] == 0) {
			result = system->createSound((string("../resources/") + s1).c_str(), FMOD_DEFAULT, 0, &sounds[s1]);
		}
		if (sounds[s2] == 0) {
			result = system->createSound((string("../resources/") + s2).c_str(), FMOD_LOOP_NORMAL, 0, &sounds[s2]);
		}
		result = system->playSound(sounds[s1], 0, true, &music1);
		result = system->playSound(sounds[s2], 0, true, &music2);
		result = system->getSoftwareFormat(&outputrate, 0, 0);
		result = music1->getDSPClock(0, &clock_start);
		result = sounds[s1]->getLength(&slen, FMOD_TIMEUNIT_PCM);
		result = sounds[s1]->getDefaults(&freq, 0);
		slen = (unsigned int)((float)slen / freq * outputrate);
		clock_start += slen;
		result = music2->setDelay(clock_start, 0, false);
		music1->setPaused(false);
		music2->setPaused(false);
		music2->setLoopCount(1000000);
	}
	else {
		if (sounds[s] == 0) {
			result = system->createSound((string("../resources/") + s).c_str(), FMOD_DEFAULT, 0, &sounds[s]);
		}
		result = system->playSound(sounds[s], 0, false, &music1);
	}
}