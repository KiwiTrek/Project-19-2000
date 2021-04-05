#include "CombatEntity.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"
#include "Animation.h"

CombatEntity::CombatEntity(int x, int y, EntityId id, Stats stats) : Entity(x, y, EntityType::COMBAT_ENTITY, id, stats)
{
	this->entityRect = { x,y,128,128 };

	pendingToDelete = false;

	// Animation

	//idle.PushBack({ 0,0,38,48 });

	//currentAnim = &idle;

	// depending on the id each combat entity will have a diferent pool of attacks/skills and different name
	switch (id)
	{
	case EntityId::MC:
		name = "MC";
		AttackPool.Add(&Attack("Strike", AttackType::DAMAGE, stats.pAtk));
		break;
	case EntityId::VIOLENT:
		name = "Grandpa";
		AttackPool.Add(&Attack("Smite foes", AttackType::DAMAGE, stats.pAtk));
		break;
	case EntityId::STUBBORN:
		break;
	case EntityId::KIND:
		break;
	case EntityId::STRESSING_SHADOW:
	{
		name = "Stressing Shadow";																		// example of a name
		AttackPool.Add(&Attack("Magical blow", AttackType::DAMAGE,stats.mAtk));							// example of an attack
		AttackPool.Add(&Attack("Stressing attack", AttackType::BUFF, 10));
		break;
	}
	case EntityId::FURIOUS_SHADOW:
		name = "Furious Shadow";
		AttackPool.Add(&Attack("Getting stronger", AttackType::BUFF, stats.pDef, stats.mDef));
		AttackPool.Add(&Attack("Fury of blades", AttackType::DAMAGE, stats.pAtk));
		break;
	case EntityId::NIGHTMARE:
		name = "Nightmare";
		AttackPool.Add(&Attack("Bad dream", AttackType::DAMAGE, stats.pAtk));
		AttackPool.Add(&Attack("Nightmarish", AttackType::BUFF, stats.pDef, stats.mDef));
		AttackPool.Add(&Attack("Close your eyes", AttackType::TAUNT, 0));
		AttackPool.Add(&Attack("Grasp of depression", AttackType::BUFF, 0));
		break;
	case EntityId::ENEMY_4:
		break;
	case EntityId::ENEMY_5:
		break;
	default:
		break;
	}

	LOG("Init Combat Entity: %s", name.GetString());
}

bool CombatEntity::Update(float dt)
{
	return true;
}

bool CombatEntity::Draw()
{
	if (app->render->debug) app->render->DrawRectangle(entityRect, 255, 0, 0, 255);
	//app->render->DrawTexture(app->entities->playerTex, playerPos.x, playerPos.y, false, &currentAnim->GetCurrentFrame(), invert);

	return true;
}

void CombatEntity::CalculatePrecision(int& i)
{
	srand(time(NULL));
	int p = rand() % 20 - 9;
	i += (i * p);
}