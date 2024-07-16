	#pragma once
#include "Sprite.h"

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

class GameObject : public Sprite
{
public:

	// CTOR and DTOR
	GameObject(SDL_Renderer* _renderer, std::string _file,
		int _x, int _y, int _w, int _h, float _speed);
	virtual ~GameObject();

	// Setters/Getters
	void SetSpeed(float _speed) { this->m_speed = _speed; }
	int GetSpeed()				{ return this->m_speed; }

	// Increase/Decrease speed
	void IncreaseSpeed(float _speed) { this->m_speed += _speed; }
	void DecreaseSpeed(float _speed) { this->m_speed -= _speed; }

	// Get left, right, top, and bottom
	int GetLeft()   const { return this->m_position.x; }
	int GetRight()  const { return this->m_position.x + this->m_position.w; }
	int GetTop()	const { return this->m_position.y; }
	int GetBottom() const { return this->m_position.y + this->m_position.h; }

	// Game object behaviours
	bool CheckCollision(const GameObject* _obj);
	virtual void Update(float _deltaTime) = 0;

protected:

	float m_speed;
};

#endif