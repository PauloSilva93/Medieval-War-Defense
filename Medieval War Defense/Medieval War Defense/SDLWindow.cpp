#include "SDLWindow.h"
#include <iostream>

SDLWindow::SDLWindow(int windowWidth, int windowHeight, const char* windowName)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->windowName = windowName;

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		throw(std::string("Failed to initialize GLAD"));
	}

	glViewport(0, 0, windowWidth, windowHeight);
}

SDL_Window* SDLWindow::SDL_GetWindow()
{
	return window;
}

Uint32 SDLWindow::SDL_ManageEvent()
{
	while (SDL_PollEvent(&evnt)) {

		return evnt.type;
	}
	return 0;
}

void SDLWindow::SDL_SwapWindow()
{
	SDL_GL_SwapWindow(this->window);
}

SDL_Event SDLWindow::getEvnt()
{
	return this->evnt;
}

int SDLWindow::getWindowWidth()
{
	return this->windowWidth;
}

int SDLWindow::getWindowHeight()
{
	return this->windowHeight;
}