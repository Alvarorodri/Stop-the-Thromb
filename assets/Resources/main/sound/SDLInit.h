#pragma once
class SDLInit
{
public:
  static void initSDLSound();

  SDLInit();
  ~SDLInit();

private:
  bool m_SoundInitilized = false;

};

