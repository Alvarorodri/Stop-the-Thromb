#include "SDLInit.h"
#include <SDL_mixer.h>
#include <SDL.h>

static SDLInit* sdlsound = nullptr;

void SDLInit::initSDLSound()
{
  if (sdlsound)
  {
    return;
  }

  sdlsound = new SDLInit();

}

SDLInit::SDLInit()
{
  // init sound
  SDL_Init(SDL_INIT_AUDIO);

  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 4096;

  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
    exit(-1);
  }
  else
  {
    m_SoundInitilized = true;
  }
}

SDLInit::~SDLInit()
{
  SDL_Quit();
}
