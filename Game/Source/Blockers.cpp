#include "Blockers.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "SceneGameplay.h"
#include "PuzzlePieces.h"


Blockers::Blockers(int x, int y, BlockerId id, Entity* player) : Entity(x, y, EntityType::BLOCKER)
{
	LOG("Init Blockers");
	this->blockerId = id;
	this->playerPtr = (Player*)player;

	this->entityRect = { x, y,64,64 };

	spikes = { 512,320,64,64 };
	lockedDoor = { 384,320,64,64 };

	switch (blockerId)
	{
	case BlockerId::SPIKES:
	{
		collider = app->collisions->AddCollider({ x,y,64,576 }, Collider::Type::SOLID_ROCK, (Module*)app->entities);
		break;
	}
	case BlockerId::LOCKED_DOOR:
	{
		collider = app->collisions->AddCollider({ x,y,64,64 }, Collider::Type::SOLID_ROCK, (Module*)app->entities);
		break;
	}
	}

	pendingToDelete = false;

	isBlocking = false;
}


bool Blockers::Update(float dt)
{
	switch (blockerId)
	{
	case BlockerId::SPIKES:
	{
		break;
	}
	case BlockerId::LOCKED_DOOR:
	{
		break;
	}
	}

	return true;
}

bool Blockers::Draw()
{
	switch (blockerId)
	{
	case BlockerId::SPIKES:
		{
		for (int i = 0; i < 9; i++)
		{
			app->render->DrawTexture(app->entities->puzzleTex, entityRect.x, entityRect.y + i * 64, false, &spikes);
		}
			break;
		}
	case BlockerId::LOCKED_DOOR:
	{
		app->render->DrawTexture(app->entities->puzzleTex, entityRect.x, entityRect.y, false, &lockedDoor);
		break;
	}
	}

	if (app->render->debug)
	{
		if (collider != nullptr) app->render->DrawRectangle(collider->rect, 0, 0, 150, 100);
	}

	return true;
}

void Blockers::OnCollision(Collider* c1, Collider* c2)
{
	
}
