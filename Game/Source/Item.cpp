#include "Item.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "SceneGameplay.h"
#include "SceneCombat.h"


ItemEntity::ItemEntity(int x, int y, ItemId id, int count) : Entity(x, y, EntityType::ITEM)
{
	LOG("Init Item");
	this->entityRect = { x + 16, y + 16,32,32 };
	collider = app->collisions->AddCollider(entityRect, Collider::Type::INTERACTABLE, (Module*)app->entities);

	pendingToDelete = false;

	Attack a;
	SDL_Rect sec = { 0,0,0,0 };
	switch (id)
	{
	case ItemId::HP_POTION:
		a = Attack("HP Potion", AttackType::HEAL, TargetType::ONE);
		sec = { 5 * 32,8 * 32,32,32 };
		break;
	case ItemId::MANA_POTION:
		a = Attack("Mana Potion", AttackType::BUFF, TargetType::ONE);
		sec = { 8 * 32,8 * 32,32,32 };
		break;
	case ItemId::ELIXIR:
		a = Attack("Elixir", AttackType::BUFF, TargetType::ONE);
		sec = { 9 * 32,8 * 32,32,32 };
		break;
	case ItemId::GRANDMA_STEW:
		a = Attack("Grandma's Stew", AttackType::BUFF, TargetType::ONE);
		sec = { 8 * 32,7 * 32,32,32 };
		break;
	case ItemId::BOTTLEED_SMITE:
		a = Attack("Smite In A Bottle", AttackType::DAMAGE, TargetType::ONE);
		sec = { 6 * 32,8 * 32,32,32 };
		break;
	case ItemId::HAPPILLS:
		a = Attack("Happills", AttackType::BUFF, TargetType::ONE);
		sec = { 4 * 32,8 * 32,32,32 };
		break;
	case ItemId::PHYS_BUFFER:
		a = Attack("Phys Buffer", AttackType::BUFF, TargetType::ONE);
		sec = { 3 * 32,7 * 32,32,32 };
		break;
	case ItemId::MAGIC_BUFFER:
		a = Attack("Magic Buffer", AttackType::BUFF, TargetType::ONE);
		sec = { 7 * 32,8 * 32,32,32 };
		break;
	default:
		break;
	}

	item = Item(id, a, count);
	item.UpdateCountText();
	item.texSec = sec;
}


bool ItemEntity::Update(float dt)
{
	return true;
}

bool ItemEntity::Draw()
{
	app->render->DrawTexture(app->entities->itemAtlas, entityRect.x, entityRect.y, false, &item.texSec);

	if (app->render->debug && collider != nullptr) app->render->DrawRectangle(collider->rect, 0, 0, 150, 100);

	return true;
}

void ItemEntity::OnCollision(Collider* c1, Collider* c2)
{
	if (app->input->CheckButton("select", KeyState::KEY_DOWN) && app->scene->current->currentScene == SceneType::GAMEPLAY)
	{
        app->audio->PlayFx(app->scene->itemCollectedFx);
		LOG("You picked up %s item.", item.effect.attackName.GetString());

		bool added = false;
		SceneGameplay* gameplay = (SceneGameplay*)app->scene->current;
		for (int i = 0; i < gameplay->combatScene->items.Count(); i++)
		{
			if (item.id == gameplay->combatScene->items.At(i)->data->id)
			{
				LOG("OLD ITEM");
				gameplay->combatScene->items.At(i)->data->count += item.count;
				gameplay->combatScene->items.At(i)->data->UpdateCountText();
				added = true;
			}
		}
		if (!added)
		{
			LOG("NEW ITEM");
			gameplay->combatScene->items.Add(&item);
		}

		pendingToDelete = true;
	}
}

Item::Item(ItemId id, Attack effect, int count) : id(id), effect(effect), count(count)
{
	UpdateCountText();
}

Item::Item()
{}

void Item::Use(CombatEntity* target)
{
	LOG("ITEM USE %s", effect.attackName.GetString());
	switch (id)
	{
	case ItemId::HP_POTION:
		target->stats.hPoints += 30;
		if (target->stats.hPoints >= target->stats.hPointsMax) target->stats.hPoints = target->stats.hPointsMax;
		break;
	case ItemId::MANA_POTION:
		target->stats.mPoints += 30;
		if (target->stats.mPoints >= target->stats.mPointsMax) target->stats.mPoints = target->stats.mPointsMax;
		break;
	case ItemId::ELIXIR:
		target->stats.hPoints += 25;
		if (target->stats.hPoints >= target->stats.hPointsMax) target->stats.hPoints = target->stats.hPointsMax;
		target->stats.mPoints += 25;
		if (target->stats.mPoints >= target->stats.mPointsMax) target->stats.mPoints = target->stats.mPointsMax;
		break;
	case ItemId::GRANDMA_STEW:
		// Clears status poison
		break;
	case ItemId::BOTTLEED_SMITE:
		target->stats.hPoints -= 30;
		if (target->stats.hPoints <= 0) target->stats.hPoints = 0;
		break;
	case ItemId::HAPPILLS:
		target->stats.stress -= 30;
		if (target->stats.stress <= 0) target->stats.stress = 0;
		break;
	case ItemId::PHYS_BUFFER:
	{
		SString s = "5 buff";
		Attack* a = new Attack(s, AttackType::BUFF, TargetType::SELF, target->stats.pAtk, target->stats.pDef);
		a->turns = 2;
		target->attackPool.Add(a);
		target->stats.pAtk += (a->stat1 * 5) / 100;
		target->stats.pDef += (a->stat2 * 5) / 100;
		break;
	}
	case ItemId::MAGIC_BUFFER:
	{
		SString s = "5 buff";
		Attack* a = new Attack(s, AttackType::BUFF, TargetType::SELF, target->stats.mAtk, target->stats.mDef);
		a->turns = 2;
		target->attackPool.Add(a);
		target->stats.mAtk += (a->stat1 * 5) / 100;
		target->stats.mDef += (a->stat2 * 5) / 100;
		break;
	}
	default:
		break;
	}
	count--;
	UpdateCountText();
	if (app->scene->current->currentScene == SceneType::GAMEPLAY)
	{
		SceneGameplay* s = (SceneGameplay*)app->scene->current;
		s->combatScene->mainChar.hp.Create("HP: %d/%d", s->combatScene->mainChar.character->stats.hPoints, s->combatScene->mainChar.character->stats.hPointsMax);
		s->combatScene->grandpa.hp.Create("HP: %d/%d", s->combatScene->grandpa.character->stats.hPoints, s->combatScene->grandpa.character->stats.hPointsMax);
		//Should add the rest of the characters
	}
    app->audio->PlayFx(app->scene->itemFx);
}

void Item::UpdateCountText()
{
	countText.Clear();
	countText.Create("x%d", count);
}