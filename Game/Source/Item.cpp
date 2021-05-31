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
		a = Attack("HP Potion");
		sec = { 5 * 32,8 * 32,32,32 };
		break;
	case ItemId::MANA_POTION:
		a = Attack("Mana Potion");
		sec = { 8 * 32,8 * 32,32,32 };
		break;
	case ItemId::ELIXIR:
		a = Attack("Elixir");
		sec = { 9 * 32,8 * 32,32,32 };
		break;
	case ItemId::GRANDMA_STEW:
		a = Attack("Grandma's Stew");
		sec = { 8 * 32,7 * 32,32,32 };
		break;
	case ItemId::BOTTLED_SMITE:
		a = Attack("Bottled Smite");
		sec = { 6 * 32,8 * 32,32,32 };
		break;
	case ItemId::HAPPILLS:
		a = Attack("Happills");
		sec = { 4 * 32,8 * 32,32,32 };
		break;
	case ItemId::PHYS_BUFFER:
		a = Attack("Phys Buffer");
		sec = { 3 * 32,7 * 32,32,32 };
		break;
	case ItemId::MAGIC_BUFFER:
		a = Attack("Magic Buffer");
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
		app->audio->PlayFx(app->entities->itemCollectedFx);
		LOG("You picked up %s item.", item.effect.attackName);

		app->entities->takingItem = true;
		char start[TEXT_LEN] = { 0 };
		strcpy_s(start,TEXT_LEN, "You picked up ");
		strcat_s(start, TEXT_LEN, item.effect.attackName);
		strcpy_s(app->entities->itemPasser, TEXT_LEN, start);

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
{
    this->button = nullptr;
    this->count = 0;
    this->id = ItemId::NONE;
}

void Item::Use(CombatEntity* target)
{
	LOG("ITEM USE %s", effect.attackName);
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
	case ItemId::BOTTLED_SMITE:
		target->stats.hPoints -= 30;
		if (target->stats.hPoints <= 0) target->stats.hPoints = 0;
		break;
	case ItemId::HAPPILLS:
		target->stats.stress -= 30;
		if (target->stats.stress <= 0) target->stats.stress = 0;
		break;
	case ItemId::PHYS_BUFFER:
	{
		Attack* a = new Attack("5 buff");
		a->turns = 2;
		target->attackPool.Add(a);
		target->stats.pAtk += (target->stats.pAtk * 5) / 100;
		target->stats.pDef += (target->stats.pDef * 5) / 100;
		break;
	}
	case ItemId::MAGIC_BUFFER:
	{
		Attack* a = new Attack("5 buff");
		a->turns = 2;
		target->attackPool.Add(a);
		target->stats.mAtk += (target->stats.mAtk * 5) / 100;
		target->stats.mDef += (target->stats.mDef * 5) / 100;
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
		memset(s->combatScene->mainChar.hp, 0, TEXT_LEN);
		sprintf_s(s->combatScene->mainChar.hp, TEXT_LEN, "HP: %d/%d", s->combatScene->mainChar.character->stats.hPoints, s->combatScene->mainChar.character->stats.hPointsMax);
		memset(s->combatScene->mainChar.mp, 0, TEXT_LEN);
		sprintf_s(s->combatScene->mainChar.mp, TEXT_LEN, "MP: %d/%d", s->combatScene->mainChar.character->stats.mPoints, s->combatScene->mainChar.character->stats.mPointsMax);
		memset(s->combatScene->mainChar.stress, 0, TEXT_LEN);
		sprintf_s(s->combatScene->mainChar.stress, TEXT_LEN, "ST: %d/%d", s->combatScene->mainChar.character->stats.stress, s->combatScene->mainChar.character->stats.stressMax);
		memset(s->combatScene->grandpa.hp, 0, TEXT_LEN);
		sprintf_s(s->combatScene->grandpa.hp, TEXT_LEN, "HP: %d/%d", s->combatScene->grandpa.character->stats.hPoints, s->combatScene->grandpa.character->stats.hPointsMax);
		memset(s->combatScene->grandpa.mp, 0, TEXT_LEN);
		sprintf_s(s->combatScene->grandpa.mp, TEXT_LEN, "MP: %d/%d", s->combatScene->grandpa.character->stats.mPoints, s->combatScene->grandpa.character->stats.mPointsMax);
		//Should add the rest of the characters
	}
	app->audio->PlayFx(app->entities->itemFx);
}

void Item::UpdateCountText()
{
	memset(countText, 0, TEXT_LEN);
	sprintf_s(countText,TEXT_LEN, "x%d", count);
}