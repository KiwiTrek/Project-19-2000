#include "SceneDevRoom.h"

#include "Audio.h"
#include "EntityManager.h"
#include "Input.h"
#include "Render.h"
#include "Map.h"
#include "Textures.h"

SceneDevRoom::SceneDevRoom()
{
}

SceneDevRoom::~SceneDevRoom()
{}

bool SceneDevRoom::Load()
{
	app->entities->Enable();
	app->map->Enable();
	//tileSetTex = app->tex->Load("dev_room_tiles.png");
	app->map->Load("devs_room.tmx");

	// initialize player
	player = app->entities->CreateEntity(-1, -1, EntityType::PLAYER, EntityId::NOT_COMBAT, NULL);
	enemyTest = app->entities->CreateEntity(400, 400, EntityType::COMBAT_ENTITY, EntityId::ENEMY_1, Stats(10, 10, 10, 10, 1000, 100, 2.0f));

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
	app->render->background = { 100,100,100,255 };
	app->map->Draw();
	player->Draw();

	return false;
}

bool SceneDevRoom::Unload()
{
	// TODO: Unload all resources
	app->entities->Disable();

	return false;
}
