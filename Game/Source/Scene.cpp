#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// TRANSITIONS
	currentStep = TransitionScene::NO_TRANSITION;
	frameCount = 0;
	maxFrames = 0;
	fadeRatio = 0.0f;
	currentGameScene = ScenesList::LOGO;
	nextGameScene = ScenesList::NONE;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	TransitionUpdate(dt);

	if (app->render->debug)
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y -= 1;

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y += 1;

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x -= 1;

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x += 1;
	}

	if (currentGameScene == ScenesList::GAMEPLAY)
	{
		//GAMEPLAY LOGIC

	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	switch (currentGameScene)
	{
	case Scene::NONE:
		break;
	case Scene::LOGO:

		nextGameScene = ScenesList::TITLE_SCREEN;
		FadeEffect(false, 60.0f);


		app->render->background = { 255,255,255,255 };
		app->render->DrawTexture(logo, app->win->screenSurface->w - 250, app->win->screenSurface->h - 250);
		LOG("AAAAAAAAAAAA");
		break;
	case Scene::TITLE_SCREEN:
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
		{
			nextGameScene = ScenesList::GAMEPLAY;
			FadeEffect(false, 60.0f);
		}

		app->render->background = { 0,0,0,255 };
		app->render->DrawTexture(nooseBG, 810, 0);
		app->render->DrawTexture(titleCard, 90, 64);
		break;
	case Scene::GAMEPLAY:
		app->render->background = { 0,0,150,255 };
		break;
	default:
		break;
	}

	TransitionPostUpdate();

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::TransitionUpdate(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == TransitionScene::NO_TRANSITION)
	{
		return true;
	}

	if (currentStep == TransitionScene::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFrames)
		{
			//DELETE THE SCENE YOU COME FROM (CLEANUPS)
			switch (currentGameScene)
			{
			case Scene::NONE:
				break;
			case Scene::LOGO:
				//DeleteLogoScene();
				if (logo != nullptr)
					app->tex->UnLoad(logo);
				break;
			case Scene::TITLE_SCREEN:
				//DeleteTitleScene();
				if (nooseBG != nullptr)
					app->tex->UnLoad(nooseBG);
				if (titleCard != nullptr)
					app->tex->UnLoad(titleCard);
				break;
			case Scene::GAMEPLAY:
				//DeleteGameplayScene();
				break;
			default:
				break;
			}

			//CREATE THE SCENE YOU ARE GOING TO (STARTS)
			switch (nextGameScene)
			{
			case Scene::NONE:
				break;
			case Scene::LOGO:
				//CreateLogoScene();
				logo = app->tex->Load("Assets/Textures/Logo.png");
				break;
			case Scene::TITLE_SCREEN:
				//CreateTitleScene();
				nooseBG = app->tex->Load("Assets/Textures/NooseBG.png");
				titleCard = app->tex->Load("Assets/Textures/TitleCard.png");
				break;
			case Scene::GAMEPLAY:
				//CreateGameplayScene();
				break;
			default:
				break;
			}
			currentStep = TransitionScene::FROM_BLACK;
			currentGameScene = nextGameScene;
			nextGameScene = ScenesList::NONE;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = TransitionScene::NO_TRANSITION;
		}
	}
	return true;
}

bool Scene::TransitionPostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == TransitionScene::NO_TRANSITION)
	{
		return true;
	}

	fadeRatio = (float)frameCount / (float)maxFrames;

	// Render the black square with alpha on the screen
	app->render->DrawRectangle(app->render->camera, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));

	return true;
}

bool Scene::FadeEffect(bool fadeInOnly, float frames)
{
	// If we are already in a fade process, ignore this call
	if (currentStep == TransitionScene::NO_TRANSITION)
	{
		if (fadeInOnly == false)
		{
			currentStep = TransitionScene::TO_BLACK;
			frameCount = 0;
			maxFrames = frames;

			//this->moduleToDisable = moduleToDisable;
			//this->moduleToEnable = moduleToEnable;

			return true;
		}
		else
		{
			currentStep = TransitionScene::FROM_BLACK;
			frameCount = frames;
			maxFrames = frames;

			return true;
		}
	}

	return false;
}

bool Scene::Load(pugi::xml_node& save)
{

}

bool Scene::Save(pugi::xml_node& save)
{

}