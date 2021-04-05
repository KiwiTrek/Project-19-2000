#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Collisions.h"
#include "DynArray.h"
#include "List.h"
#include "Log.h"
#include <time.h>

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
	STRESSING_SHADOW,
	FURIOUS_SHADOW,
	NIGHTMARE,
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
	Stats(int pAtk, int mAtk = 0, int pDef = 0, int mDef = 0, int hPoints = 1, int hPointsMax = 1, int speed = 0, int mPoints = 1, int mPointsMax = 1, int stress = 0, int stressMax = 0) : pAtk(pAtk), mAtk(mAtk), pDef(pDef), mDef(mDef), hPoints(hPoints), hPointsMax(hPointsMax), speed(speed), mPoints(mPoints), mPointsMax(mPointsMax), stress(stress), stressMax(stressMax)
	{}

	Stats()
	{}

public:
	int pAtk, mAtk, pDef, mDef, hPoints, hPointsMax, speed, mPoints, mPointsMax, stress, stressMax; // stress is only for MC
};

enum class AttackType
{
	DAMAGE,
	BUFF,
	HEAL,
	TAUNT
};

class Attack
{
public:
	Attack(SString name, AttackType type, int stat1, int stat2 = 0) : attackName(name), type(type), stat1(stat1), stat2(stat2)
	{}

public:
	SString attackName;
	AttackType type;
	int stat1, stat2;
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
