#pragma once
#include <vector>
#include <SDL_mixer.h>
class SDL2Music
{
public:

  SDL2Music();

  void addMusicTrack(const char* path);

  void playMusicTrack(const int which);

  void Play_Pause();

private:

  std::vector<Mix_Music*> m_Musics;

  bool m_Paused = false;

  bool m_Playing = false;

};

