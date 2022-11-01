#include "AudioManager.h"

AudioManager* AudioManager::instance_ = nullptr;

AudioManager *AudioManager::getInstance() {
	if (instance_ == nullptr) instance_ = new AudioManager();
	return instance_;
}

AudioManager::AudioManager() {
	SDL_Init(SDL_INIT_AUDIO);

	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	int result = Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);

	if (result != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %S", Mix_GetError());
		exit(-1);
	}
}

AudioManager::~AudioManager() {
}

void AudioManager::init(string file) {
	ifstream fin;
	string line;
	stringstream sstream;
	int nrSounds;

	fin.open(file.c_str());
	if (!fin.is_open()) return;
	getline(fin, line);
	sstream.str(line);
	sstream >> nrSounds;

	for (int i = 0; i < nrSounds; ++i) {
		stringstream sstream1;
		string sound;

		getline(fin, line);
		sstream1.str(line);
		sstream1 >> sound;

		addSoundEffect(sound.c_str());
	}

	fin.close();

}

int AudioManager::addSoundEffect(const char* soundFile) {
	Mix_Chunk* tmpChunk = Mix_LoadWAV(soundFile);

	if (tmpChunk != nullptr) {
		mSoundEffectBank.push_back(tmpChunk);
	}

	return mSoundEffectBank.size() - 1;
}

void AudioManager::playSoundEffect(int sound, int volume = 128) {
	int channel = Mix_PlayChannel(-1, mSoundEffectBank[sound], 0);

	Mix_Volume(channel, volume);
}
