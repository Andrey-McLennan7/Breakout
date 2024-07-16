#include "Score.h"

Score::Score(SDL_Renderer* _renderer, std::string _font,
	int _font_size, std::string _lable, int _score) :

	GameObject(_renderer, "", 0, 0, 0, 0, 0),
	m_lable{ _lable }, m_score{ _score }, m_font{ nullptr }
{
	this->m_font = TTF_OpenFont(_font.c_str(), _font_size);
}

Score::~Score()
{
	if (this->m_font != nullptr)
	{
		TTF_CloseFont(this->m_font);
	}
}

void Score::Update(float _deltaTime)
{
	std::string lable{ this->m_lable };

	lable += std::to_string(this->m_score);

	SDL_Surface* score_surface = TTF_RenderText_Solid(this->m_font, lable.c_str(), { 255, 255, 255, 255 });
	this->m_image = SDL_CreateTextureFromSurface(this->m_renderer, score_surface);
	SDL_FreeSurface(score_surface);

	SDL_QueryTexture(this->m_image, 0, 0, &this->m_position.w, &this->m_position.h);
}

void Score::Draw()
{
	Sprite::Draw();

	SDL_DestroyTexture(this->m_image);
}