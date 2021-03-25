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

//#ifndef __MODULE_PHYSICS_H__
//#define __MODULE_PHYSICS_H__
//
//#include "Module.h"
//#include "Globals.h"
//#include "p2Point.h"
//#include "Object.h"
//
//#define WATER_DENSITY 0.0025
//#define DAMPEN 0.975
//
//class Collider
//{
//public:
//	// Collision Type enum
//	enum Type
//	{
//		NONE = -1,
//		SOLID,
//		WATER,
//		MOON,
//		PLAYER,
//		ASTEROID,
//		MAX
//	};
//
//	// Constructor
//	Collider(SDL_Rect rect, Type type, Module* listener = nullptr) : rect(rect), type(type), listener(listener)
//	{}
//
//	// Sets the position of the construct
//	void SetPos(int x, int y, int w, int h);
//
//	SDL_Rect rect;
//	Type type;
//	Module* listener = nullptr;
//};
//
//class ModulePhysics : public Module
//{
//public:
//	ModulePhysics(Application* app, bool startEnabled = false);
//	~ModulePhysics();
//
//	bool Start();
//	UpdateStatus PreUpdate();
//	UpdateStatus Update(float dt);
//	bool CleanUp();
//
//	// Adds a new object to the list
//	void AddObject(Object* object);
//	// Removes an existing object from the list
//	void RemoveObject(Object* object);
//
//	void UpdateDrag(fPoint direction);
//
//private:
//	// Calculates acceleration from force
//	fPoint Force2Accel(fPoint force, int mass);
//
//	// Updates gravity
//	void UpdateGravity();
//
//	// Steps an object in the world
//	void UpdatePhysics(Object* object, float dt);
//
//	// Checks if two objects are intersecting
//	bool Intersects(Object* A, Object* B);
//
//	// Resolves two objects colliding
//	void ResolveCollisions(Object* A, Object* B);
//
//	// Calculates the shortest distance between a circle and a rect
//	float ShortestDist(Object* A, Circle* B, iPoint& rPos);
//
//	// Applies buoyancy to the object B
//	void Buoyancy(Object* A, Object* B);
//
//public:
//	fPoint gravity;
//	bool debug;
//	fPoint dragDirection;
//
//private:
//	// List of all existing objects in the scene
//	p2List<Object*> objects;
//
//	// The collision matrix
//	// Defines the interaction for two collider types
//	// If set to false, collider 1 will ignore collider 2
//	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
//};
//
//#endif // !__MODULE_PHYSICS_H__