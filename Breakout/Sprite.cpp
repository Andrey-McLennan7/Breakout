#include "Sprite.h"

Sprite::Sprite(SDL_Renderer* _renderer, std::string _file, int _x, int _y, int _w, int _h) :
	m_renderer{ _renderer },
	m_position{ _x, _y, _w, _h },
	m_image{ nullptr }
{
	// Apply texture
	if (!_file.empty())
	{
		SDL_Surface* bmpFile{ SDL_LoadBMP(_file.c_str()) };
		this->m_image = SDL_CreateTextureFromSurface(this->m_renderer, bmpFile);
		SDL_FreeSurface(bmpFile);
	}
}

Sprite::~Sprite()
{
	// Clean resources
	if (this->m_image != nullptr)
	{
		SDL_DestroyTexture(this->m_image);
	}
}

void Sprite::Draw()
{
	if (this->m_image != nullptr)
	{
		// Draw asset on object
		SDL_RenderCopy(this->m_renderer, this->m_image, 0, &this->m_position);
	}
	else
	{
		// Use the renderer's current colour if no such image exists
		SDL_RenderFillRect(this->m_renderer, &this->m_position);
	}
}