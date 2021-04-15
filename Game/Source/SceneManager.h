#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"

class SceneManager : public Module
{
public:

	SceneManager();

	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	Scene* current;

	ListItem<GuiControl*>* currentButton = nullptr;

	bool onTransition;
	bool continueLoadRequest;
	float transitionAlpha;

	bool fullscreenChecked = false;
private:
	Scene* next;
	bool fadeOutCompleted;


};

#endif // __SCENEMANAGER_H__