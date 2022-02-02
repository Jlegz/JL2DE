#include "JL2DE.h"

Music::Music(string path)
{
	music = Mix_LoadMUS(path.c_str());
	if (music == NULL)
	{
		cout << "Failed to load scratch sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
	}

}

void Music::play()
{
	Mix_PlayMusic(music, -1);
}

void Music::togglePause()
{
	Mix_PauseMusic();
}