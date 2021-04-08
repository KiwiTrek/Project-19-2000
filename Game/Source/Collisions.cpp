#include "Collisions.h"

#include "App.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Collisions::Collisions()
{
	name.Create("collisions");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		colliders[i] = nullptr;
	}

	matrix[Collider::Type::AIR][Collider::Type::AIR] = false;
	matrix[Collider::Type::AIR][Collider::Type::SOLID] = false;
	matrix[Collider::Type::AIR][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::AIR][Collider::Type::INTERACTABLE] = false;

	matrix[Collider::Type::SOLID][Collider::Type::AIR] = false;
	matrix[Collider::Type::SOLID][Collider::Type::SOLID] = false;
	matrix[Collider::Type::SOLID][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::SOLID][Collider::Type::INTERACTABLE] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::AIR] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::SOLID] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::INTERACTABLE] = true;

	matrix[Collider::Type::INTERACTABLE][Collider::Type::AIR] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::SOLID] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::INTERACTABLE] = true;

}

Collisions::~Collisions()
{}

void Collisions::Init()
{
	active = false;
}

bool Collisions::Start()
{
	return true;
}

bool Collisions::Awake(pugi::xml_node&)
{
	return true;
}

bool Collisions::PreUpdate()
{
	// Delete colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	// Check collisions
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
		{
			continue;
		}

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
			{
				continue;
			}

			c2 = colliders[k];

			if (c1->Intersects(c2->rect) && matrix[c1->type][c2->type])
			{
				if (c1->listener)
				{
					c1->listener->OnCollision(c1, c2);
				}
				if (c2->listener)
				{
					c2->listener->OnCollision(c2, c1);
				}
			}
		}
	}

	return true;
}

bool Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

bool Collisions::Load(pugi::xml_node&)
{
	return true;
}

bool Collisions::Save(pugi::xml_node&)
{
	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;
	// Adds the collider in the first null spot on the array
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}


SDL_Rect Collisions::ResolveCollisions(Collider* collider, iPoint nextFrame)
{
	if (app->map->data.type != MapTypes::MAPTYPE_UNKNOWN)
	{
		iPoint difference = { nextFrame.x - collider->rect.x,nextFrame.y - collider->rect.y };
		iPoint tilePos = app->map->WorldToMap(nextFrame.x, nextFrame.y);

		iPoint what = app->map->MapToWorld(tilePos.x, tilePos.y);
		iPoint the = app->map->WorldToMap(nextFrame.x + collider->rect.w, nextFrame.y + collider->rect.h);

		SDL_Rect tileRect = { what.x,what.y,app->map->data.tileWidth,app->map->data.tileHeight };
		if ((app->map->GetTileProperty(tilePos.x, tilePos.y, "CollisionId") == Collider::Type::SOLID
			|| app->map->GetTileProperty(the.x, the.y, "CollisionId") == Collider::Type::SOLID)
			&& collider->Intersects(tileRect))
		{
			if (difference.x > 0) nextFrame.x--;
			else if (difference.x < 0) nextFrame.x++;
			if (difference.y > 0) nextFrame.y--;
			else if (difference.y < 0) nextFrame.y++;

			if (difference == iPoint(0, 0))
			{
				return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
			}
			return ResolveCollisions(collider, nextFrame);
		}
	}
	return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
}

void Collider::SetPos(int x, int y, int w, int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

bool Collider::Intersects(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w
		&& rect.x + rect.w > r.x
		&& rect.y < r.y + r.h 
		&& rect.h + rect.y > r.y);
}