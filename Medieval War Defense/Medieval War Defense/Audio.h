#pragma once
#include <iostream>
#include <SDL/SDL_mixer.h>

class Audio
{
public:
	const char* audioPath;
	virtual void LoadAudio(const char* audioPath) = 0;
	virtual void PlayAudio() = 0;
	virtual void SetVolume(int volumePercentage) = 0;
};