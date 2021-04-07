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

	void CalculatePrecision(int& i);

public:

	List<Attack*> AttackPool;
	SString name;

private:

	// Animation
	Animation idle;
	SDL_Rect stressingShadowSec;
	SDL_Rect furiousShadowSec;

};

#endif // !__PLAYER_H__
