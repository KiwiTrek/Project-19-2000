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


Npc::Npc(int x, int y, NpcId npcId, Entity* player) : Entity(x, y, EntityType::NPC)
{
	LOG("Init NPC");
	this->npcId = npcId;
	this->playerPtr = player;

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
		vibin.PushBack({ 54,48,54,36 });

		vibin.speed = 0.5f;
		vibin.loop = true;

		currentAnim = &vibin;

		collider = app->collisions->AddCollider(this->entityRect, Collider::Type::INTERACTABLE, (Module*)app->entities);
		break;
	}
	default:
		break;
	}

	int flags = 1 << FlagsAnimation::DOWN;

}


bool Npc::Update(float dt, NpcId npcId)
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
		app->render->DrawTexture(app->entities->NPCTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame());
		if (app->render->debug)
		{
			app->render->DrawRectangle({ collider->rect.x, collider->rect.y, collider->rect.w, collider->rect.h }, 0, 0, 150, 100);
		}
		break;
	case NpcId::STORE_GUY:
		app->render->DrawTexture(app->entities->NPCTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame());
		if (app->render->debug)
		{
			app->render->DrawRectangle({ collider->rect.x, collider->rect.y, collider->rect.w, collider->rect.h }, 0, 0, 150, 100);
		}
	case NpcId::CAT:
		app->render->DrawTexture(app->entities->NPCTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame());
		if (app->render->debug)
		{
			app->render->DrawRectangle({ collider->rect.x, collider->rect.y, collider->rect.w, collider->rect.h }, 0, 0, 150, 100);
		}
	default:
		break;
	}

	//app->render->DrawTexture(app->entities->playerTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame(), invert);
	//if (app->render->debug)
	//{
	//	app->render->DrawRectangle({ collider->rect.x, collider->rect.y, collider->rect.w, collider->rect.h }, 0, 0, 150, 100);
	//}

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

		default:
			break;
		}
	}
}