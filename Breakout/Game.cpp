#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ nullptr },
	m_renderer{ nullptr },
	m_background_music{ nullptr },
	m_round_won{ nullptr },
	m_extran_life{ nullptr },
	m_isRunning{ true },
	m_tickCount{ 0 },
	m_instructions{ nullptr },
	m_paddle{ nullptr },
	m_ball{ nullptr },
	m_alien_textures{ new SDL_Texture * [5]{ nullptr } },
	m_number_of_types{ 0 },
	m_score{ nullptr },
	m_aliens_left{ nullptr },
	m_rounds{ nullptr },
	m_lives{ nullptr },
	m_life_score{ 10000 },
	m_paddleTick{ true },
	m_row{ 10 },
	m_col{ 25 },
	m_table{ this->m_row * this->m_col }
{}

Game::~Game()
{
	delete this->m_paddle;
	delete this->m_ball;

	this->ClearEnemies();

	delete[] this->m_alien_textures;

	delete this->m_score;
	delete this->m_aliens_left;
	delete this->m_rounds;
	delete this->m_lives;

	Mix_FreeMusic(this->m_background_music);

	Mix_FreeChunk(this->m_round_won);

	QuitSDL(&this->m_renderer, &this->m_window);
}

bool Game::InitialiseGame()
{
	if (!InitSDL())
	{
		return false;
	}

	if (!InitTTF())
	{
		return false;
	}

	if (!AccessAudioDevice())
	{
		return false;
	}

	if (!CreateWindow(&this->m_window))
	{
		return false;
	}

	if (!CreateRenderer(&this->m_window, &this->m_renderer))
	{
		return false;
	}

	SetupRenderer(&this->m_renderer);

	if (!SetupTexture(&this->m_renderer, &this->m_instructions, INSTRUCTIONS_IMAGE_PATH))
	{
		return false;
	}

	std::string enemyPath[]
	{
		RED_ENEMY_PATH,
		ORANGE_ENEMY_PATH,
		YELLOW_ENEMY_PATH,
		GREEN_ENEMY_PATH,
		BLUE_ENEMY_PATH
	};

	this->m_number_of_types = sizeof(enemyPath) / sizeof(std::string);

	for (int i{ 0 }; i < this->m_number_of_types; ++i)
	{
		if (!SetupTexture(&this->m_renderer, &this->m_alien_textures[i], enemyPath[i]))
		{
			return false;
		}
	}

	this->m_paddle = new Player
	{
		this->m_renderer,			   // Access main renderer
		PADDLE_IMAGE_PATH,		       // File path name
		PADDLE_SOUND_PATH,			   // Sound path
		WINSIZE_X / 2, WINSIZE_Y - 25, // Position (x, y)
		200, 20,					   // Size (width, height)
		600.0f						   // Speed
	};

	this->m_ball = new Ball
	{
		this->m_renderer,			  // Access main renderer
		BALL_IMAGE_PATH,			  // File path
		BALL_SOUND_PATH,		      // Sound path
		WINSIZE_X / 2, WINSIZE_Y / 2, // Position (x, y)
		-1, -1,						  // Velocity (dirX, dirY)
		15, 15,						  // Size (width, height)
		600.0f						  // Speed
	};

	this->CreateEnemyLayout();

	this->m_score = new Score
	{
		this->m_renderer, // Access main renderer
		FONT_PATH, 30,	  // Font path and font size
		"SCORE:", 0		  // Message and starting score
	};

	this->m_aliens_left = new Score
	{
		this->m_renderer, // Access main renderer
		FONT_PATH, 30,    // Font path and font size
		"ALIENS LEFT : ", // Message
		this->m_table	  // Starting score
	};

	this->m_rounds = new Score
	{
		this->m_renderer, // Access main renderer
		FONT_PATH, 30,    // Font path and font size
		"ROUND:", 1		  // Number of rounds
	};

	this->m_lives = new Score
	{
		this->m_renderer, // Access main renderer
		FONT_PATH, 30,    // Font path and font size
		"LIVES:", 3		  // Number of lives left
	};

	this->m_background_music = Mix_LoadMUS(BACKGROUND_MUSIC_PATH);

	this->m_round_won = Mix_LoadWAV(WIN_SOUND_PATH);
	this->m_extran_life = Mix_LoadWAV(EXTRA_LIFE_SOUND_PATH);

	return true;
}

void Game::ShowInstructions()
{
	SDL_Rect src{ 0, 0, 0, 0 };
	bool showingInstructions{ true };

	SDL_QueryTexture(this->m_instructions, 0, 0, &src.w, &src.h);

	while (showingInstructions)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				this->m_isRunning = false;
				showingInstructions = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_TAB:
					showingInstructions = false;
					break;
				case SDLK_ESCAPE:
					this->m_isRunning = false;
					showingInstructions = false;
					break;
				}
			}
		}

		SDL_RenderClear(this->m_renderer);

		SDL_RenderCopy(this->m_renderer, this->m_instructions, &src, 0);

		SDL_RenderPresent(this->m_renderer);
	};

	SDL_DestroyTexture(this->m_instructions);

	SDL_SetRenderDrawColor(this->m_renderer, 000, 000, 000, 255);
}

void Game::RunGameLoop()
{
	while (this->m_isRunning)
	{
		this->ProcessInput();
		this->UpdateGame();
		this->GenerateOutput();
	}
}

void Game::ProcessInput()
{
	// Use this to read mouse input
	SDL_Event event;

	// Use this to read keyboard input
	const Uint8* state{ SDL_GetKeyboardState(NULL) };

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE])
		{
			// if the user clicks on the window cross or presses the ESC key,
			// end the game loop
			this->m_isRunning = false;
		}
	}

	// Keyboard input
	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
	{
		// Press A/LEFT to move the paddle to the left
		this->m_paddle->SetDirection(LEFT);
	}

	if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
	{
		// Press D/RIGHT to move the paddle to the right
		this->m_paddle->SetDirection(RIGHT);
	}

	// Launch ball
	if (state[SDL_SCANCODE_SPACE])
	{
		if (this->m_paddleTick)
		{
			this->m_paddleTick = false;
			this->m_ball->SetDirection(1, 1);
			Mix_PlayMusic(this->m_background_music, -1);
		}
	}

	// Mouse input
	int mx{ 0 }, my{ 0 };
	Uint8 mouse = SDL_GetMouseState(&mx, &my);
	if (mouse & ( SDL_BUTTON(1) | SDL_BUTTON(3) ) )
	{
		// Left/Right-Click and hold to move paddle left or right with mouse movement
		this->ControllPaddleWithMouse(static_cast<float>(mx - (this->m_paddle->GetW() / 2)));
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while ( !SDL_TICKS_PASSED( SDL_GetTicks64(), this->m_tickCount + 16 ) );
	
	// Delta time is the difference in ticks from last frame
	float deltaTime{ ( SDL_GetTicks64() - this->m_tickCount ) / 1000.0f };
	this->m_tickCount = SDL_GetTicks64();

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update the player's position based on delta time
	this->m_paddle->Update(deltaTime);
	this->CheckPaddlePosition();

	// Update the ball's position based on delta time
	if (this->m_paddleTick)
	{
		this->SetBallInitialPosition();
	}
	else
	{
		this->m_ball->Update(deltaTime);
	}

	this->CheckBallCollisionWithWall();
	this->CheckBallCollisionWithPaddle();
	this->CheckBallCollisionWithEnemy();

	this->CheckNumberOfEnemies();

	this->m_score->Update(deltaTime);
	this->m_score->SetX(WINSIZE_X - (this->m_score->GetW() + 10) );
	this->m_score->SetY(10);

	this->m_aliens_left->Update(deltaTime);
	this->m_aliens_left->SetX(10);
	this->m_aliens_left->SetY(10);

	this->m_rounds->Update(deltaTime);
	this->m_rounds->SetX((WINSIZE_X / 2) - 75);
	this->m_rounds->SetY(10);

	// If the player gets a certain amount in score
	// they get an extra life
	if (this->m_score->GetScore() >= this->m_life_score)
	{
		// Increase the score
		this->m_lives->IncrementScore(1);

		this->m_life_score += 10000;

		// Play sound effect
		Mix_PlayChannel(-1, this->m_extran_life, 0);
	}

	this->m_lives->Update(deltaTime);
	this->m_lives->SetX(WINSIZE_X - (this->m_score->GetW() + 10 + this->m_lives->GetW() + 20) );
	this->m_lives->SetY(10);

	if (this->m_lives->GetScore() == 0)
	{
		this->ResetGame();
	}
}

void Game::GenerateOutput()
{
	SDL_RenderClear(this->m_renderer);

	for (int i{ 0 }; i < this->m_table; ++i)
	{
		// Check if alien is present
		if (this->m_aliens[i]->GetState())
		{
			// Check if the game object has a texture
			if (!this->m_aliens[i]->HasAsset())
			{
				switch (this->m_aliens[i]->GetColourType())
				{
				case 0: SDL_SetRenderDrawColor(this->m_renderer, 255, 000, 000, 255); break;
				case 1: SDL_SetRenderDrawColor(this->m_renderer, 255, 165, 000, 255); break;
				case 2: SDL_SetRenderDrawColor(this->m_renderer, 255, 255, 000, 255); break;
				case 3: SDL_SetRenderDrawColor(this->m_renderer, 000, 255, 000, 255); break;
				case 4: SDL_SetRenderDrawColor(this->m_renderer, 000, 000, 255, 255); break;
				}
			}

			this->m_aliens[i]->Draw();
		}
	}

	SDL_SetRenderDrawColor(this->m_renderer, 255, 255, 255, 255);

	this->m_paddle->Draw();
	this->m_ball->Draw();

	this->m_score->Draw();
	this->m_aliens_left->Draw();
	this->m_rounds->Draw();
	this->m_lives->Draw();

	SDL_SetRenderDrawColor(this->m_renderer, 000, 000, 000, 255);
	SDL_RenderPresent(this->m_renderer);
}

void Game::CheckPaddlePosition()
{
	// Check if postion is within the boundaries of the window
	if (this->m_paddle->GetLeft() < 0)
	{
		// Far left
		this->m_paddle->SetX(0);
	}
	else if ( this->m_paddle->GetRight() > WINSIZE_X)
	{
		// Far right
		this->m_paddle->SetX( WINSIZE_X - this->m_paddle->GetW() );
	}
}

void Game::CheckBallCollisionWithWall()
{
	// Top and bottom
	if (this->m_ball->GetTop() < 50)
	{
		// Top
		// Keep ball within the screen and refelct in the y direction
		this->m_ball->SetY(50.0f);
		this->m_ball->ChangeDirY(-1.0f);
	}
	else if (this->m_ball->GetBottom() > WINSIZE_Y)
	{
		// Bottom
		// Reset the ball's position back to the original position
		this->ResetPaddleAndBall();

		// Pause and rewind music
		Mix_PauseMusic();
		Mix_RewindMusic();

		// Play sound effect
		this->m_ball->Play();

		// Reduce number of lives
		this->m_lives->DecrementScore(1);
	}

	// Left and right
	if (this->m_ball->GetLeft() <= 0)
	{
		// Left
		// Keep ball within screen and reflect in the x direction
		this->m_ball->SetX(0);
		this->m_ball->ChangeDirX(-1.0f);
	}
	else if (this->m_ball->GetRight() >= WINSIZE_X)
	{
		// Right
		// Keep ball within screen and reflect in the x direction
		this->m_ball->SetX(WINSIZE_X - this->m_ball->GetW());
		this->m_ball->ChangeDirX(-1.0f);
	}
}

void Game::CheckBallCollisionWithPaddle()
{
	if (this->m_ball->CheckCollision(this->m_paddle))
	{
		// Get the centre in the x-coordinate of the ball
		float ballCentre{ static_cast<float>(this->m_ball->GetX() + this->m_ball->GetW() / 2) };

		this->m_ball->SetY( this->m_paddle->GetY() - this->m_ball->GetH() );

		float x_direction{ this->ReflectBallFromPaddle(ballCentre - this->m_paddle->GetX() ) };

		this->m_ball->SetDirection(x_direction, -1);

		// Play sound effect
		this->m_paddle->Play();
	}
}

void Game::CheckBallCollisionWithEnemy()
{
	for (int i{ 0 }; i < this->m_table; ++i)
	{
		// Check if alien is present
		if (this->m_aliens[i]->GetState())
		{
			// Get the centre in the x- and y- coordinate of the ball
			float ballCentreX{ static_cast<float>(this->m_ball->GetX() + this->m_ball->GetW() / 2) };
			float ballCentreY{ static_cast<float>(this->m_ball->GetY() + this->m_ball->GetH() / 2) };

			// Get the centre in the x- and y- coordinate of the alien
			float alienCentreX{ static_cast<float>(this->m_aliens[i]->GetX() + this->m_aliens[i]->GetW() / 2)};
			float alienCentreY{ static_cast<float>(this->m_aliens[i]->GetY() + this->m_aliens[i]->GetH() / 2)};

			if ( this->m_ball->CheckCollision(this->m_aliens[i]) )
			{
				// Collision detected, remove the alien
				this->m_aliens[i]->SetState(false);

				// Play sound effect
				this->m_aliens[i]->Play();

				// Reduce the number of aliens
				this->m_aliens_left->DecrementScore(1);

				// Increase score based on alien type
				this->m_score->IncrementScore(this->m_aliens[i]->GetPoints());

				// Calculate y-size
				float minY{ 0.0f };
				float maxY{ 0.0f };

				if ( this->m_aliens[i]->GetTop() > this->m_ball->GetTop() )
				{
					minY = this->m_aliens[i]->GetTop();
				}
				else
				{
					minY = this->m_ball->GetY();
				}

				if ( this->m_aliens[i]->GetBottom() < this->m_ball->GetBottom() )
				{
					maxY = this->m_aliens[i]->GetBottom();
				}
				else
				{
					maxY = this->m_ball->GetBottom();
				}

				float sizeY{ maxY - minY };

				// Calculate x-size
				float minX{ 0.0f };
				float maxX{ 0.0f };

				if ( this->m_aliens[i]->GetLeft() > this->m_ball->GetLeft() )
				{
					minX = this->m_aliens[i]->GetLeft();
				}
				else
				{
					minX = this->m_ball->GetLeft();
				}

				if ( this->m_aliens[i]->GetRight() < this->m_ball->GetRight() )
				{
					maxX = this->m_aliens[i]->GetRight();
				}
				else
				{
					maxX = this->m_ball->GetRight();
				}

				float sizeX{ maxX - minX };

				int dirIndex{ 0 };
				// Set collision response
				if (sizeX > sizeY)
				{
					float newY{ static_cast<float>( this->m_ball->GetY() ) };

					if (ballCentreY > alienCentreY)
					{
						// Bottom
						newY += sizeY + 0.01f;
						dirIndex = 3;
					}
					else
					{
						// Top
						newY -= sizeY + 0.01f;
						dirIndex = 1;
					}

					this->m_ball->SetY(newY);
				}
				else
				{
					float newX{ static_cast<float>( this->m_ball->GetX() ) };

					if (ballCentreX < alienCentreX)
					{
						// Left
						newX -= sizeX + 0.01f;
					}
					else
					{
						newX += sizeX + 0.01f;
					}

					this->m_ball->SetX(newX);
				}

				this->ReflectBallFromAlien(dirIndex);

				return;
			}
		}
	}
}

float Game::ReflectBallFromPaddle(float _hitX)
{
	// Make sure that the _hitX variable is within the width of the paddle
	if (_hitX < 0)
	{
		_hitX = 0;
	}
	else if (_hitX > this->m_paddle->GetW())
	{
		_hitX = this->m_paddle->GetW();
	}

	// Everything in the left side of the paddle gets reflected to the left
	// Everything in the right side of the paddle gets reflected to the right
	_hitX -= this->m_paddle->GetW() / 2.0f;

	// Scale reflection, making it fall in the range -2.0f to 2.0f
	return 2.0f * ( _hitX / (this->m_paddle->GetW() / 2.0f) );
}

void Game::ReflectBallFromAlien(int _dirIndex)
{
	// dirindex 0: Left, 1: Top, 2: Right, 3: Bottom

	// Direction factors
	int mulX = 1;
	int mulY = 1;

	if (this->m_ball->GetDirX() > 0)
	{
		// Ball is moving in the positive x direction
		if (this->m_ball->GetDirY() > 0)
		{
			// Ball is moving in the positive y direction
			// +1 +1
			if (_dirIndex == 0 || _dirIndex == 3)
			{
				mulX = -1;
			}
			else
			{
				mulY = -1;
			}
		}
		else if (this->m_ball->GetDirY() < 0)
		{
			// Ball is moving in the negative y direction
			// +1 -1
			if (_dirIndex == 0 || _dirIndex == 1)
			{
				mulX = -1;
			}
			else
			{
				mulY = -1;
			}
		}
	}
	else if (this->m_ball->GetDirX() < 0)
	{
		// Ball is moving in the negative x direction
		if (this->m_ball->GetDirY() > 0)
		{
			// Ball is moving in the positive y direction
			// -1 +1
			if (_dirIndex == 2 || _dirIndex == 3)
			{
				mulX = -1;
			}
			else
			{
				mulY = -1;
			}
		}
		else if (this->m_ball->GetDirY() < 0)
		{
			// Ball is moving in the negative y direction
			// -1 -1
			if (_dirIndex == 1 || _dirIndex == 2)
			{
				mulX = -1;
			}
			else
			{
				mulY = -1;
			}
		}
	}

	// Set the new direction of the ball, by multiplying the old direction
	// with the determined direction factors
	this->m_ball->SetDirection(mulX * this->m_ball->GetDirX(), mulY * this->m_ball->GetDirY());
}

void Game::CreateEnemyLayout()
{
	int alx{ 4 };
	int aly{ 70 };
	int alw{ 50 };
	int alh{ 30 };
	int type{ 0 };

	for (int i{ 1 }; i <= this->m_row; ++i)
	{
		this->AddEnemy(alx, aly, alw, alh, type);
		aly += alh + 1;

		if (i % 2 == 0)
		{
			++type;
		}
	}
}

void Game::AddEnemy(int _x, int _y, int _w, int _h, int _type)
{
	// Assign the values for each brick per column
	for (int j{ 1 }; j <= this->m_col; ++j)
	{
		Enemy* alien
		{
			new Enemy
			{
				this->m_renderer,			   // Access main renderer
				this->m_alien_textures[_type], // Image path
				ENEMY_SOUND_PATH,			   // Sound path
				_x, _y, _w, _h,				   // Position (x, y) and Size (w, h)
				_type, true					   // type (0, 1, 2, 3, 4) and state (exists or destroyed)
			}
		};

		this->m_aliens.push_back(alien);

		_x += _w + 1;
	}
}

void Game::ClearEnemies()
{
	for (int i = 0; i < this->m_table; ++i)
	{
		delete this->m_aliens[i];
	}

	this->m_aliens.clear();

	for (int i{ 0 }; i < this->m_number_of_types; ++i)
	{
		SDL_DestroyTexture(this->m_alien_textures[i]);
	}
}

void Game::ControllPaddleWithMouse(float _x)
{
	if (STILL == this->m_paddle->GetDiretion())
	{
		if (_x < 0)
		{
			this->m_paddle->SetX(0.0f);
		}
		else if (_x + this->m_paddle->GetW() > WINSIZE_X)
		{
			this->m_paddle->SetX(WINSIZE_X - this->m_paddle->GetW() );
		}
		else
		{
			this->m_paddle->SetX(_x);
		}
	}
}

void Game::ResetPaddleAndBall()
{
	this->m_paddleTick = true;
	this->SetBallInitialPosition();
}

void Game::SetBallInitialPosition()
{
	this->m_ball->SetX( this->m_paddle->GetX() + this->m_paddle->GetW() / 2 - this->m_ball->GetW() / 2 );
	this->m_ball->SetY( this->m_paddle->GetTop() - this->m_ball->GetH() );
}

void Game::ResetEnemyState()
{
	for (int i{ 0 }; i < this->m_table; ++i)
	{
		if (!this->m_aliens[i]->GetState())
		{
			this->m_aliens[i]->SetState(true);
		}
	}
}

void Game::CheckNumberOfEnemies()
{
	// Check if there are any bricks (i.e., aliens) left
	if (this->m_aliens_left->GetScore() == 0)
	{
		// If they are all destroyed then reset game and continue
		// from current score
		this->ResetLayout();
		this->m_aliens_left->SetScore(this->m_table);

		if (this->m_rounds->GetScore() <= 5)
		{
			// Increase the ball and paddle speeds by 100
			this->m_paddle->IncreaseSpeed(100.0f);
			this->m_ball->IncreaseSpeed(100.0f);

			if (this->m_rounds->GetScore() <= 10 && this->m_rounds->GetScore() > 5)
			{
				// Decrease the paddles length
				float paddle_width{ static_cast<float>( this->m_paddle->GetW() ) };
				paddle_width -= 10;
				this->m_paddle->SetW(paddle_width);
			}
		}

		// Move to next round
		this->m_rounds->IncrementScore(1);

		// Pause and rewind music
		Mix_PauseMusic();
		Mix_RewindMusic();

		// Play round won sound effect
		Mix_PlayChannel(-1, this->m_round_won, 0);
	}
}

void Game::ResetLayout()
{
	this->ResetEnemyState();
	this->ResetPaddleAndBall();
}

void Game::ResetGame()
{
	this->ResetLayout();

	this->m_aliens_left->SetScore(this->m_table);
	this->m_score->SetScore(0);
	this->m_rounds->SetScore(1);
	this->m_lives->SetScore(3);

	this->m_ball->SetSpeed(600.0f);
	this->m_paddle->SetSpeed(600.0f);
}