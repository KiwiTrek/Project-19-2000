#ifndef __SCENEENDING_H__
#define __SCENEENDING_H__

#include "Scene.h"
#include "Point.h"
#include "GuiButton.h"

#include "SDL/include/SDL.h"

class SceneEnding : public Scene
{
public:

	SceneEnding();
	virtual ~SceneEnding();

	bool Load();

	bool Update(float dt);

	bool Draw();

	bool Unload();

	// Declare on mouse click event
	bool OnGuiMouseClickEvent(GuiControl* control);

private:

	SDL_Texture* bg;
	iPoint pos;

	GuiButton* btnContinue;
	GuiButton* btnTitle;
	GuiButton* btnExit;

	int state;
	float timeCounter;
};

#endif // __SCENEENDING_H__