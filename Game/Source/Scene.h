#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"
#include "Font.h"

class Font;

enum class SceneType
{
    NONE,
	LOGO,
	TITLE_SCREEN,
	GAMEPLAY,
	ENDING
};

enum class CombatStateType
{
	COMBAT_START,
	COMBAT_MIDGAME,
	COMBAT_END
};

class Scene
{
public:

	Scene() : active(true), loaded(false), transitionRequired(false)
	{
		combat = false;
		combatCooldown = 1.0f;
		enteringCombat = false;
		usingGamepad = false;
		inMenu = false;
		changeMenu = false;
        combatState = CombatStateType::COMBAT_START;
        currentScene = SceneType::NONE;
        nextScene = SceneType::NONE;
	}

	virtual bool Load()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw()
	{
		return true;
	}

	virtual bool DrawPauseMenu()
	{
		return true;
	}

	virtual bool Unload()
	{
		return true;
	}

	void TransitionToScene(SceneType scene)
	{
		transitionRequired = true;
		nextScene = scene;
	}

	// Define multiple Gui Event methods
	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:

	bool active = true;

	// Possible properties
	bool loaded = false;
	// TODO: Transition animation properties

	bool transitionRequired;
	SceneType nextScene;
	SceneType currentScene;
	bool combat;
	float combatCooldown;
	CombatStateType combatState;
	bool enteringCombat;

	bool usingGamepad;

	bool inMenu;
	bool changeMenu;
};

#endif // __SCENE_H__