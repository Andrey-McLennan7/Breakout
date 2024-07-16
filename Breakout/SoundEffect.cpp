#include "SoundEffect.h"

SoundEffect::SoundEffect(std::string _sound_path) :
	m_sound_effect{ nullptr }
{
	this->Load(_sound_path);
}

SoundEffect::~SoundEffect()
{
	if (this->m_sound_effect)
	{
		Mix_FreeChunk(this->m_sound_effect);
	}
}

void SoundEffect::Load(std::string _sound_path)
{
	this->m_sound_effect = Mix_LoadWAV(_sound_path.c_str());
}

void SoundEffect::Play()
{
	Mix_PlayChannel(-1, this->m_sound_effect, 0);
}