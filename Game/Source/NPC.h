#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"

class Npc : public Entity
{
public:
	enum FlagsAnimation
	{
		DOWN,
		LEFT,
		RIGHT,
		UP,
	};

	// Constructor
	Npc(int x, int y, NpcId id, Entity* player);

	// Called each loop iteration
	bool Update(float dt);

	// Blit
	bool Draw();

	void OnCollision(Collider* c1, Collider* c2);

	int id;

private:

	NpcId npcId = NpcId::NONE;
	// Animations stuff
	int animFlags;

	// Animation
	Animation idle;
	Animation idleLeft;
	Animation idleRight;
	Animation idleUp;
	Animation vibin;

	Entity* playerPtr;
};

#endif // !__NPC_H__
