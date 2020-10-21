#pragma once
#include <iostream>
#include <SDL/SDL.h>
#include <GLAD/glad.h>

class SDLWindow
{
	SDL_Window* window;
	SDL_Event evnt;
	int windowWidth;
	int windowHeight;
	const char* windowName;

public:
	SDLWindow(int windowWidth, int windowHeight, const char* windowName);

	SDL_Window* SDL_GetWindow();

	Uint32 SDL_ManageEvent();

	void SDL_SwapWindow();

	template <class T>
	void ProcessInput(SDL_Scancode key, T *variable, T value)
	{
		const Uint8*  currentKeyStates = SDL_GetKeyboardState(NULL);

		if (currentKeyStates[key])
		{
			*variable = value;
		}
	}

	SDL_Event getEvnt();

	int getWindowWidth();

	int getWindowHeight();
};