#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "Module.h"
#include "SDL.h"
#include "Point.h"

#define MAX_COLLIDERS 250

class Collider
{
public:
	// Collision Type enum
	enum class Type
	{
		NONE = -1,
		AIR,
		SOLID,
		DOOR,
		PUZZLE,
		OTHER,
		EVENT,
		ENEMY_SPAWN,
		INTERACTABLE,
		DEBUG,
		PLAYER,
		BUTTON,
		MOVEABLE,
		SOLID_ROCK,
		MAX
	};

	// Constructor
	Collider(SDL_Rect rect, Type type, Module* listener = nullptr) : rect(rect), type(type), listener(listener)
	{}

	// Sets the position of the construct
	void SetPos(int x, int y, int w, int h);

	// Checks if two rects are intersecting
	bool Intersects(const SDL_Rect& r) const;

	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listener = nullptr;
};

class Collisions : public Module
{
public:
	// Constructor
	Collisions();

	// Called when program is executed
	void Init();

	// Destructor
	virtual ~Collisions();

	// Called before player is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Save/Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
	SDL_Rect ResolveCollisions(Collider* collider, iPoint nextFrame, float dt);

	int saveFx;
	int toiletFx;
	int doorFx;
	int tutorialWarpFx;
	int wakingUpFx;

private:
	// All existing colliders in the scene
	Collider* colliders[MAX_COLLIDERS] = { nullptr };

	// The collision matrix. Defines the interaction for two collider types
	// If set to false, collider 1 will ignore collider 2
	bool matrix[(uint)Collider::Type::MAX][(uint)Collider::Type::MAX];

	bool onceNightmare;
	bool saveOnce;
	bool onceDoor;

	char folderAudioFx[TEXT_LEN] = { 0 };
};
#endif // !__COLLISIONS_H__