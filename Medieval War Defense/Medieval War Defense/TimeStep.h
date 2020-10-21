#pragma once
#include <SDL/SDL.h>

class TimeStep
{
	float startTime = 0.0f;
	float currentTime = 0.0f;
	float newTime = 0.0f;
	float frameTime = 0.0f;
	float renderTime = 0.0f;
	int fps = 0;
	bool firstRun = true;

public:

	float step(int fps);

};