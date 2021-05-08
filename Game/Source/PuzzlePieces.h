#ifndef __PUZZLE_PIECES_H__
#define __PUZZLE_PIECES_H__

#include "Entity.h"

class PuzzlePieces : public Entity
{
public:
	// Constructor
	PuzzlePieces(int x, int y, PuzzleId id, Entity* player);

	// Called each loop iteration
	bool Update(float dt);

	// Blit
	bool Draw();

	void OnCollision(Collider* c1, Collider* c2);

private:

	PuzzleId puzzleId = PuzzleId::NONE;

	Entity* playerPtr;

	SDL_Rect rock;
	SDL_Rect slidingRock;
	SDL_Rect button;
	SDL_Rect buttonPressed;

public:

	bool isPushed = true;

};

#endif // !__PUZZLE_PIECES_H__
