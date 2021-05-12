#include "PuzzlePieces.h"

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


PuzzlePieces::PuzzlePieces(int x, int y, PuzzleId id, Entity* player) : Entity(x, y, EntityType::PUZZLE_PIECE)
{
	LOG("Init PuzzlePieces");
	this->puzzleId = id;
	this->playerPtr = player;

	this->entityRect = { x, y,34,46 };

	rock = { 0,256,64,64 };
	slidingRock = { 0,384,64,64 };
	button = { 896,0,64,64 };
	buttonPressed = { 960,0,64,64 };

	pendingToDelete = false;

	switch (puzzleId)
	{
	case PuzzleId::ROCK:
	{
		collider = app->collisions->AddCollider({ x,y,64,64 }, Collider::Type::PUZZLE, (Module*)app->entities);
		break;
	}
	case PuzzleId::SLIDING_ROCK:
	{
		collider = app->collisions->AddCollider({ x,y,64,64 }, Collider::Type::PUZZLE, (Module*)app->entities);
		break;
	}
	}

}


bool PuzzlePieces::Update(float dt)
{
	
	return true;
}

bool PuzzlePieces::Draw()
{
	switch (puzzleId)
	{
	case PuzzleId::ROCK:
		app->render->DrawTexture(app->entities->puzzleTex, entityRect.x, entityRect.y, false, &rock);
		break;
	case PuzzleId::SLIDING_ROCK:
		{
			app->render->DrawTexture(app->entities->puzzleTex, entityRect.x, entityRect.y, false, &slidingRock);
			break;
		}
		case PuzzleId::BUTTON:
		{
			if (!isPushed)
			{
				app->render->DrawTexture(app->entities->puzzleTex, entityRect.x, entityRect.y, false, &button);
			}
			else
			{
				app->render->DrawTexture(app->entities->puzzleTex, entityRect.x, entityRect.y, false, &buttonPressed);
			}
			break;
		}
	default:
		break;
	}
	if (app->render->debug)
	{
		if (collider != nullptr) app->render->DrawRectangle(collider->rect, 0, 0, 150, 100);
	}

	return true;
}

void PuzzlePieces::OnCollision(Collider* c1, Collider* c2)
{
	switch (puzzleId)
	{
	case PuzzleId::ROCK:
		
		break;
	case PuzzleId::SLIDING_ROCK:
	{
		if (this->collider->rect.y + this->collider->rect.h > playerPtr->entityRect.y)
		{
			//this->collider->rect.x += 64;
		}
		if (this->collider->rect.y < playerPtr->entityRect.y + playerPtr->entityRect.h)
		{

		}
		break;
	}
	default:
		break;
	}
}