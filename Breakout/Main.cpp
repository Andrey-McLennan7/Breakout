#include "Game.h"

int main(int argc, char* argv[])
{
	Game* game{ new Game() };

	if (game->InitialiseGame())
	{
		game->ShowInstructions();
		game->RunGameLoop();
	}

	delete game;

	return 0;
}