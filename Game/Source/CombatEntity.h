#ifndef __COMBAT_ENTITY_H__
#define __COMBAT_ENTITY_H__

#include "Entity.h"

class CombatEntity : public Entity
{
public:

	// Constructor
	CombatEntity(int x, int y, EntityId id, Stats stats);

	// Called each loop iteration
	bool Update(float dt);

	// Blit
	bool Draw();

private:

	// Animation
	Animation idle;

	List<Attack*> AttackPool;
};

#endif // !__PLAYER_H__
