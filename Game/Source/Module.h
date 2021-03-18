#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "pugixml.hpp"

class App;
class Collider;
class GuiControl;

class Module
{
public:
	// Constructor
	Module() : active(false)
	{}

	// Called when program is executed
	virtual void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called before all Updates
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called after all Updates
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// Load
	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	// Save
	virtual bool Save(pugi::xml_node&)
	{
		return true;
	}

	// Enable module
	virtual void Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	// Disable module
	virtual void Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	// Collision response
	virtual void OnCollision(Collider* c1, Collider* c2) {}

	// On mouse click response
	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:
	SString name;
	bool active;
};

#endif // __MODULE_H__