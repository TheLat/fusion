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
			line = string("../") + line;
			result = system->createSound(line.c_str(), FMOD_DEFAULT, 0, &sounds[line]);
		}
		f.close();
	}
	//result = system->playSound(sounds[string("../resources/music/02-pallet-town-loop.mp3")], 0, false, &channel);
}