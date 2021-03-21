#include "SceneGameplay.h"

#include "Audio.h"
#include "EntityManager.h"
#include "Input.h"
#include "Render.h"

SceneGameplay::SceneGameplay()
{
}

SceneGameplay::~SceneGameplay()
{}

bool SceneGameplay::Load() /*EntityManager entityManager)*/
{
	app->entities->Enable();

	// Initialize player
	player = app->entities->CreateEntity(-1, -1, EntityType::PLAYER);

	app->audio->StopMusic();

	return false;
}

bool SceneGameplay::Update(float dt)
{
	// L02: DONE 3: Request Load / Save when pressing L/S
	//if (input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) app->LoadGameRequest();
	//if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) app->SaveGameRequest();

	return true;
}

bool SceneGameplay::Draw()
{
	app->render->background = { 0,0,150,255 };

	player->Draw();

	return false;
}

bool SceneGameplay::Unload()
{
	// TODO: Unload all resources
	app->entities->Disable();

	return false;
}
