#pragma once
#include "GameObject.h"
#include <SDL_ttf.h>
#include <string>

class Score : public GameObject
{
public:

	// CTOR/DTOR
	Score(SDL_Renderer* _renderer, std::string _font,	
		int _font_size, std::string _lable, int _score);
	~Score();

	// Update/Draw object
	void Update(float _deltaTime) override;
	void Draw() override;

	// Getters/Setters
	int GetScore() const	  { return this->m_score; }
	void SetScore(int _score) { this->m_score = _score; }

	// Score increaser/decreaser
	void IncrementScore(int _score) { this->m_score += _score; }
	void DecrementScore(int _score) { this->m_score -= _score; }

private:

	int m_score;
	std::string m_lable;

	TTF_Font* m_font;
};