#include "sound.h"
#include "core.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

extern void log(const char *format, const char *value = 0);
extern bool safe_getline(ifstream &f, string& s);
extern string safepath;
extern engine e;

void soundengine::init_sounds() {
#ifdef __SWITCH__
	// TODO:  Initialize sounds and sound system
	SDL_Init(SDL_INIT_AUDIO);
	Mix_Init(MIX_INIT_MP3);
	Mix_AllocateChannels(SOUND_CHANNELS);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
	string tmp;
#else
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;
	result = FMOD::System_Create(&system);
	result = system->getVersion(&version);
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
#endif

	ifstream f((safepath + string("data/sounds.dat")).c_str());
	string line;
	while (f.is_open()) {
		while (safe_getline(f, line)) {
#ifdef __SWITCH__
			if (line.find("music/") == 0) {
				music[line] = Mix_LoadMUS((safepath + line).c_str());
			}
			else {
				tmp = line;
				tmp.erase(tmp.find(".mp3"), tmp.length());
				tmp = tmp + string(".wav");
				sounds[line] = Mix_LoadWAV((safepath + tmp).c_str());
			}
#else
			if (line.find("-loop.mp3") == -1)
				result = system->createSound((safepath + line).c_str(), FMOD_DEFAULT, 0, &sounds[line]);
			else
				result = system->createSound((safepath + line).c_str(), FMOD_LOOP_NORMAL, 0, &sounds[line]);
#endif
		}
		f.close();
	}
#ifdef __SWITCH__
#else
	channel_index = 0;
	for (unsigned i = 0; i < SOUND_CHANNELS; ++i) {
	    channels[i] = 0;
	}
	music1 = 0;
	music2 = 0;
#endif
}

bool soundengine::is_playing(int chan) {
	bool playing = false;
	if (chan < 0 || chan >= SOUND_CHANNELS)
		return false;
#ifdef __SWITCH__
	return Mix_Playing(chan);
#else
	(channels[chan])->isPlaying(&playing);
#endif
	return playing;
}

int soundengine::play_sound(string s) {
#ifdef __SWITCH__
	if (sounds[s] == 0) {
		string tmp = s;
		tmp.erase(tmp.find(".mp3"), tmp.length());
		tmp = tmp + string(".wav");
		sounds[s] = Mix_LoadWAV(tmp.c_str());
	}
	int volume = int(get_sfx_volume() * MIX_MAX_VOLUME);
	int chan =  Mix_PlayChannel(-1, sounds[s], 0);
	Mix_Volume(chan, volume);
	return chan;
#else
	FMOD_RESULT       result;
	int ret;
	float volume = get_sfx_volume();
	if (sounds[s] == 0) {
		result = system->createSound((safepath + s).c_str(), FMOD_DEFAULT, 0, &sounds[s]);
	}
	if (channels[channel_index])
	    channels[channel_index]->stop();
	result = system->playSound(sounds[s], 0, true, &(channels[channel_index]));
	result = (channels[channel_index])->setVolume(volume);
	result = (channels[channel_index])->setPaused(false);
	ret = channel_index;
	channel_index = (channel_index+1) % SOUND_CHANNELS;
	return ret;
#endif
}

void soundengine::play_sound_blocking(string s) {
#ifdef __SWITCH__
	int chan = Mix_PlayChannel(-1, sounds[s], 0);
	while (this->is_playing(chan)) {
	}
#else
	FMOD_RESULT       result;
	float volume = get_sfx_volume();
	bool playing = false;
	if (sounds[s] == 0) {
		result = system->createSound((safepath + s).c_str(), FMOD_DEFAULT, 0, &sounds[s]);
	}
	if (channels[channel_index])
	    channels[channel_index]->stop();
	result = system->playSound(sounds[s], 0, true, &(channels[channel_index]));
	result = (channels[channel_index])->setVolume(volume);
	result = (channels[channel_index])->setPaused(false);
	result = (channels[channel_index])->isPlaying(&playing);
	while (playing) {
		result = (channels[channel_index])->isPlaying(&playing);
	}
	channel_index = (channel_index+1) % SOUND_CHANNELS;
#endif
}

#ifdef __SWITCH__
Mix_Music* looper = 0;
void playloop() {
	Mix_PlayMusic(looper, -1);
}
#endif

void soundengine::play_music(string s) {
	string s1;
	string s2;
	if (s == "") {
		return;
}
	if (last_music == s) {
		return;
	}
	last_music = s;
#ifdef __SWITCH__
	int volume = int(MIX_MAX_VOLUME * get_music_volume());
	Mix_VolumeMusic(volume);
	if (s.find(",") != -1) {
		s1 = s;
		s2 = s;
		s1.erase(s1.find(","), s1.length());
		s2.erase(0, s2.find(",") + 1);
		Mix_PlayMusic(music[s1], 0);
		looper = music[s2];
		Mix_HookMusicFinished(playloop);
	}
	else {
		Mix_PlayMusic(music[s], -1);
		Mix_HookMusicFinished(0);
	}
#else
	FMOD_RESULT       result;
	unsigned long long clock_start = 0;
	unsigned int slen = 0;
	float freq = 0.0;
	int outputrate = 0;
    float volume = get_music_volume();
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
#endif
}

void soundengine::play_cry(string s, bool blocking) {
	string s1, s2;
	bool playing = false;
	s1 = s;
	s2 = s;
	s1.erase(s1.find("-"), s1.length());
	s2.erase(0, s2.find("-") + 1);
	s1 = string("cries/") + s1 + string(".mp3");
	s2 = string("cries/") + s2 + string(".mp3");
#ifdef __SWITCH__
	if (s1 == s2) {
		if (blocking) {
			this->play_sound_blocking(s1);
		}
		else {
			this->play_sound(s1);
		}
	}
	else {
		int freq, chans, points, frames, ms1, ms2;
		unsigned short fmt;
		points = (sounds[s1]->alen / ((fmt & 0xFF) / 8));
		frames = (points / chans);
		ms1 = ((frames * 1000) / freq);
		points = (sounds[s2]->alen / ((fmt & 0xFF) / 8));
		frames = (points / chans);
		ms2 = ((frames * 1000) / freq);
		Mix_QuerySpec(&freq, &fmt, &chans);
		if (ms1 < ms2) {
			ms2 = ms1;
}
		ms1 = ms2;
		int chan1 = this->play_sound(s1);
		Mix_FadeOutChannel(chan1, ms1);
		int chan2 = Mix_FadeInChannel(-1, sounds[s2], 0, ms1);
		if (blocking) {
			while (this->is_playing(chan1) || this->is_playing(chan2)) {
			}
		}
		else {
		}
	}
#else
	FMOD_RESULT       result;
	unsigned long long clock_start;
	unsigned int slen1 = 0, slen2 = 0;
	float flen1 = 0, flen2 = 0;
	float freq1 = 0.0, freq2 = 0.0;
	float volume = get_sfx_volume();
	int outputrate = 0;
	if (s1 == s2) {
	    if (channels[channel_index])
	        channels[channel_index]->stop();
	    result = system->playSound(sounds[s1], 0, true, &(channels[channel_index]));
		channels[channel_index]->setVolume(volume);
		channels[channel_index]->setPaused(false);
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
	(channels[channel_index])->setVolume(volume);
	(channels[(channel_index + 1) % SOUND_CHANNELS])->setVolume(volume);
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
#endif
}

void soundengine::mute_music(bool partial) {
#ifdef __SWITCH__
	if (partial) {
		int volume = int(MIX_MAX_VOLUME * get_music_volume() * 0.5);
		Mix_VolumeMusic(volume);
	}
	else {
		Mix_VolumeMusic(0);
	}
#else
    float volume = 0.0;
    if (partial)
        volume = 0.5*get_music_volume();
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
#endif
}

void soundengine::unmute_music() {
#ifdef __SWITCH__
	int volume = int(MIX_MAX_VOLUME * get_music_volume());
	Mix_VolumeMusic(volume);
#else
    float volume = get_music_volume();
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
#endif
}

void soundengine::update_volumes() {
	unmute_music();
}

float soundengine::get_sfx_volume() {
	return float(e.mc.values[string("SFXVOLUME")]) / 8.0f;
}
float soundengine::get_music_volume() {
	return float(e.mc.values[string("MUSICVOLUME")]) / 8.0f;
}

void soundengine::cleanup() {
#ifdef __SWITCH__
	std::map<string, Mix_Music*>::iterator it1 = music.begin();
	while (it1 != music.end()) {
		if (it1->second)
			Mix_FreeMusic(it1->second);
		it1++;
	}
	std::map<string, Mix_Chunk*>::iterator it2 = sounds.begin();
	while (it2 != sounds.end()) {
		if (it2->second)
			Mix_FreeChunk(it2->second);
		it2++;
	}
	SDL_Quit();
#endif
}