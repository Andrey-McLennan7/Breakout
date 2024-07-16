#pragma once
#include <SDL_mixer.h>
#include <string>

class SoundEffect
{
public:

	// CTOR/DTOR
	SoundEffect(std::string _sound_path);
	~SoundEffect();

	// Load/Play sound effect
	void Load(std::string _sound_path);
	void Play();

protected:

	Mix_Chunk* m_sound_effect;
};