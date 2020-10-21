#pragma once
#include "Audio.h"

class Music : public Audio
{
public:
	Mix_Music *music = NULL;

	void LoadAudio(const char* musicPath);

	void PlayAudio();

	void ResumeOrPauseMusic();

	void ResetMusic();

	void SetVolume(int volumePercentage);

	~Music();
};