#include "SceneGameplay.h"

#include "Audio.h"
#include "EntityManager.h"
#include "Input.h"
#include "Render.h"

SceneGameplay::SceneGameplay()
{
	//MENU
	btnInventory = new GuiButton(1, { 90, 120, 200, 60 }, "INVENTORY");
	btnInventory->SetObserver(this);

	btnSkills = new GuiButton(2, { 90, 200, 200, 60 }, "SKILLS");
	btnSkills->SetObserver(this);

	btnSkillTree = new GuiButton(3, { 90, 280, 200, 60 }, "SKILL TREE");
	btnSkillTree->SetObserver(this);

	btnEquipment = new GuiButton(4, { 90, 360, 200, 60 }, "EQUIPMENT");
	btnEquipment->SetObserver(this);

	btnStats = new GuiButton(5, { 90, 440, 200, 60 }, "STATS");
	btnStats->SetObserver(this);

	btnOptions = new GuiButton(6, { 90, 520, 200, 60 }, "OPTIONS");
	btnOptions->SetObserver(this);

	btnTitleScreen = new GuiButton(7, { 90, 600, 200, 60 }, "TITLE SCREEN");
	btnTitleScreen->SetObserver(this);

	//OPTIONS


	//CONTROLS


	flags = 0;
}

SceneGameplay::~SceneGameplay()
{}

bool SceneGameplay::Load() /*EntityManager entityManager)*/
{
	app->entities->Enable();

	dialogueFont = new Font("Assets/Fonts/DialogueFont.xml");

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

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) TransitionToScene(SceneType::DEV_ROOM);

	if ((flags & 1<<Flags::MENU) != 0)
	{
		app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);

		btnInventory->Update(dt);
		btnSkills->Update(dt);
		btnSkillTree->Update(dt);
		btnEquipment->Update(dt);
		btnStats->Update(dt);
		btnOptions->Update(dt);
		btnTitleScreen->Update(dt);

		if ((flags & 1<<Flags::INVENTORY) != 0)
		{

		}
		else if((flags & 1 << Flags::SKILLS) != 0)
		{

		}
		else if ((flags & 1 << Flags::SKILL_TREE) != 0)
		{

		}
		else if ((flags & 1 << Flags::EQUIPMENT) != 0)
		{

		}
		else if ((flags & 1 << Flags::STATS) != 0)
		{

		}
		else if ((flags & 1 << Flags::OPTIONS) != 0)
		{

		}
	}





	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->CheckButton("menu", KeyState::KEY_DOWN))
	{
		ResetButtons();
		flags = ToggleBit(flags, Flags::MENU);
	}

	return true;
}

bool SceneGameplay::Draw()
{
	app->render->background = { 0,0,150,255 };

	player->Draw();

	if ((flags & 1<<Flags::MENU) != 0)
	{
		btnInventory->Draw();
		btnSkills->Draw();
		btnSkillTree->Draw();
		btnEquipment->Draw();
		btnStats->Draw();
		btnOptions->Draw();
		btnTitleScreen->Draw();

		if ((flags & 1 << Flags::INVENTORY) != 0)
		{

		}
		else if ((flags & 1 << Flags::SKILLS) != 0)
		{

		}
		else if ((flags & 1 << Flags::SKILL_TREE) != 0)
		{

		}
		else if ((flags & 1 << Flags::EQUIPMENT) != 0)
		{

		}
		else if ((flags & 1 << Flags::STATS) != 0)
		{

		}
		else if ((flags & 1 << Flags::OPTIONS) != 0)
		{

		}
	}

	return false;
}

bool SceneGameplay::Unload()
{
	// TODO: Unload all resources
	app->entities->Disable();

	return false;
}

void SceneGameplay::ResetButtons()
{
	btnInventory->state = GuiControlState::NORMAL;
	btnEquipment->state = GuiControlState::NORMAL;
	btnSkills->state = GuiControlState::NORMAL;
	btnSkillTree->state = GuiControlState::NORMAL;
	btnEquipment->state = GuiControlState::NORMAL;
	btnStats->state = GuiControlState::NORMAL;
	btnOptions->state = GuiControlState::NORMAL;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneGameplay::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 1: //INVENTORY
		ResetButtons();
		btnInventory->state = GuiControlState::DISABLED;
		flags = 1<<Flags::MENU;
		flags = SetBit(flags, 1<<Flags::INVENTORY);
		break;
	case 2: //SKILLS
		ResetButtons();
		btnSkills->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, 1 << Flags::SKILLS);
		break;
	case 3: //SKILL TREE
		ResetButtons();
		btnSkillTree->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, 1 << Flags::SKILL_TREE);
		break;
	case 4: //EQUIPMENT
		ResetButtons();
		btnEquipment->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, 1 << Flags::EQUIPMENT);
		break;
	case 5: //STATS
		ResetButtons();
		btnStats->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, 1 << Flags::STATS);
		break;
	case 6: //OPTIONS
		ResetButtons();
		btnOptions->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, 1 << Flags::OPTIONS);
		break;
	case 7: // TITLE SCREEN
		TransitionToScene(SceneType::TITLE_SCREEN);
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		break;
	case 11:
		break;
	case 12:
		break;
	default:
		break;
	}

	return true;
}
