#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "GuiManager.h"
#include "Map.h"
#include "SceneLogo.h"
#include "SceneTitleScreen.h"
#include "SceneGameplay.h"
#include "SceneDevRoom.h"

#include "Defs.h"
#include "Log.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager() : Module()
{
	name.Create("scenemanager");

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;;
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake()
{
	LOG("Loading Scene Manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	current = new SceneLogo();
	current->Load();
	next = nullptr;

	return true;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	if (!onTransition)
	{
		//if (app->render->debug)
		//{
		//	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) app->render->camera.y -= 1;
		//	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) app->render->camera.y += 1;
		//	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) app->render->camera.x -= 1;
		//	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) app->render->camera.x += 1;
		//}

		current->Update(dt);
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f)
			{
				transitionAlpha = 1.0f;

				current->Unload();	// Unload current screen
				next->Load();	// Load next screen

				RELEASE(current);	// Free current pointer
				current = next;		// Assign next pointer
				next = nullptr;

				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;
			}
		}
		else // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && current->currentScene != SceneType::GAMEPLAY) return false;

	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	bool ret = true;

	// Draw current scene
	current->Draw();

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
		case SceneType::LOGO:
			next = new SceneLogo();
			next->currentScene = SceneType::LOGO;
			break;
		case SceneType::TITLE_SCREEN:
			next = new SceneTitleScreen();
			next->currentScene = SceneType::TITLE_SCREEN;
			break;
		case SceneType::GAMEPLAY:
			next = new SceneGameplay();
			next->currentScene = SceneType::GAMEPLAY;
			break;
		case SceneType::DEV_ROOM:
			next = new SceneDevRoom();
			next->currentScene = SceneType::DEV_ROOM;
			break;
		default:
			break;
		}

		current->transitionRequired = false;
	}

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing scene");

	if (current != nullptr) current->Unload();

	return true;
}