#include "Music.h"

void Music::LoadAudio(const char* musicPath)
{
	audioPath = musicPath;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	music = Mix_LoadMUS(musicPath);
	if (music == NULL)
	{
		std::cout << ("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

void Music::PlayAudio()
{
	Mix_PlayMusic(music, -1);
}

void Music::ResumeOrPauseMusic()
{
	if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
	else Mix_PauseMusic();
}

void Music::ResetMusic()
{
	Mix_Music *music = NULL;
	LoadAudio(audioPath);
	PlayAudio();
}

void Music::SetVolume(int volumePercentage)
{
	Mix_VolumeMusic((float (MIX_MAX_VOLUME) / 100.0f)  * volumePercentage);
}

Music::~Music()
{
	//Mix_FreeMusic(music);
	//music = NULL;
}
