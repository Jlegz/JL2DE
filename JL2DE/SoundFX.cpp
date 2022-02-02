#include "JL2DE.h"

SoundFX::SoundFX(string path)
{
	soundFX = Mix_LoadWAV(path.c_str());
	if (soundFX == NULL)
	{
		cout << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
	}

}

void SoundFX::play()
{
	Mix_PlayChannel(-1, soundFX, 0);
}