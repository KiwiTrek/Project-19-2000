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

	// initialize specific combat entity id variables
	switch (id)
	{
	case EntityId::MC:
	{
		name = "MC";

		Attack* a = new Attack("Smack");
		this->attackPool.Add(a);

		a = new Attack("Comfort");
		this->attackPool.Add(a);

		a = new Attack("Slap");
		this->attackPool.Add(a);

		a = new Attack("Encourage");
		this->attackPool.Add(a);

		a = new Attack("Boring Speech");
		this->attackPool.Add(a);
		break;
	}
	case EntityId::VIOLENT:
	{
		name = "Grandpa";

		Attack* a = new Attack("Smite foes");
		this->attackPool.Add(a);

		a = new Attack("Magic missile");
		this->attackPool.Add(a);

		a = new Attack("Magic Barrage");
		this->attackPool.Add(a);

		a = new Attack("Magic Hand Slap");
		this->attackPool.Add(a);

		a = new Attack("Judgemental Stare");
		this->attackPool.Add(a);
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

		Attack* a = new Attack("Magical blow");
		this->attackPool.Add(a);

		a = new Attack("Stressing attack");
		this->attackPool.Add(a);
		break;
	}
	case EntityId::FURIOUS_SHADOW:
	{
		furiousShadowSec = { 0,0,128,128 };
		name = "Furious Shadow";

		Attack* a = new Attack("Getting stronger");
		this->attackPool.Add(a);

		a = new Attack("Fury of blades");
		this->attackPool.Add(a);
		break;
	}
	case EntityId::NIGHTMARE:
	{
		nightmareSec = { 256,0,128,128 };
		name = "Nightmare";

		Attack* a = new Attack("Bad dream");
		this->attackPool.Add(a);

		a = new Attack("Nightmarish");
		this->attackPool.Add(a);

		a = new Attack("Close your eyes");
		this->attackPool.Add(a);

		a = new Attack("Grasp of depression");
		this->attackPool.Add(a);
		break;
	}
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
		default:
			break;
		}
	}

	return true;
}

void CombatEntity::CalculatePrecision(int& i)
{
	int p = rand() % 20 - 9;
	i += (i * p) / 100;
}