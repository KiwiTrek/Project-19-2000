#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Point.h"
#include "App.h"
//#include "Map.h"

#include "SDL.h"
#include "Log.h"

class Collider;

struct Physics
{
public:
	void UpdatePhysics(iPoint& pos, float dt)
	{
		if (axisX)
		{
			pos.x = pos.x + speed.x * dt;
		}
		if (axisY)
		{
			// Euler
			if (verlet == false)
			{
				pos.y = pos.y + speed.y * dt;
			}
			// Verlet
			else
			{
				pos.y = pos.y + speed.y * dt + (gravity * dt * dt * 0.5);
				speed.y = speed.y + gravity * dt;
			}
		}
	}

	// Indicates the direction of the velocity
	void CheckDirection()
	{
		if (speed.y >= 0.0f)
		{
			positiveSpeedY = true;
		}
		else if (speed.y < 0.0f)
		{
			positiveSpeedY = false;
		}
	}

	// Collisions
	//void ResolveCollisions(SDL_Rect& currentFrame, iPoint nextFrame, bool goingLeft)
	//{
	//	iPoint tiledPos(currentFrame.x / app->generalTileSize, currentFrame.y / app->generalTileSize);
	//	iPoint correctedPos;
	//	iPoint checkedPos;

	//	// X axis
	//	if (!goingLeft) // Right
	//	{
	//		tiledPos.x = (currentFrame.x + currentFrame.w) / app->generalTileSize;
	//		int i = 0;
	//		while (app->map->GetTileProperty(tiledPos.x + i, tiledPos.y, "CollisionId") == Collider::Type::AIR && i < 5)
	//		{
	//			i++;
	//		}
	//		i--;
	//		correctedPos.x = MIN(nextFrame.x - currentFrame.x, (tiledPos.x + i) * app->generalTileSize - currentFrame.x);
	//	}
	//	else // Left
	//	{
	//		int i = 0;
	//		while (app->map->GetTileProperty(tiledPos.x - i, tiledPos.y, "CollisionId") == Collider::Type::AIR && i < 5)
	//		{
	//			i++;
	//		}
	//		i--;
	//		correctedPos.x = -MIN(currentFrame.x - nextFrame.x, currentFrame.x - (tiledPos.x - i) * app->generalTileSize);
	//	}

	//	// Y axis
	//	if (positiveSpeedY) // Bottom
	//	{
	//		tiledPos.y = (currentFrame.y + currentFrame.h) / app->generalTileSize;
	//		int i = 0;
	//		while (app->map->GetTileProperty(tiledPos.x, tiledPos.y + i, "CollisionId") == Collider::Type::AIR && i < 5)
	//		{
	//			i++;
	//		}
	//		i--;
	//		correctedPos.y = MIN(nextFrame.y - currentFrame.y, (tiledPos.y + i) * app->generalTileSize - currentFrame.y);
	//	}
	//	else // Top
	//	{
	//		int i = 0;
	//		while (app->map->GetTileProperty(tiledPos.x, tiledPos.y - i, "CollisionId") == Collider::Type::AIR && i < 5)
	//		{
	//			i++;
	//		}
	//		i--;
	//		correctedPos.y = -MIN(currentFrame.y - nextFrame.y, currentFrame.y - (tiledPos.y - i) * app->generalTileSize);
	//	}
	//	currentFrame.x += correctedPos.x;
	//	currentFrame.y += correctedPos.y;

	//	iPoint currentFrameTile = { currentFrame.x / app->generalTileSize, currentFrame.y / app->generalTileSize };

	//	if (app->map->GetTileProperty(currentFrameTile.x + 1, currentFrameTile.y, "CollisionId") == Collider::Type::SOLID
	//		&& app->map->GetTileProperty(currentFrameTile.x, currentFrameTile.y + 1, "CollisionId") != Collider::Type::SOLID
	//		&& !goingLeft)
	//	{
	//		currentFrame.y -= correctedPos.y;
	//		speed.x = 0.0f;
	//		speed.y = 0.0f;
	//	}
	//	else if (app->map->GetTileProperty((currentFrame.x - 1) / app->generalTileSize, currentFrameTile.y, "CollisionId") == Collider::Type::SOLID
	//		&& app->map->GetTileProperty(currentFrameTile.x, currentFrameTile.y, "CollisionId") != Collider::Type::SOLID
	//		&& goingLeft)
	//	{
	//		currentFrame.y -= correctedPos.y;
	//		speed.x = 0.0f;
	//		speed.y = 0.0f;
	//	}
	//	else if (app->map->GetTileProperty(currentFrameTile.x, currentFrameTile.y + 1, "CollisionId") == Collider::Type::SOLID
	//		|| app->map->GetTileProperty(currentFrameTile.x + 1, currentFrameTile.y + 1, "CollisionId") == Collider::Type::SOLID)
	//	{
	//		speed.y = 0.0f;
	//	}
	//	else if (app->map->GetTileProperty(currentFrameTile.x, currentFrameTile.y, "CollisionId") == Collider::Type::SOLID)
	//	{
	//		speed.y = 0.0f;
	//	}
	//}

	// Collisions related variables
	bool axisX;
	bool axisY;

	// Speed related variables
	bool positiveSpeedY;
	bool verlet = true;
	fPoint speed;
	float gravity = 600.0f;
};

#endif // !__PHYSICS_H__
