#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"
#include "Font.h"

class Font;

enum SceneType
{
	LOGO,
	TITLE_SCREEN,
	GAMEPLAY,
    DEV_ROOM
};

class Scene
{
public:

	Scene() : active(true), loaded(false), transitionRequired(false) {}

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
    SString name;         // Scene name identifier?

    // Possible properties
    bool loaded = false;
    // TODO: Transition animation properties

    bool transitionRequired;
    SceneType nextScene;
};

#endif // __SCENE_H__