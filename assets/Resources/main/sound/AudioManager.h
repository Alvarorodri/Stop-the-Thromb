#ifndef _AUDIO_MANAGER_INCLUDE
#define _AUDIO_MANAGER_INCLUDE

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
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

	enum soundNames {RunningInThe90s, LaserGun, Explode, ButtonPressed, ButtonSelected, Boost, ChargeAttack, PowerUp, BossRoar, EnemyKilled, CollectBonus};

	static AudioManager *getInstance();

private:

	AudioManager();
	~AudioManager();

public:

	void init(string file);

	int addSoundEffect(const char* soundFile);
	void playSoundEffect(int sound, int volume);

	void playSoundEffectLooped(int sound, int volume);
	void stopSoundEffectLooped(int sound);

private:

	std::vector<Mix_Chunk*> mSoundEffectBank;

	map<soundNames, int> loops;

};

#endif // _AUDIO_MANAGER_INCLUDE
