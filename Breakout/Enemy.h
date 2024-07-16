#pragma once
#include "GameObject.h"
#include "SoundEffect.h"

#ifndef _BRICK_H_
#define _BRICK_H_

class Enemy : public GameObject, public SoundEffect
{
public:

	Enemy(SDL_Renderer* _renderer, SDL_Texture* _image, std::string _sound_path,
		int _x, int _y, int _w, int _h, int _type, bool _state);
	virtual ~Enemy();

	// Update object
	void Update(float _deltaTime) override;

	// Setters
	void SetColourTypeAndPoints(int _type, SDL_Texture* _image);
	void SetState(bool _state) { this->m_state = _state; }
	void SetPoints(int _points) { this->m_points = _points; }

	// Getters
	int GetColourType() const { return this->m_colour_type; }
	    bool GetState() const { return this->m_state; }
		int GetPoints() const { return this->m_points; }

	// Has asset
	bool HasAsset() const;

private:

	int m_colour_type;
	bool m_state;
	int m_points;
};

#endif 