#pragma once
#include "GameObject.h"
#include "SoundEffect.h"
#include <math.h>

#ifndef _BALL_H_
#define _BALL_H_

struct Velocity
{
	float x;
	float y;
};

class Ball : public GameObject, public SoundEffect
{
public:

	// CTOR/DTOR
	Ball(SDL_Renderer* _renderer, std::string _file_path, std::string _sound_path,
		int _x, int _y, float _dirX, float _dirY, int _w, int _h, float _speed);
	virtual ~Ball();

	// Control the direction of th ball movement
	void SetDirection(float _dirX, float _dirY);

	// Setter/Getter
	void SetDirX(float _x) { this->m_velocity.x = _x; }
	void SetDirY(float _y) { this->m_velocity.y = _y; }
	float GetDirX() const { return this->m_velocity.x; }
	float GetDirY() const { return this->m_velocity.y; }

	// Changers
	void ChangeDirX(float _x) { this->m_velocity.x *= _x; }
	void ChangeDirY(float _y) { this->m_velocity.y *= _y; }

	// Update/Draw object
	void Update(float _deltaTime) override;
	void Draw() override;

private:

	Velocity m_velocity;
};

#endif