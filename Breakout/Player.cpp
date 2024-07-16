#include "Player.h"

Player::Player(SDL_Renderer* _renderer, std::string _file_path, std::string _sound_path,
	int _x, int _y, int _w, int _h, float _speed) :

	GameObject{ _renderer, _file_path, _x, _y, _w, _h, _speed },
	SoundEffect{ _sound_path },

	m_direction{ STILL }
{}

Player::~Player()
{}

void Player::Update(float _deltaTime)
{
	switch (this->m_direction)
	{
	case RIGHT:
		this->m_position.x += static_cast<int>(this->m_speed * _deltaTime);
		break;
	case LEFT:
		this->m_position.x -= static_cast<int>(this->m_speed * _deltaTime);
		break;
	default:
		break;
	}

	this->m_direction = STILL;
}

void Player::Draw()
{
	Sprite::Draw();
}