#ifndef _AUDIO_MANAGER_INCLUDE
#define _AUDIO_MANAGER_INCLUDE

#include <iostream>
#include <vector>
#include <fstream>
// https://youtu.be/q06uSmkiqec

#include <sstream>
#include <cstring>

#include "SDL_mixer.h"
#include "SDL.h"

using namespace std;

class AudioManager {

private :

	static AudioManager *instance_;

public:

	enum soundNames {RunningInThe90s, LaserGun };

	static AudioManager *getInstance();

private:

	AudioManager();
	~AudioManager();

public:

	void init(string file);

	int addSoundEffect(const char* soundFile);
	void playSoundEffect(int sound, int volume);

private:

	std::vector<Mix_Chunk*> mSoundEffectBank;

};

#endif // _AUDIO_MANAGER_INCLUDE
