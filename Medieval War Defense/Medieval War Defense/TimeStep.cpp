#include "TimeStep.h"

float TimeStep::step(int fps)
{
	if (firstRun)
	{
		currentTime = SDL_GetTicks() / 1000.0f;
		firstRun = false;
	}

	this->fps = fps;

	renderTime = 0;
	newTime = SDL_GetTicks() / 1000.0f;
	frameTime = newTime - currentTime;
	currentTime = newTime;
	renderTime += frameTime;
	return renderTime * fps;
}
