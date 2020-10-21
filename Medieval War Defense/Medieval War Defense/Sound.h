#pragma once
#include "Audio.h"

class Sound : public Audio
{
public:
	Mix_Chunk *sound = NULL;

	void LoadAudio(const char* soundPath);

	void PlayAudio();

	void SetVolume(int volumePercentage);

	~Sound();
};