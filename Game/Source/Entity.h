#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Collisions.h"
#include "DynArray.h"
#include "List.h"
#include "Log.h"

class Collider;

enum class EntityType
{
	PLAYER,
	COMBAT_ENTITY,
	UNKNOWN
};

enum class EntityId
{
	NOT_COMBAT,
	MC,
	VIOLENT,
	STUBBORN,
	KIND,
	ENEMY_1,
	ENEMY_2,
	ENEMY_3,
	ENEMY_4,
	ENEMY_5
};

//enum EnemyType
//{
//	NO_TYPE,
//	GROUND,
//	FLYING
//};

class Stats
{
public:
	Stats(int pAtk, int mAtk = 0, int pDef = 0, int mDef = 0, int hPoints = 0, int mPoints = 0, float speed = 0.0f, float stress = 0.0f) : pAtk(pAtk), mAtk(mAtk), pDef(pDef), mDef(mDef), hPoints(hPoints), mPoints(mPoints), speed(speed), stress(stress)
	{}

	Stats()
	{}

public:
	int pAtk, mAtk, pDef, mDef, hPoints, mPoints;
	float speed;

	// for MC only
	float stress;
};

enum class AttackType
{
	DAMAGE,
	BUFF,
	HEAL
};

class Attack
{
public:
	Attack(SString name, AttackType type, Stats attackStats) : attackName(name), type(type), attackStats(attackStats)
	{}

	SString attackName;
	AttackType type;
	Stats attackStats;
};

class Entity
{
public:
	// Constructor
	Entity(int x, int y, EntityType type, EntityId id, Stats stats/*, EnemyType eType = EnemyType::NO_TYPE*/) : type(type), id(id), stats(stats)/*, eType(eType)*/
	{}

	Entity(int x, int y, EntityType type) : type(type)
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
	// discern type of entity
	EntityType type;
	EntityId id;

	// basic entity values
	SDL_Rect entityRect;
	Collider* collider;
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

	//Public vars from combat entity
	Stats stats;

	//Public vars from enemy
	//EnemyType eType;
};

#endif // __ENTITY_H__
