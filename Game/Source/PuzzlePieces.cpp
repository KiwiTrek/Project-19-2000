#include "PuzzlePieces.h"

#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Map.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Collisions.h"


PuzzlePieces::PuzzlePieces(int x, int y, PuzzleId id, Entity* player) : Entity(x, y, EntityType::PUZZLE_PIECE)
{
	LOG("Init PuzzlePieces");
	this->puzzleId = id;
	this->playerPtr = (Player*)player;

	this->entityRect = { x, y,64,64 };

	rock = { 0,256,64,64 };
	slidingRock = { 0,384,64,64 };
	button = { 896,0,64,64 };
	buttonPressed = { 960,0,64,64 };

	pendingToDelete = false;


	if (id == PuzzleId::BUTTON)
	{
		collider = app->collisions->AddCollider({ x,y,64,64 }, Collider::Type::BUTTON, (Module*)app->entities);
		isPressed = false;
	}
	else
	{
		collider = app->collisions->AddCollider({ x,y,64,64 }, Collider::Type::MOVEABLE, (Module*)app->entities);
		collision = app->collisions->AddCollider({ x + 4,y + 4,56,56 }, Collider::Type::SOLID, (Module*)app->entities);

		isPushedUp = false;
		isPushedDown = false;
		isPushedLeft = false;
		isPushedRight = false;
		value = 0;
	}
}


bool PuzzlePieces::Update(float dt)
{
	switch (puzzleId)
	{
	case PuzzleId::ROCK:
	{
		if (isPushedUp)
		{
			entityRect.y -= 2;
			value++;
			if (value >= 32)
			{
				value = 0;
				isPushedUp = false;
			}
		}
		else if (isPushedDown)
		{
			entityRect.y += 2;
			value++;
			if (value >= 32)
			{
				value = 0;
				isPushedDown = false;
			}
		}
		else if (isPushedLeft)
		{
			entityRect.x -= 2;
			value++;
			if (value >= 32)
			{
				value = 0;
				isPushedLeft = false;
			}
		}
		else if (isPushedRight)
		{
			entityRect.x += 2;
			value++;
			if (value >= 32)
			{
				value = 0;
				isPushedRight = false;
			}
		}

		if (value == 0)
		{
			CorrectPosition();
		}

		break;
	case PuzzleId::SLIDING_ROCK:
	{
		if (isPushedUp)
		{
			entityRect.y -= 2;
			value++;
			if (value >= 32)
			{
				value = 0;

				CorrectPosition();

				if (!CheckUp())
				{
					isPushedUp = false;
				}		
			}
		}
		else if (isPushedDown)
		{
			entityRect.y += 2;
			value++;
			if (value >= 32)
			{
				value = 0;

				CorrectPosition();

				if (!CheckDown())
				{
					isPushedDown = false;
				}
			}
		}
		else if (isPushedLeft)
		{
			entityRect.x -= 2;
			value++;
			if (value >= 32)
			{
				value = 0;

				CorrectPosition();

				if (!CheckLeft())
				{
					isPushedLeft = false;
				}
			}
		}
		else if (isPushedRight)
		{
			entityRect.x += 2;
			value++;
			if (value >= 32)
			{
				value = 0;

				CorrectPosition();

				if (!CheckRight())
				{
					isPushedRight = false;
				}
			}
		}

		break;
	}
	}
	}

	collider->SetPos(entityRect.x, entityRect.y, entityRect.w, entityRect.h);
	if (collision != nullptr)
	{
		collision->SetPos(entityRect.x + 4, entityRect.y + 4, collision->rect.w, collision->rect.h);
	}

	return true;
}

bool PuzzlePieces::Draw()
{
	switch (puzzleId)
	{
	case PuzzleId::ROCK:
	{
		app->render->DrawTexture(app->entities->puzzleTex, entityRect.x, entityRect.y, false, &rock);
		break;
	}
	case PuzzleId::SLIDING_ROCK:
		{
			app->render->DrawTexture(app->entities->puzzleTex, entityRect.x, entityRect.y, false, &slidingRock);
			break;
		}
		case PuzzleId::BUTTON:
		{
			if (!isPressed)
			{
				app->render->DrawTexture(app->entities->puzzleTex, entityRect.x, entityRect.y, false, &button);
			}
			else
			{
				app->render->DrawTexture(app->entities->puzzleTex, entityRect.x, entityRect.y, false, &buttonPressed);
			}

			isPressed = false;

			break;
		}
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

void PuzzlePieces::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::BUTTON)
	{	
		isPressed = true;
	}
	if (c1->type == Collider::Type::MOVEABLE)
	{
		if (c2->type == Collider::Type::PLAYER)
		{
			if (playerPtr->animFlags == 1 << Player::FlagsAnimation::UP)
			{
				if (!isPushedUp)
				{
					isPushedUp = CheckUp();
				}
			}
			if (playerPtr->animFlags == 1 << Player::FlagsAnimation::DOWN)
			{
				if (!isPushedDown)
				{
					isPushedDown = CheckDown();
				}
			}
			if (playerPtr->animFlags == 1 << Player::FlagsAnimation::LEFT)
			{
				if (!isPushedLeft)
				{
					isPushedLeft = CheckLeft();
				}
			}
			if (playerPtr->animFlags == 1 << Player::FlagsAnimation::RIGHT)
			{
				if (!isPushedRight)
				{
					isPushedRight = CheckRight();
				}
			}
		}
	}
}

bool PuzzlePieces::CheckUp()
{
	ListItem<Entity*>* e = app->entities->entities.start;
	while (e != nullptr)
	{
		if (e->data->collider->type == Collider::Type::MOVEABLE && e->data->collider->rect.x == entityRect.x && e->data->collider->rect.y == (entityRect.y - 64))
		{
			return false;
		}
		e = e->next;
	}
	if (app->map->GetTileProperty((entityRect.x / 64), (entityRect.y / 64) - 1, "CollisionId") == Collider::Type::PUZZLE || app->map->GetTileProperty((entityRect.x / 64), (entityRect.y / 64) - 1, "CollisionId") == Collider::Type::MOVEABLE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PuzzlePieces::CheckDown()
{
	ListItem<Entity*>* e = app->entities->entities.start;
	while (e != nullptr)
	{
		if (e->data->collider->type == Collider::Type::MOVEABLE && e->data->collider->rect.x == entityRect.x && e->data->collider->rect.y == (entityRect.y + 64))
		{
			return false;
		}
		e = e->next;
	}
	if (app->map->GetTileProperty((entityRect.x / 64), (entityRect.y / 64) + 1, "CollisionId") == Collider::Type::PUZZLE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PuzzlePieces::CheckLeft()
{
	ListItem<Entity*>* e = app->entities->entities.start;
	while (e != nullptr)
	{
		if (e->data->collider->type == Collider::Type::MOVEABLE && e->data->collider->rect.x == (entityRect.x - 64) && e->data->collider->rect.y == entityRect.y)
		{
			return false;
		}
		e = e->next;
	}
	if (app->map->GetTileProperty((entityRect.x / 64) - 1, (entityRect.y / 64), "CollisionId") == Collider::Type::PUZZLE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PuzzlePieces::CheckRight()
{
	ListItem<Entity*>* e = app->entities->entities.start;
	while (e != nullptr)
	{
		if (e->data->collider->type == Collider::Type::MOVEABLE && e->data->collider->rect.x == (entityRect.x + 64) && e->data->collider->rect.y == entityRect.y)
		{
			return false;
		}
		e = e->next;
	}
	if (app->map->GetTileProperty((entityRect.x / 64) + 1, (entityRect.y / 64), "CollisionId") == Collider::Type::PUZZLE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PuzzlePieces::CorrectPosition()
{
	prevPos = app->map->WorldToMap(entityRect.x, entityRect.y);
	prevPos = app->map->MapToWorld(prevPos.x, prevPos.y);
	entityRect.x = prevPos.x;
	entityRect.y = prevPos.y;
}