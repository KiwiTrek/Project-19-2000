#include "SceneDevRoom.h"

#include "Audio.h"
#include "EntityManager.h"
#include "Input.h"
#include "Render.h"
#include "Map.h"
#include "Textures.h"

SceneDevRoom::SceneDevRoom()
{
	ListItem<Entity*>* e = turnOrder.start;
	while (e != nullptr)
	{
		ListItem<Entity*>* eNext = e->next;
		int i = turnOrder.Find(e->data);
		delete turnOrder[i];
		turnOrder.Del(turnOrder.At(i));
		e = eNext;
	}
	turnOrder.Clear();
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
	playerTest = app->entities->CreateEntity(500, 200, EntityType::COMBAT_ENTITY, EntityId::MC, Stats(20, 20, 20, 20, 20, 20, 1.0f, 0.0f));
	enemyTest = app->entities->CreateEntity(400, 400, EntityType::COMBAT_ENTITY, EntityId::ENEMY_1, Stats(10, 10, 10, 10, 1000, 100, 2.0f));
	enemyTest2 = app->entities->CreateEntity(600, 400, EntityType::COMBAT_ENTITY, EntityId::ENEMY_2, Stats(10, 10, 10, 10, 1000, 100, 0.5f));

	app->audio->StopMusic();

	return false;
}

bool SceneDevRoom::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) TransitionToScene(SceneType::TITLE_SCREEN);
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) combat = !combat;
	// L02: DONE 3: Request Load / Save when pressing L/S

	if (combat)
	{
		//CHECK WHO IS FIRST
		ListItem<Entity*>* e = app->entities->entities.start;

		while (e != nullptr)
		{
			//ADDS THEM TO THE LIST
			if (e->data->type == EntityType::COMBAT_ENTITY)
			{
				turnOrder.Add(e->data);
			}
			e = e->next;
		}

		SortSpeed(false);

		e = turnOrder.start;
		while (e->data != nullptr)
		{
			if (IsCharacter(e->data))
			{
				//WAIT FOR PLAYER INPUT
			}
			else
			{
				//ENEMY ATTACK PATTERN?
			}
			e = e->next;
		}
	}

	return true;
}

bool SceneDevRoom::Draw()
{
	app->render->background = { 100,100,100,255 };
	app->map->Draw();
	//player->Draw();

	return false;
}

bool SceneDevRoom::Unload()
{
	// TODO: Unload all resources
	app->entities->Disable();

	return false;
}

bool SceneDevRoom :: SortSpeed(bool isSorted)
{
	int count = 0;
	ListItem<Entity*>* e = turnOrder.start;
	while (e != nullptr && e->next != nullptr)
	{
		if (e->data->stats.speed > e->next->data->stats.speed)
		{
			SWAP<Entity*>(e->data, e->next->data);
			count++;
		}
		e = e->next;
	}

	if (count != 0)
	{
		SortSpeed(false);
	}
	return true;
}

bool SceneDevRoom::IsCharacter(Entity* e)
{
	return (e->id != EntityId::MC && e->id != EntityId::KIND && e->id != EntityId::STUBBORN && e->id != EntityId::VIOLENT);
}
