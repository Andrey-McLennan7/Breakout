#include "Enemy.h"

Enemy::Enemy(SDL_Renderer* _renderer, SDL_Texture* _image, std::string _sound_path,
	int _x, int _y, int _w, int _h, int _type, bool _state) :

	GameObject{ _renderer, "", _x, _y, _w, _h, 0 },
	SoundEffect{ _sound_path },

	m_state{ _state }, m_points{ 0 }
{
	// type: (0 : red), (1 : orange), (2 : yellow), (3 : green), (4 : blue)
	this->SetColourTypeAndPoints(_type, _image);
}

Enemy::~Enemy()
{}

void Enemy::Update(float _deltaTime)
{}

void Enemy::SetColourTypeAndPoints(int _type, SDL_Texture* _image)
{
	if (_type >= 0 && _type < 5)
	{
		this->m_colour_type = _type;
		this->m_image = _image;

		// type: (0 : red), (1 : orange), (2 : yellow), (3 : green), (4 : blue)
		switch (this->m_colour_type)
		{
		case 0:
			this->m_points = 100;
			break;
		case 1:
			this->m_points = 70;
			break;
		case 2:
			this->m_points = 50;
			break;
		case 3:
			this->m_points = 20;
			break;
		case 4:
			this->m_points = 10;
			break;
		}
	}
}

bool Enemy::HasAsset() const
{
	if (this->m_image != nullptr)
	{
		return true;
	}

	return false;
}