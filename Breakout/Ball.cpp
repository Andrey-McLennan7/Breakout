#include "Ball.h"

Ball::Ball(SDL_Renderer* _renderer, std::string _file_path, std::string _sound_path,
	int _x, int _y, float _dirX, float _dirY, int _w, int _h, float _speed) :

	GameObject{ _renderer, _file_path, _x, _y, _w, _h, _speed },
	SoundEffect(_sound_path),

	m_velocity{ 0.0f, 0.0f }
{
	this->SetDirection(_dirX, _dirY);
}

Ball::~Ball()
{}

void Ball::SetDirection(float _dirX, float _dirY)
{
	// Normalise the direction vector
	float r{ static_cast<float>(sqrt(pow(_dirX, 2) + pow(_dirY, 2))) };

	// Multiply unit vector with the ball speed
	this->m_velocity.x = this->m_speed * (_dirX / r);
	this->m_velocity.y = this->m_speed * (_dirY / r);
}

void Ball::Update(float _deltaTime)
{
	// Move ball vertically based on direction
	this->m_position.x += static_cast<int>(this->m_velocity.x * _deltaTime);
	this->m_position.y += static_cast<int>(this->m_velocity.y * _deltaTime);
}

void Ball::Draw()
{
	Sprite::Draw();
}