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

	iPoint playerPos;
	bool godMode = false;

	// Animation
	Animation idle;
};

#endif // !__PLAYER_H__
