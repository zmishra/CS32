#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Arena.h"
#include <string>

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRats);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	std::string takePlayerTurn();
};
#endif
