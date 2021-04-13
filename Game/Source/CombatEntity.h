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

	List<Attack*> attackPool;
	SString name;
	bool isStunned;
	bool isTaunted;
	CombatEntity* tauntedBy;

private:

	// Animation
	Animation idle;
	SDL_Rect stressingShadowSec;
	SDL_Rect furiousShadowSec;
	SDL_Rect nightmareSec;

};

#endif // !__PLAYER_H__
