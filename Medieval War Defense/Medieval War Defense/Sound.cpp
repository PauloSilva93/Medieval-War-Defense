#include "Sound.h"

void Sound::LoadAudio(const char* soundPath)
{
	audioPath = soundPath;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	sound = Mix_LoadWAV(soundPath);
	if (sound == NULL)
	{
		std::cout << ("Failed to load sound! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

void Sound::PlayAudio()
{
	Mix_PlayChannel(-1, sound, 0);
}

void Sound::SetVolume(int volumePercentage)
{
	Mix_VolumeChunk(sound, volumePercentage);
}

Sound::~Sound()
{
	//Mix_FreeChunk(sound);
	//sound = NULL;
}