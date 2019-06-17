#include "sound.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

extern bool safe_getline(ifstream &f, string& s);

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
		while (safe_getline(f, line)) {
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
	if (s == "") {
		return;
	}
	if (last_music == s) {
		return;
	}
	last_music = s;
	if (music1) {
		music1->stop();
		music1 = 0;
	}
	if (music2) {
		music2->stop();
		music2 = 0;
	}
	if (s.find(",") != -1) {
		s1 = s;
		s2 = s;
		s1.erase(s1.find(","), s1.length());
		s2.erase(0, s2.find(",") + 1);
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

void soundengine::play_cry(string s) {
	FMOD::Channel    *channel1 = 0, *channel2 = 0;
	FMOD_RESULT       result;
	string s1, s2;
	unsigned long long clock_start;
	unsigned int slen1 = 0, slen2 = 0;
	float freq1 = 0.0, freq2 = 0.0;
	int outputrate = 0;
	s1 = s;
	s2 = s;
	s1.erase(s1.find("-"), s1.length());
	s2.erase(0, s2.find("-") + 1);
	s1 = string("cries/") + s1 + string("-out.mp3");
	s2 = string("cries/") + s2 + string("-in.mp3");
	result = system->playSound(sounds[s1], 0, true, &channel1);
	result = system->playSound(sounds[s2], 0, true, &channel2);
	result = system->getSoftwareFormat(&outputrate, 0, 0);
	result = channel1->getDSPClock(0, &clock_start);
	result = sounds[s1]->getLength(&slen1, FMOD_TIMEUNIT_PCM);
	result = sounds[s1]->getDefaults(&freq1, 0);
	slen1 = (unsigned int)((float)slen1 / freq1 * outputrate);
	result = sounds[s2]->getLength(&slen2, FMOD_TIMEUNIT_PCM);
	result = sounds[s2]->getDefaults(&freq2, 0);
	slen2 = (unsigned int)((float)slen2 / freq2 * outputrate);
	if (slen1 > slen2) {
		clock_start += (slen1 - slen2);
		result = channel2->setDelay(clock_start, 0, false);
	}
	channel1->setPaused(false);
	channel2->setPaused(false);
}