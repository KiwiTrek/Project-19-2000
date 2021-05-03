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
	this->itemId = id;
	this->entityRect = { x, y,32,32 };
	collider = app->collisions->AddCollider(entityRect, Collider::Type::INTERACTABLE, (Module*)app->entities);

	pendingToDelete = false;

	Attack a;
	SDL_Rect sec;
	switch (itemId)
	{
	case ItemId::HP_POTION:
		a = Attack("HP Potion", AttackType::HEAL, TargetType::ONE, 0);
		sec = { 5 * 32,8 * 32,32,32 };
		break;
	case ItemId::MANA_POTION:
		a = Attack("Mana Potion", AttackType::BUFF, TargetType::ONE, 0);
		sec = { 8 * 32,8 * 32,32,32 };
		break;
	case ItemId::ELIXIR:
		a = Attack("Elixir", AttackType::BUFF, TargetType::ONE, 0);
		sec = { 9 * 32,8 * 32,32,32 };
		break;
	case ItemId::GRANDMA_STEW:
		a = Attack("Grandma's Stew", AttackType::BUFF, TargetType::ONE, 0);
		sec = { 8 * 32,7 * 32,32,32 };
		break;
	case ItemId::BOTTLEED_SMITE:
		a = Attack("Smite In A Bottle", AttackType::DAMAGE, TargetType::ONE, 30);
		sec = { 6 * 32,8 * 32,32,32 };
		break;
	case ItemId::HAPPILLS:
		a = Attack("Happills", AttackType::BUFF, TargetType::ONE, 0);
		sec = { 4 * 32,8 * 32,32,32 };
		break;
	case ItemId::STAT_BUFFER:
		a = Attack("Stat Buffer", AttackType::BUFF, TargetType::ONE, 0);
		sec = { 7 * 32,8 * 32,32,32 };
		break;
	default:
		break;
	}

	item = Item(a, count);
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
		LOG("You picked up %s item.", item.effect.attackName.GetString());

		bool added = false;
		SceneGameplay* gameplay = (SceneGameplay*)app->scene->current;
		for (int i = 0; i < gameplay->combatScene->items.Count(); i++)
		{
			if (item.effect.attackName == gameplay->combatScene->items.At(i)->data->effect.attackName)
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