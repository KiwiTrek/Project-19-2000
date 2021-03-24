#include "SceneDevRoom.h"

#include "Audio.h"
#include "EntityManager.h"
#include "Input.h"
#include "Render.h"

SceneDevRoom::SceneDevRoom()
{
}

SceneDevRoom::~SceneDevRoom()
{}

bool SceneDevRoom::Load()
{
	app->entities->Enable();

	// initialize player
	player = app->entities->CreateEntity(-1, -1, EntityType::PLAYER);

	app->audio->StopMusic();

	return false;
}

bool SceneDevRoom::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) TransitionToScene(SceneType::TITLE_SCREEN);
	// L02: DONE 3: Request Load / Save when pressing L/S

	return true;
}

bool SceneDevRoom::Draw()
{
	app->render->background = { 0,100,0,255 };

	player->Draw();

	return false;
}

bool SceneDevRoom::Unload()
{
	// TODO: Unload all resources
	app->entities->Disable();

	return false;
}
