#pragma once
#include <SDL.h>
#include <string>

#ifndef _SPRITE_H_
#define _SPRITE_H_

class Sprite
{
public:

	// CTOR/DTOR
	Sprite(SDL_Renderer* _renderer, std::string _file, int _x, int _y, int _w, int _h);
	virtual ~Sprite();

	// Setters
	void SetX(float _x) { this->m_position.x = static_cast<int>(_x); }
	void SetY(float _y) { this->m_position.y = static_cast<int>(_y); }
	void SetW(float _w) { this->m_position.w = static_cast<int>(_w); }
	void SetH(float _h) { this->m_position.h = static_cast<int>(_h); }

	// Getters
	int GetX() const { return this->m_position.x; }
	int GetY() const { return this->m_position.y; }
	int GetW() const { return this->m_position.w; }
	int GetH() const { return this->m_position.h; }

	// Draw object
	virtual void Draw();

protected:
	
	SDL_Renderer* m_renderer;
	SDL_Texture* m_image;
	SDL_Rect m_position;
};

#endif