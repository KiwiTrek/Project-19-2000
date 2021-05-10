#include "NPC.h"

#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Map.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Animation.h"
#include "Collisions.h"
#include "Player.h"
#include "DialogSystem.h"


Npc::Npc(int x, int y, NpcId npcId, Entity* player, int interactFx) : Entity(x, y, EntityType::NPC)
{
	LOG("Init NPC");
	this->npcId = npcId;
	this->playerPtr = player;
	this->collision = nullptr;
	this->animFlags = 0;
	this->interactFx = interactFx;

	this->entityRect = { x, y,34,46 };

	pendingToDelete = false;

	switch (npcId)
	{
	case NpcId::HERO:
	{
		this->entityRect = { x, y,34,46 };

		idle.PushBack({ 0,0,34,46 });
		idleLeft.PushBack({ 34,0,34,46 });
		idleRight.PushBack({ 102,0,34,46 });
		idleUp.PushBack({ 68,0,34,46 });

		currentAnim = &idle;

		collider = app->collisions->AddCollider({ x - 5,y - 5,44,56 }, Collider::Type::INTERACTABLE, (Module*)app->entities);
		collision = app->collisions->AddCollider({ x,y,34,23 }, Collider::Type::SOLID, (Module*)app->entities);
		break;
	}
	case NpcId::STORE_GUY:
	{
		this->entityRect = { x, y,34,46 };

		idle.PushBack({ 136,0,34,46 });

		collider = app->collisions->AddCollider({ x - 5,y - 5,44,124 }, Collider::Type::INTERACTABLE, (Module*)app->entities);

		currentAnim = &idle;
		break;
	}
	case NpcId::CAT:
	{
		this->entityRect = { x, y,54,36 };

		vibin.PushBack({ 0,48,54,36 });
		vibin.PushBack({ 56,48,54,36 });

		vibin.speed = 1.5f;
		vibin.loop = true;

		currentAnim = &vibin;

		collider = app->collisions->AddCollider({ x - 20,y - 5,82,40 }, Collider::Type::INTERACTABLE, (Module*)app->entities);
		break;
	}
	case NpcId::GRANDPA:
	{
		this->entityRect = { x,y,34,54 };

		idle.PushBack({ 2,2,34,54 });
		idle.PushBack({ 38,2,34,54 });
		idle.PushBack({ 74,2,34,54 });
		idle.PushBack({ 110,2,34,54 });

		idle.speed = 1.4f;
		idle.loop = true;

		idleLeft.PushBack({ 2,58,34,54 });
		idleLeft.PushBack({ 38,58,34,54 });
		idleLeft.PushBack({ 74,58,34,54 });
		idleLeft.PushBack({ 110,58,34,54 });

		idleLeft.speed = 1.4f;
		idleLeft.loop = true;

		idleRight.PushBack({ 2,114,34,54 });
		idleRight.PushBack({ 38,114,34,54 });
		idleRight.PushBack({ 74,114,34,54 });
		idleRight.PushBack({ 110,114,34,54 });

		idleRight.speed = 1.4f;
		idleRight.loop = true;

		idleUp.PushBack({ 2,170,34,54 });
		idleUp.PushBack({ 38,170,34,54 });
		idleUp.PushBack({ 74,170,34,54 });
		idleUp.PushBack({ 110,170,34,54 });

		idleUp.speed = 1.4f;
		idleUp.loop = true;

		collider = app->collisions->AddCollider({ x - 2,y - 2,36,56 }, Collider::Type::INTERACTABLE, (Module*)app->entities);
		collision = app->collisions->AddCollider({ x + 2,y + 2,30,27 }, Collider::Type::SOLID, (Module*)app->entities);

		currentAnim = &idle;
	}
	default:
		break;
	}
}


bool Npc::Update(float dt)
{
	currentAnim->Update(dt);
	
	return true;
}

bool Npc::Draw()
{
	switch (npcId)
	{
	case NpcId::HERO:
		if ((app->entities->flagsGrandpa & 1 << (int)DialogueFlags::ACTIVE) == 0)
		{
			app->render->DrawTexture(app->entities->NPCTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame());
		}
		break;
	case NpcId::STORE_GUY:
		app->render->DrawTexture(app->entities->NPCTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame());
		break;
	case NpcId::CAT:
		if ((app->entities->flagsShopkeeper & 1 << (int)DialogueFlags::ACTIVE) == 0)
		{
		app->render->DrawTexture(app->entities->NPCTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame());
		}
		break;
	case NpcId::GRANDPA:
		app->render->DrawTexture(app->entities->grandpaTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame());
		break;
	default:
		break;
	}

	if (app->render->debug)
	{
		if (collider != nullptr) app->render->DrawRectangle(collider->rect, 0, 0, 150, 100);
		if (collision != nullptr) app->render->DrawRectangle(collision->rect, 150, 0, 0, 100);
	}

	return true;
}

void Npc::OnCollision(Collider* c1, Collider* c2)
{
	if (app->input->CheckButton("select", KeyState::KEY_DOWN))
	{
		LOG("Interaction");
		if (app->entities->dialogCounter == 0.0f) app->audio->PlayFx(interactFx);
		switch (npcId)
		{
		case NpcId::HERO:
		{
			if (this->collider->rect.y + this->collider->rect.h > playerPtr->entityRect.y && this->collision->rect.y < playerPtr->entityRect.y)
			{
				this->currentAnim = &idle;
			}
			if (this->collider->rect.y < playerPtr->entityRect.y + playerPtr->entityRect.h && this->collision->rect.y > playerPtr->entityRect.y)
			{
				this->currentAnim = &idleUp;
			}
			else if (this->collision->rect.y + this->collision->rect.h > playerPtr->entityRect.y)
			{
				if (this->collision->rect.x < playerPtr->entityRect.x)
				{
					this->currentAnim = &idleRight;
				}
				else if (this->collision->rect.x > playerPtr->entityRect.x)
				{
					this->currentAnim = &idleLeft;
				}
			}
			LOG("Interaction super hero");
			app->entities->flagsSuperhero = SetBit(app->entities->flagsSuperhero, DialogueFlags::ACTIVE);
			if (app->entities->dialogCounter == 0.0f)
			{
				app->entities->flagsSuperhero = SetBit(app->entities->flagsSuperhero, DialogueFlags::TALKING_TO);
				app->entities->dialogCounter = 0.5f;
			}
			break;
		}
		case NpcId::GRANDPA:
		{
			if (this->collider->rect.y + this->collider->rect.h > playerPtr->entityRect.y && this->collision->rect.y < playerPtr->entityRect.y)
			{
				this->currentAnim = &idle;
			}
			if (this->collider->rect.y < playerPtr->entityRect.y + playerPtr->entityRect.h && this->collision->rect.y > playerPtr->entityRect.y)
			{
				this->currentAnim = &idleUp;
			}
			else if (this->collision->rect.y + this->collision->rect.h > playerPtr->entityRect.y)
			{
				if (this->collision->rect.x < playerPtr->entityRect.x)
				{
					this->currentAnim = &idleRight;
				}
				else if (this->collision->rect.x > playerPtr->entityRect.x)
				{
					this->currentAnim = &idleLeft;
				}
			}

			LOG("Interaction Grandpa");
			app->entities->flagsGrandpa = SetBit(app->entities->flagsGrandpa, DialogueFlags::ACTIVE);
			if (app->entities->dialogCounter == 0.0f)
			{
				app->entities->flagsGrandpa = SetBit(app->entities->flagsGrandpa, DialogueFlags::TALKING_TO);
				app->entities->dialogCounter = 0.5f;
			}
			break;
		}
			case NpcId::STORE_GUY:
		{
			LOG("Interaction ShopKeeper");
			app->entities->flagsShopkeeper = SetBit(app->entities->flagsShopkeeper, DialogueFlags::ACTIVE);
			if (app->entities->dialogCounter == 0.0f)
			{
				app->entities->flagsShopkeeper = SetBit(app->entities->flagsShopkeeper, DialogueFlags::TALKING_TO);
				app->entities->dialogCounter = 0.5f;
			}
			break;
		}

		case NpcId::CAT:
		{
			LOG("Interaction Kitty");
			app->entities->flagsCat = SetBit(app->entities->flagsCat, DialogueFlags::ACTIVE);
			if (app->entities->dialogCounter == 0.0f)
			{
				app->entities->flagsCat = SetBit(app->entities->flagsCat, DialogueFlags::TALKING_TO);
				app->entities->dialogCounter = 0.5f;
			}
			break;
		}
		default:
			break;
		}
	}
}