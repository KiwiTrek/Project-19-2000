#include "SceneDevRoom.h"
#include "SceneCombat.h"

#include "Audio.h"
#include "GuiManager.h"
#include "EntityManager.h"
#include "Input.h"
#include "Render.h"
#include "Map.h"
#include "Textures.h"
#include "DialogSystem.h"
#include "DialogUtils.h"
#include "Player.h"

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
	app->gui->Enable();

	dialogSystem = new DialogSystem();

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
	Player* tmp = (Player*)player;

	if (dialogSystem->currentDialog != nullptr)
	{
		tmp->inDialog = true;
	}
	else
	{
		tmp->inDialog = false;
	}

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

	// The key to skip to the next dialog line.
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) {
		dialogSystem->NextDialog();
	}

	// Select the next option.
	if (dialogSystem->currentDialog != nullptr && app->input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN) {
		dialogSystem->selectedOption += 1;
		if (dialogSystem->selectedOption == dialogSystem->currentDialog->children->size())
			dialogSystem->selectedOption = dialogSystem->currentDialog->children->size() - 1;
	}

	// Select the previous option.
	if (dialogSystem->currentDialog != nullptr && app->input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN) {
		dialogSystem->selectedOption -= 1;
		if (dialogSystem->selectedOption < 0) dialogSystem->selectedOption = 0;
	}

	/* ONLY FOR TESTING */

	if (app->input->GetKey(SDL_SCANCODE_K) == KeyState::KEY_DOWN) {
		dialogSystem->StartDialog("1");
	}

	if (app->input->GetKey(SDL_SCANCODE_L) == KeyState::KEY_DOWN) {
		dialogSystem->StartDialog("2");
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

	if (dialogSystem->currentDialog != nullptr)
	{
		dialogSystem->DrawDialog(dialogueFont);
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
	app->gui->Disable();
	app->map->Disable();

	return false;
}