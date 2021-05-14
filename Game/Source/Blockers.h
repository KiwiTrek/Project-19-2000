#ifndef __BLOCKERS_H__
#define __BLOCKERS_H__

#include "Entity.h"
#include "Player.h"

class Blockers : public Entity
{
public:
	// Constructor
	Blockers(int x, int y, BlockerId id, Entity* player);

	// Called each loop iteration
	bool Update(float dt);

	// Blit
	bool Draw();

	void OnCollision(Collider* c1, Collider* c2);

private:

	BlockerId blockerId = BlockerId::NONE;

	Player* playerPtr;

	SDL_Rect spikes;
	SDL_Rect lockedDoor;

	bool isBlocking;
};

#endif // !__BLOCKERS_H__