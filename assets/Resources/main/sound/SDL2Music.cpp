#include "SDL2Music.h"
#include "SDLInit.h"
#include <SDL.h>

SDL2Music::SDL2Music()
{
  SDLInit::initSDLSound();
}

void SDL2Music::addMusicTrack(const char* path)
{
  Mix_Music* tmp_music = Mix_LoadMUS(path);

  if (tmp_music != nullptr)
  {
    m_Musics.push_back(tmp_music);
  }
  else
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
      "Couldn't load audio: %s",
      Mix_GetError());
  }
}

void SDL2Music::playMusicTrack(const int which)
{ 
	Mix_VolumeMusic(42);
  if (which > m_Musics.size())
  {
    return;
  }

  Mix_PlayMusic(m_Musics.at(which), -1);
 

  m_Playing = true;

}

void SDL2Music::Play_Pause()
{
  if (m_Playing && !m_Paused)
  {
    Mix_PauseMusic();
    m_Paused = true;
  }
  else if (m_Playing && m_Paused)
  {
    Mix_ResumeMusic();
    m_Paused = false;
  }
  else
  {
    return;
  }
}
