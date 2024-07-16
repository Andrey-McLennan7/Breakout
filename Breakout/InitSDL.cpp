#include "InitSDL.h"

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_Log("Failed to initialise SDL2 : %s", SDL_GetError());
		return false;
	}

	return true;
}

bool InitTTF()
{
	if (TTF_Init())
	{
		SDL_Log("Failed to initialise TTF : %s", SDL_GetError());
		return false;
	}

	return true;
}

bool AccessAudioDevice()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
	{
		SDL_Log("Failed to access audio device : %s", SDL_GetError());
		return false;
	}

	return true;
}

bool CreateWindow(SDL_Window** _window)
{
	*_window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINSIZE_X, WINSIZE_Y, SDL_WINDOW_SHOWN);

	if (!*_window)
	{
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	return true;
}

bool CreateRenderer(SDL_Window** _window, SDL_Renderer** _renderer)
{
	*_renderer = SDL_CreateRenderer(*_window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!*_renderer)
	{
		SDL_Log("Failed to create renderer : %s", SDL_GetError());
		return false;
	}

	return true;
}

void SetupRenderer(SDL_Renderer** _renderer)
{
	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize(*_renderer, WINSIZE_X, WINSIZE_Y);

	// Set the renderer's initial colour to black
	SDL_SetRenderDrawColor(*_renderer, 0, 0, 0, 255);
}

bool SetupTexture(SDL_Renderer** _renderer, SDL_Texture** _image, std::string _filePath)
{
	SDL_Surface* bmpFile{ SDL_LoadBMP(_filePath.c_str()) };
	*_image = SDL_CreateTextureFromSurface(*_renderer, bmpFile);
	SDL_FreeSurface(bmpFile);

	if (!*_image)
	{
		SDL_Log("Failed to initialise image : %s %s", _filePath.c_str(), SDL_GetError());
		return false;
	}

	return true;
}

void QuitSDL(SDL_Renderer** _renderer, SDL_Window** _window)
{
	Mix_CloseAudio();

	SDL_DestroyRenderer(*_renderer);
	SDL_DestroyWindow(*_window);

	TTF_Quit();
	SDL_Quit();
}