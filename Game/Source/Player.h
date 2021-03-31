#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"


class Player : public Entity
{
public:

	// Constructor
	Player(int x, int y);

	// Called each loop iteration
	bool Update(float dt);

	// Blit
	bool Draw();

private:
	bool godMode = false;
	bool inMenu = false;
	iPoint playerPos;

	// Animations stuff
	bool lookingLeft;
	bool lookingRight;
	bool lookingDown;
	bool lookingUp;
	bool walking;

	// Animation
	Animation idle;
	Animation idleLeft;
	Animation idleRight;
	Animation idleUp;
	Animation walkingDown;
	Animation walkingLeft;
	Animation walkingRight;
	Animation walkingUp;
};

#endif // !__PLAYER_H__
