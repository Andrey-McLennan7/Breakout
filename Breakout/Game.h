#pragma once
#include "InitSDL.h"
#include "Player.h"
#include "Ball.h"
#include "Enemy.h"
#include "Score.h"
#include <vector>

#ifndef _GAME_H_
#define _GAME_H_

// Define series of paths
#define BACKGROUND_MUSIC_PATH "./mixer/MP3/background.mp3"
#define INSTRUCTIONS_IMAGE_PATH "./images/Instructions.bmp"

#define PADDLE_SOUND_PATH "./mixer/WAV/shot.wav"
#define PADDLE_IMAGE_PATH "./images/paddle.bmp"

#define BALL_SOUND_PATH "./mixer/WAV/miss.wav"
#define BALL_IMAGE_PATH "./images/ball.bmp"

#define ENEMY_SOUND_PATH "./mixer/WAV/kill.wav"

#define RED_ENEMY_PATH	  "./images/enemies/red.bmp"
#define ORANGE_ENEMY_PATH "./images/enemies/orange.bmp"
#define YELLOW_ENEMY_PATH "./images/enemies/yellow.bmp"
#define GREEN_ENEMY_PATH  "./images/enemies/green.bmp"
#define BLUE_ENEMY_PATH   "./images/enemies/blue.bmp"

#define WIN_SOUND_PATH "./mixer/WAV/win.wav"
#define EXTRA_LIFE_SOUND_PATH "./mixer/WAV/life.wav"

#define FONT_PATH "./ttf/Pixel12x10.ttf"

class Game
{
public:

	Game();
	~Game();

	bool InitialiseGame();

	void ShowInstructions();

	void RunGameLoop();

private:

	// Window and renderer
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	// Backgournd music
	Mix_Music* m_background_music;
	Mix_Chunk* m_round_won;
	Mix_Chunk* m_extran_life;

	// Game loop controller
	bool m_isRunning;

	// Controll frame-rate (FPS)
	Uint32 m_tickCount;

	// Show instructions image
	SDL_Texture* m_instructions;

	// Game objects
	// Playable paddle
	Player* m_paddle;
	bool m_paddleTick;

	// Bouncing ball
	Ball* m_ball;

	// Array of enemy 
	std::vector<Enemy*> m_aliens;
	const int m_row;
	const int m_col;
	const int m_table;

	// Textures for all enemies
	SDL_Texture** m_alien_textures;

	int m_number_of_types;

	// Score systems
	Score* m_score;		  // Current highscore
	Score* m_aliens_left; // Number of alieans left
	Score* m_rounds;	  // Number of rounds beaten
	Score* m_lives;		  // Number of lives left

	int m_life_score;

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void CheckPaddlePosition();

	// Ball collision
	void CheckBallCollisionWithWall();
	void CheckBallCollisionWithPaddle();
	void CheckBallCollisionWithEnemy();

	// Reflect ball
	float ReflectBallFromPaddle(float _hitX);
	void ReflectBallFromAlien(int _dirIndex);

	// Create the initial layout of the aliens
	void CreateEnemyLayout();
	void AddEnemy(int _x, int _y, int _w, int _h, int _type);
	void ClearEnemies();

	void ControllPaddleWithMouse(float _x);

	void ResetPaddleAndBall();
	void SetBallInitialPosition();
	void ResetEnemyState();

	void CheckNumberOfEnemies();

	void ResetLayout();
	void ResetGame();
};

#endif