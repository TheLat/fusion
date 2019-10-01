#include "sound.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

extern bool safe_getline(ifstream &f, string& s);
extern string safepath;

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
			result = system->createSound((safepath + line).c_str(), FMOD_DEFAULT, 0, &sounds[line]);
		}
		f.close();
	}
	channel_index = 0;
	for (unsigned i = 0; i < SOUND_CHANNELS; ++i) {
	    channels[i] = 0;
	}
	music1 = 0;
	music2 = 0;
}

void soundengine::play_sound(string s) {
	FMOD_RESULT       result;
	if (sounds[s] == 0) {
		result = system->createSound((safepath + s).c_str(), FMOD_DEFAULT, 0, &sounds[s]);
	}
	if (channels[channel_index])
	    channels[channel_index]->stop();
	result = system->playSound(sounds[s], 0, false, &(channels[channel_index]));
	channel_index = (channel_index+1) % SOUND_CHANNELS;
}

void soundengine::play_sound_blocking(string s) {
	FMOD_RESULT       result;
	bool playing = false;
	if (sounds[s] == 0) {
		result = system->createSound((safepath + s).c_str(), FMOD_DEFAULT, 0, &sounds[s]);
	}
	if (channels[channel_index])
	    channels[channel_index]->stop();
	result = system->playSound(sounds[s], 0, false, &(channels[channel_index]));
	result = (channels[channel_index])->isPlaying(&playing);
	while (playing) {
		result = (channels[channel_index])->isPlaying(&playing);
	}
	channel_index = (channel_index+1) % SOUND_CHANNELS;
	// TODO: seems to block for a second longer than it should
}

void soundengine::play_music(string s) {
	FMOD_RESULT       result;
	string s1;
	string s2;
	unsigned long long clock_start = 0;
	unsigned int slen = 0;
	float freq = 0.0;
	int outputrate = 0;
    double volume = 1.0;
	if (s == "") {
		return;
	}
	if (last_music == s) {
		return;
	}
	last_music = s;
	// TODO: Fade-out for playing music.
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
			result = system->createSound((safepath + s1).c_str(), FMOD_DEFAULT, 0, &sounds[s1]);
		}
		if (sounds[s2] == 0) {
			result = system->createSound((safepath + s2).c_str(), FMOD_LOOP_NORMAL, 0, &sounds[s2]);
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
		music2->setLoopCount(1000000);
        music2->setVolume(volume);
        music1->setVolume(volume);
		music2->setPaused(false);
		music1->setPaused(false);
	}
	else {
		if (sounds[s] == 0) {
			result = system->createSound((safepath + s).c_str(), FMOD_LOOP_NORMAL, 0, &sounds[s]);
		}
		result = system->playSound(sounds[s], 0, true, &music1);
		music1->setLoopCount(1000000);
        music1->setVolume(volume);
		music1->setPaused(false);
	}
}

void soundengine::play_cry(string s, bool blocking) {
	FMOD_RESULT       result;
	string s1, s2;
	bool playing = false;
	unsigned long long clock_start;
	unsigned int slen1 = 0, slen2 = 0;
	float flen1 = 0, flen2 = 0;
	float freq1 = 0.0, freq2 = 0.0;
	int outputrate = 0;
	s1 = s;
	s2 = s;
	s1.erase(s1.find("-"), s1.length());
	s2.erase(0, s2.find("-") + 1);
	s1 = string("cries/") + s1 + string(".mp3");
	s2 = string("cries/") + s2 + string(".mp3");
	if (s1 == s2) {
	    if (channels[channel_index])
	        channels[channel_index]->stop();
	    result = system->playSound(sounds[s1], 0, false, &(channels[channel_index]));
	    if (blocking) {
	        result = (channels[channel_index])->isPlaying(&playing);
            while (playing) {
                result = (channels[channel_index])->isPlaying(&playing);
            }
	    }
	    channel_index = (channel_index+1) % SOUND_CHANNELS;
	    return;
	}
	if (channels[channel_index])
        channels[channel_index]->stop();
	if (channels[(channel_index + 1) % SOUND_CHANNELS])
        channels[channel_index]->stop();
	result = system->playSound(sounds[s1], 0, true, &(channels[channel_index]));
	result = system->playSound(sounds[s2], 0, true, &(channels[(channel_index + 1) % SOUND_CHANNELS]));
	result = system->getSoftwareFormat(&outputrate, 0, 0);
	result = (channels[channel_index])->getDSPClock(0, &clock_start);
	result = sounds[s1]->getLength(&slen1, FMOD_TIMEUNIT_PCM);
	result = sounds[s1]->getDefaults(&freq1, 0);
	flen1 = ((float)slen1 / freq1 * outputrate);
	result = sounds[s2]->getLength(&slen2, FMOD_TIMEUNIT_PCM);
	result = sounds[s2]->getDefaults(&freq2, 0);
	flen2 = ((float)slen2 / freq2 * outputrate);
	result = (channels[channel_index])->addFadePoint(clock_start, 1.0);
	result = (channels[channel_index])->addFadePoint(clock_start + flen1, 0.0);
	if (flen1 > flen2) {
		clock_start += (flen1 - flen2);
		result = (channels[(channel_index + 1) % SOUND_CHANNELS])->setDelay(clock_start, 0, false);
	}
	result = (channels[(channel_index + 1) % SOUND_CHANNELS])->addFadePoint(clock_start, 0.0);
	result = (channels[(channel_index + 1) % SOUND_CHANNELS])->addFadePoint(clock_start + flen2, 1.0);
	(channels[channel_index])->setPaused(false);
	(channels[(channel_index + 1) % SOUND_CHANNELS])->setPaused(false);
	if (blocking) {
        result = (channels[channel_index])->isPlaying(&playing);
        while (playing) {
            result = (channels[channel_index])->isPlaying(&playing);
        }
        result = (channels[(channel_index + 1) % SOUND_CHANNELS])->isPlaying(&playing);
        while (playing) {
            result = (channels[(channel_index + 1) % SOUND_CHANNELS])->isPlaying(&playing);
        }
    }
	channel_index = (channel_index+2) % SOUND_CHANNELS;
}

void soundengine::mute_music(bool partial) {
    double volume = 0.0;
    if (partial)
        volume = 0.5;
    if (music1) {
        music1->setPaused(true);
        music1->setVolume(volume);
        music1->setPaused(false);
    }
    if (music2) {
        music2->setPaused(true);
        music2->setVolume(volume);
        music2->setPaused(false);
    }
}

void soundengine::unmute_music() {
    double volume = 1.0;
    if (music1) {
        music1->setPaused(true);
        music1->setVolume(volume);
        music1->setPaused(false);
    }
    if (music2) {
        music2->setPaused(true);
        music2->setVolume(volume);
        music2->setPaused(false);
    }
}