#ifndef __PUZZLE_PIECES_H__
#define __PUZZLE_PIECES_H__

#include "Entity.h"
#include "Player.h"

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

	void CorrectPosition();

	bool CheckUp();
	bool CheckDown();
	bool CheckLeft();
	bool CheckRight();

private:

	PuzzleId puzzleId = PuzzleId::NONE;

	Player* playerPtr;

	SDL_Rect rock;
	SDL_Rect slidingRock;
	SDL_Rect button;
	SDL_Rect buttonPressed;
	SDL_Rect doorUp;

public:

	bool isPressed;
	bool isPushedUp;	
	bool isPushedDown;
	bool isPushedLeft;
	bool isPushedRight;
	int value = 0;
	float timerSlideFx;

	Collider* collision;

	iPoint prevPos;
};

#endif // !__PUZZLE_PIECES_H__
