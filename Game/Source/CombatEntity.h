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
	char name[TEXT_LEN] = { 0 };
	int isStunned;
	int isTaunted;
	CombatEntity* tauntedBy;

private:

	// Animation
	Animation idle;
    SDL_Rect furiousShadowSec = { 0,0,0,0 };
    SDL_Rect nightmareSec = { 0,0,0,0 };
    SDL_Rect stressingShadowSec = { 0,0,0,0 };

};

#endif // !__PLAYER_H__
