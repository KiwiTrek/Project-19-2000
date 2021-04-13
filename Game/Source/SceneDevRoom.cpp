#include "SceneDevRoom.h"
#include "SceneCombat.h"

#include "Audio.h"
#include "EntityManager.h"
#include "Input.h"
#include "Render.h"
#include "Map.h"
#include "Textures.h"

SceneDevRoom::SceneDevRoom()
{
	combatScene = new SceneCombat();
}

SceneDevRoom::~SceneDevRoom()
{}

bool SceneDevRoom::Load()
{
	app->collisions->Enable();
	app->entities->Enable();
	app->map->Enable();
	app->map->Load("dev_room.tmx");

	// initialize player
	player = app->entities->CreateEntity(-1, -1, EntityType::PLAYER, EntityId::NOT_COMBAT, NULL);

	dialogueFont = new Font("Assets/Fonts/DialogueFont.xml");

	combatScene->Load();

	app->audio->StopMusic();

	combat = false;
	enteringCombat = false;

	return false;
}

bool SceneDevRoom::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) TransitionToScene(SceneType::TITLE_SCREEN);
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		combat = true;
		enteringCombat = true;
	}
	// L02: DONE 3: Request Load / Save when pressing L/S

	if (combat)
	{
		if (enteringCombat)
		{
			enteringCombat = false;
			combatScene->Start();
		}
		combatScene->Update(dt);
	}

	return true;
}

bool SceneDevRoom::Draw()
{
	if (combat) combatScene->Draw(dialogueFont);
	else
	{
		app->render->background = { 100,100,100,255 };
		app->map->Draw();
	}

	return false;
}

bool SceneDevRoom::Unload()
{
	// TODO: Unload all resources
	if (combatScene != nullptr)
	{
		combatScene->Unload();
		combatScene = nullptr;
	}
	app->entities->Disable();
	if (dialogueFont != nullptr) delete dialogueFont;
	app->collisions->Disable();
	app->map->Disable();

	return false;
}