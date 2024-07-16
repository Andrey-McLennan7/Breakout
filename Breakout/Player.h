#pragma once
#include "GameObject.h"
#include "SoundEffect.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

enum Direction
{
	STILL = 0,
	LEFT = 1,
	RIGHT = 2
};

class Player : public GameObject, public SoundEffect
{
public:

	// CTOR/DTOR
	Player(SDL_Renderer* _renderer, std::string _file_path, std::string _sound_path,
		int _x, int _y, int _w, int _h, float _speed);
	virtual ~Player();

	// Getters/Setters
	void SetDirection(Direction _dir) { this->m_direction = _dir; }
	Direction GetDiretion() { return this->m_direction; }

	// Update/Draw object
	void Update(float _deltaTime) override;
	void Draw()	override;

private:

	Direction m_direction;
};

#endif