#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#ifndef _INIT_SDL_H_
#define _INIT_SDL_H_

// Macros that describe the default width and height of the window
#define WINSIZE_X 1280 // 1280 pixels in x-axis (width)
#define WINSIZE_Y 720  // 720 pixels in y-axis (height)

bool InitSDL();
// Post: ensure that the program has access to everything in the main
//		 SDL2 library

bool InitTTF();
// Post: ensure that the program has access to everything in the TTF
//		 SDL2 library

bool AccessAudioDevice();
// Post: ensure that the program has access to an audio device

bool CreateWindow(SDL_Window** _window);
// Pre:  'SDL_Window* argument' == nullptr
// Post: 'SDL_Window* argument' will have a set width and height,
//		 positioned at the centre of the screen, and be resizable

bool CreateRenderer(SDL_Window** _window, SDL_Renderer** _renderer);
// Pre:  'SDL_Renderer* r_argument' == nullptr
//		 'SDL_Window* w_argument' != nullptr
// Post: 'SDL_Renderer* w_argument' will have a dedicated renderer
//		 that outputs colours, images, and audio sounds

void SetupRenderer(SDL_Renderer** _renderer);
// Pre:  'SDL_Renderer* argument' != nullptr
// Post: ensure that the renderer size output matches the size of the window

bool SetupTexture(SDL_Renderer** _renderer, SDL_Texture** _image, std::string _filePath);
// Pre:  'SDL_Renderer* argument' != nullptr
//		 'SDL_Texture* arugument' == nullptr
//		 'filePath' != NULL
// Post: 'SDL_Texture* argument' will be assigned an individual file

void QuitSDL(SDL_Renderer** _renderer, SDL_Window** _window);
// Pre:  'SDL_Renderer* argument' != nullptr
//		 'SDL_Window* argument' != nullptr
// Post: window and renderer will be destroyed and program will quit SDL2

#endif