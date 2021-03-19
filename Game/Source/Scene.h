#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

class Scene : public Module
{
public:
	enum ScenesList
	{
		NONE = 0,
		LOGO,
		TITLE_SCREEN,
		GAMEPLAY
	};

	Scene();

	// Destructor
	virtual ~Scene();

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

	bool Load(pugi::xml_node& save);
	bool Save(pugi::xml_node& save);

	bool TransitionUpdate(float dt);
	bool TransitionPostUpdate();
	bool FadeEffect(bool fadeInOnly, float frames);

private:
	// ALL SCENE ASSETS
	SDL_Texture* logo;
	SDL_Texture* nooseBG;
	SDL_Texture* titleCard;

	// TRANSITIONS
	enum TransitionScene
	{
		NO_TRANSITION = 0,
		TO_BLACK,
		FROM_BLACK
	}

	currentStep = TransitionScene::NO_TRANSITION;
	uint32 frameCount = 0;
	uint32 maxFrames = 0;
	float fadeRatio = 0.0f;
	ScenesList currentGameScene = ScenesList::NONE;
	ScenesList nextGameScene = ScenesList::NONE;
};

#endif // __SCENE_H__