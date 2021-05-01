#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

class Player : public Entity
{
public:
	enum FlagsAnimation
	{
		DOWN,
		LEFT,
		RIGHT,
		UP,
		WALKING
	};

	// Constructor
	Player(int x, int y, int footstepFx);

	// Called each loop iteration
	bool Update(float dt);

	// Blit
	bool Draw();

	void OnCollision(Collider* c1, Collider* c2);

	bool inDialog;
	bool godMode;

private:
	iPoint playerPos;

	// Animations stuff
	int animFlags;
	float timerFootstep;
	int footstepFx;

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
