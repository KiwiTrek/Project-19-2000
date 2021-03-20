#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Physics.h"
#include "DynArray.h"

class Collider;

enum class EntityType
{
	PLAYER,
	UNKNOWN
};

enum EnemyType
{
	NO_TYPE,
	GROUND,
	FLYING
};

class Entity
{
public:
	// Constructor
	Entity(int x, int y, EntityType type, EnemyType eType = EnemyType::NO_TYPE) : type(type), eType(eType)
	{}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Blit
	virtual bool Draw()
	{
		return true;
	}

	// Collision response
	virtual void OnCollision(Collider* c1, Collider* c2)
	{}

public:
	EntityType type;
	SDL_Rect entityRect;
	Collider* collider;
	Physics physics;
	iPoint nextPos;
	uint destroyedFx;

	bool pendingToDelete = false;

	// Original spawn position
	iPoint spawnPos;

	// Animation related variables
	Animation* currentAnim = nullptr;
	bool invert = false;

	//Public vars from player
	bool isDead = false;

	//Public vars from enemy
	EnemyType eType;
};

#endif // __ENTITY_H__
