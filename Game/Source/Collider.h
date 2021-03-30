#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Module.h"
#include "SDL.h"

enum ColliderType
{
	ERROR = -1,
	GROUND,
	DOOR,
	WALL,
	DEATH,
	PLAYER,
	MAX
};

class Collider
{
public:
	// Constructor
	Collider(SDL_Rect rect, ColliderType type, Module* listener = nullptr) : rect(rect), type(type), listener(listener)
	{}

	// Sets the position of the construct
	//void SetPos(int x, int y, int w, int h)
	//{
	//	rect.x = x;
	//	rect.y = y;
	//	rect.w = w;
	//	rect.h = h;
	//}

	// Checks if two rects are intersecting
	bool Intersects(const SDL_Rect& r) const
	{
		return (rect.x < r.x + r.w && rect.x + rect.w > r.x && rect.y < r.y + r.h && rect.h + rect.y > r.y);
	}

public:
	SDL_Rect rect;
	ColliderType type;
	Module* listener = nullptr;
	bool pendingToDelete = false;
};

#endif // !__COLLIDER_H__