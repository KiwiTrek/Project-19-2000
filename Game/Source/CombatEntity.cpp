#include "CombatEntity.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"
#include "Animation.h"
#include "SceneManager.h"

CombatEntity::CombatEntity(int x, int y, EntityId id, Stats stats) : Entity(x, y, EntityType::COMBAT_ENTITY, id, stats)
{
	this->entityRect = { x,y,128,128 };
	this->collider = app->collisions->AddCollider(entityRect, Collider::Type::SOLID, app->entities);

	pendingToDelete = false;
	isStunned = false;
	isTaunted = false;
	tauntedBy = nullptr;

	// Animation

	//idle.PushBack({ 0,0,38,48 });

	//currentAnim = &idle;

	// initialize specific combat entity id variables
	switch (id)
	{
	case EntityId::MC:
	{
		name = "MC";

		SString tmpS = "Smack";
		Attack* tmpA = new Attack(tmpS, AttackType::DAMAGE, TargetType::ONE, stats.pAtk);
		this->attackPool.Add(tmpA);

		tmpS = "Comfort";
		tmpA = new Attack(tmpS, AttackType::HEAL, TargetType::ONE, 0);
		this->attackPool.Add(tmpA);

		tmpS = "Slap";
		tmpA = new Attack(tmpS, AttackType::DAMAGE, TargetType::ONE, stats.pAtk);
		this->attackPool.Add(tmpA);

		tmpS = "Encourage";
		tmpA = new Attack(tmpS, AttackType::BUFF, TargetType::ONE);
		this->attackPool.Add(tmpA);

		tmpS = "Boring Speech";
		tmpA = new Attack(tmpS, AttackType::BUFF, TargetType::ONE);
		this->attackPool.Add(tmpA);

		break;
	}
	case EntityId::VIOLENT:
	{
		name = "Grandpa";

		SString tmpS = "Smite foes";
		Attack* tmpA = new Attack(tmpS, AttackType::DAMAGE, TargetType::ONE, stats.mAtk);
		this->attackPool.Add(tmpA);

		tmpS = "Weak Magic missile";
		tmpA = new Attack(tmpS, AttackType::DAMAGE, TargetType::ONE, stats.mAtk / 1.3f);
		this->attackPool.Add(tmpA);

		tmpS = "Weak Magic Barrage";
		tmpA = new Attack(tmpS, AttackType::DAMAGE, TargetType::ALL_ENEMIES, stats.mAtk / 1.4f);
		this->attackPool.Add(tmpA);

		tmpS = "Magic Hand Slap";
		tmpA = new Attack(tmpS, AttackType::DAMAGE, TargetType::ONE, stats.mAtk / 1.1f);
		this->attackPool.Add(tmpA);

		tmpS = "Judgemental Stare";
		tmpA = new Attack(tmpS, AttackType::DAMAGE, TargetType::ONE, stats.mAtk * 1.5f);
		this->attackPool.Add(tmpA);

		break;
	}
	case EntityId::STUBBORN:
		break;
	case EntityId::KIND:
		break;
	case EntityId::STRESSING_SHADOW:
	{
		stressingShadowSec = { 128,0,128,128 };
		name = "Stressing Shadow";

		SString tmpS = "Magical blow";
		Attack* tmpA = new Attack(tmpS, AttackType::DAMAGE, TargetType::ONE, stats.mAtk);
		this->attackPool.Add(tmpA);

		tmpS = "Stressing attack";
		tmpA = new Attack(tmpS, AttackType::BUFF, TargetType::ONE, 10);
		this->attackPool.Add(tmpA);
		break;
	}
	case EntityId::FURIOUS_SHADOW:
	{
		furiousShadowSec = { 0,0,128,128 };
		name = "Furious Shadow";

		SString tmpS = "Getting stronger";
		Attack* tmpA = new Attack(tmpS, AttackType::BUFF, TargetType::SELF, stats.pDef, stats.mDef);
		this->attackPool.Add(tmpA);

		tmpS = "Fury of blades";
		tmpA = new Attack(tmpS, AttackType::DAMAGE, TargetType::ALL_ALLIES, stats.pAtk);
		this->attackPool.Add(tmpA);
		break;
	}
	case EntityId::NIGHTMARE:
	{
		nightmareSec = { 256,0,128,128 };
		name = "Nightmare";

		SString tmpS = "Bad dream";
		Attack* tmpA = new Attack(tmpS, AttackType::DAMAGE, TargetType::ONE, stats.pAtk);
		this->attackPool.Add(tmpA);

		tmpS = "Nightmarish";
		tmpA = new Attack(tmpS, AttackType::BUFF, TargetType::SELF, stats.pDef, stats.mDef);
		this->attackPool.Add(tmpA);

		tmpS = "Close your eyes";
		tmpA = new Attack(tmpS, AttackType::TAUNT, TargetType::SELF);
		this->attackPool.Add(tmpA);

		tmpS = "Grasp of depression";
		tmpA = new Attack(tmpS, AttackType::BUFF, TargetType::ONE);
		this->attackPool.Add(tmpA);
		break;
	}
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
	if (app->scene->current->combat)
	{
		if (app->render->debug) app->render->DrawRectangle(entityRect, 255, 0, 0, 255);

		switch (id)
		{
		case EntityId::STRESSING_SHADOW:
			app->render->DrawTexture(app->entities->enemiesTex, -app->render->camera.x + entityRect.x, -app->render->camera.y + entityRect.y, false, &stressingShadowSec);
			break;
		case EntityId::FURIOUS_SHADOW:
			app->render->DrawTexture(app->entities->enemiesTex, -app->render->camera.x + entityRect.x, -app->render->camera.y + entityRect.y, false, &furiousShadowSec);
			break;
		case EntityId::NIGHTMARE:
			app->render->DrawTexture(app->entities->enemiesTex, -app->render->camera.x + entityRect.x, -app->render->camera.y + entityRect.y, false, &nightmareSec);
			break;
		case EntityId::ENEMY_4:
			break;
		case EntityId::ENEMY_5:
			break;
		default:
			break;
		}
	}

	return true;
}

void CombatEntity::CalculatePrecision(int& i)
{
	srand(time(NULL));
	int p = rand() % 20 - 9;
	i += (i * p)/100;
}