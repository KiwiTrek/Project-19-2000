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


Npc::Npc(int x, int y, NpcId npcId, Entity* player) : Entity(x, y, EntityType::NPC)
{
	LOG("Init NPC");
	this->npcId = npcId;
	this->playerPtr = player;
	this->collision = nullptr;

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

	int flags = 1 << FlagsAnimation::DOWN;

}


bool Npc::Update(float dt)
{
	//switch (npcId)
	//{
	//case NpcId::CAT:
	//{
	//	
	//}
	//default:
	//	break;
	//}
	currentAnim->Update(dt);
	
	return true;
}

bool Npc::Draw()
{
	switch (npcId)
	{
	case NpcId::HERO:
	case NpcId::STORE_GUY:
	case NpcId::CAT:
		app->render->DrawTexture(app->entities->NPCTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame());
		break;
	case NpcId::GRANDPA:
		app->render->DrawTexture(app->entities->grandpaTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame());
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
		switch (npcId)
		{
		case NpcId::HERO:
		{
			if (this->entityRect.x + this->entityRect.w - 5 < playerPtr->entityRect.x)
			{
				this->currentAnim = &idleRight;
			}
			else if (this->entityRect.x + 5 > playerPtr->entityRect.x + playerPtr->entityRect.w)
			{
				this->currentAnim = &idleLeft;
			}

			if (this->entityRect.y + 5 > playerPtr->entityRect.y + playerPtr->entityRect.h)
			{
				this->currentAnim = &idleUp;
			}
			else if (this->entityRect.y + this->entityRect.h - 5 < playerPtr->entityRect.y)
			{
				this->currentAnim = &idle;
			}
			break;
		}
		case NpcId::GRANDPA:
		{
			if (this->entityRect.x + this->entityRect.w - 5 < playerPtr->entityRect.x)
			{
				this->currentAnim = &idleRight;
			}
			else if (this->entityRect.x + 5 > playerPtr->entityRect.x + playerPtr->entityRect.w)
			{
				this->currentAnim = &idleLeft;
			}

			if (this->entityRect.y + 5 > playerPtr->entityRect.y + playerPtr->entityRect.h)
			{
				this->currentAnim = &idleUp;
			}
			else if (this->entityRect.y + this->entityRect.h - 5 < playerPtr->entityRect.y)
			{
				this->currentAnim = &idle;
			}
			break;
		}
			case NpcId::STORE_GUY:
		{
			LOG("Interaction ShopKeeper");
			app->entities->shopkeeperActive = true;
			if (app->entities->dialogCounter == 0)
			{
				app->entities->talkingToShopkeeper = true;
				app->entities->dialogCounter = 1;
			}
			break;
		}

		case NpcId::CAT:
		{
			LOG("Interaction Kitty");
			app->entities->catActive = true;
			if (app->entities->dialogCounter == 0)
			{
				app->entities->talkingToCat = true;
				app->entities->dialogCounter = 1;
			}
			break;
		}
		default:
			break;
		}
	}
}