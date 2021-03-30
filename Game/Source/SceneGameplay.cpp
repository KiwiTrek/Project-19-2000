#include "SceneGameplay.h"

#include "Audio.h"
#include "EntityManager.h"
#include "GuiManager.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"

SceneGameplay::SceneGameplay()
{
	float tmpValue = 0;

	//MENU
	btnInventory = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 1, { 90, 120, 200, 60 }, "INVENTORY", 40, this);
	btnSkills = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 2, { 90, 200, 200, 60 }, "SKILLS", 40, this);
	btnSkillTree = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 3, { 90, 280, 200, 60 }, "SKILL TREE", 35, this);
	btnEquipment = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 4, { 90, 360, 200, 60 }, "EQUIPMENT", 40, this);
	btnStats = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 5, { 90, 440, 200, 60 }, "STATS", 40, this);
	btnOptions = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 6, { 90, 520, 200, 60 }, "OPTIONS", 40, this);
	btnTitleScreen = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 7, { 90, 600, 200, 60 }, "TITLE SCREEN", 35, this);

	//OPTIONS
	sldrVolume = (GuiSlider*)app->gui->CreateGuiControl(GuiControlType::SLIDER, 8, { 180, 200, 69, 42 }, "VOLUME", 40, this, 6);
	sldrVolume->value = app->audio->GetMusicVolume();
	sldrVolume->maxValue = 128;
	tmpValue = (float)(sldrVolume->limits.w - sldrVolume->bounds.w) / (float)sldrVolume->maxValue;
	sldrVolume->bounds.x = sldrVolume->limits.x + (tmpValue * sldrVolume->value);

	sldrFx = (GuiSlider*)app->gui->CreateGuiControl(GuiControlType::SLIDER, 9, { 800, 200, 69, 42 }, "FX", 40, this, 6);
	sldrFx->value = app->audio->GetFxVolume();
	sldrFx->maxValue = 128;
	tmpValue = (float)(sldrFx->limits.w - sldrFx->bounds.w) / (float)sldrFx->maxValue;
	sldrFx->bounds.x = sldrFx->limits.x + (tmpValue * sldrFx->value);

	boxFullScreen = (GuiCheckBox*)app->gui->CreateGuiControl(GuiControlType::CHECKBOX, 10, { 180, 400, 60, 60 }, "FULLSCREEN", 40, this);
	boxVSync = (GuiCheckBox*)app->gui->CreateGuiControl(GuiControlType::CHECKBOX, 11, { 800, 400, 60, 60 }, "VSync", 40, this);
	btnControls = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 12, { 180, 600, 200, 60 }, "CONTROLS", 40, this);
	btnBack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 13, { 800, 600, 200, 60 }, "BACK", 40, this);

	//CONTROLS
	btnKeySelect = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 14, { 180, 120, 300, 60 }, "SELECT", 40, this);
	btnKeyCancel = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 15, { 180, 200, 300, 60 }, "CANCEL", 40, this);
	btnKeyMenu = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 16, { 180, 280, 300, 60 }, "MENU", 40, this);
	btnKeyUp = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 17, { 180, 360, 300, 60 }, "UP", 40, this);
	btnKeyDown = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 18, { 180, 440, 300, 60 }, "DOWN", 40, this);
	btnKeyLeft = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 19, { 180, 520, 300, 60 }, "LEFT", 40, this);
	btnKeyRight = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 20, { 180, 600, 300, 60 }, "RIGHT", 40, this);
	btnBack2 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 21, { 1280 / 2 - 200 / 2, 600, 200, 60 }, "BACK", 40, this);
	btnPadSelect = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 22, { 800, 120, 300, 60 }, "SELECT", 40, this);
	btnPadCancel = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 23, { 800, 200, 300, 60 }, "CANCEL", 40, this);
	btnPadMenu = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 24, { 800, 280, 300, 60 }, "MENU", 40, this);
	btnPadUp = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 25, { 800, 360, 300, 60 }, "UP", 40, this);
	btnPadDown = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 26, { 800, 440, 300, 60 }, "DOWN", 40, this);
	btnPadLeft = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 27, { 800, 520, 300, 60 }, "LEFT", 40, this);
	btnPadRight = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 28, { 800, 600, 300, 60 }, "RIGHT", 40, this);

	flags = 0;
}

SceneGameplay::~SceneGameplay()
{}

bool SceneGameplay::Load()
{
	app->entities->Enable();

	dialogueFont = new Font("Assets/Fonts/DialogueFont.xml");
	buttonFont = new Font("Assets/Fonts/ButtonFont.xml");

	// Initialize player
	player = app->entities->CreateEntity(-1, -1, EntityType::PLAYER, EntityId::NOT_COMBAT, NULL);

	app->audio->StopMusic();

	return false;
}

bool SceneGameplay::Update(float dt)
{
	// L02: DONE 3: Request Load / Save when pressing L/S
	//if (input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) app->LoadGameRequest();
	//if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) TransitionToScene(SceneType::DEV_ROOM);

	if ((flags & 1<<Flags::MENU) != 0 && (flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
	{
		btnInventory->Update(dt);
		btnSkills->Update(dt);
		btnSkillTree->Update(dt);
		btnEquipment->Update(dt);
		btnStats->Update(dt);
		btnOptions->Update(dt);
		btnTitleScreen->Update(dt);

		if ((flags & 1<<Flags::INVENTORY) != 0)
		{
			// arrow buttons maybe?
			// buttons for each item i supose?
		}
		else if((flags & 1 << Flags::SKILLS) != 0)
		{
			// arrow buttons maybe?
			// buttons for each skill i supose?
		}
		else if ((flags & 1 << Flags::SKILL_TREE) != 0)
		{
			// arrow buttons maybe?
			// buttons for each skill in the skill tree?
			// the skill tree lines change when you get the node?
		}
		else if ((flags & 1 << Flags::EQUIPMENT) != 0)
		{
			// buttons for every equipment??? ps: oh lord
		}
		else if ((flags & 1 << Flags::STATS) != 0)
		{
			// no buttons here? since its only the stats
		}
	}
	else if ((flags & 1 << Flags::OPTIONS) != 0 && (flags & 1 << Flags::CONTROLS) == 0)
	{
		sldrVolume->Update(dt);
		sldrFx->Update(dt);
		boxFullScreen->Update(dt);
		boxVSync->Update(dt);
		btnControls->Update(dt);
		btnBack->Update(dt);
	}
	else if ((flags & 1 << Flags::CONTROLS) != 0)
	{
		btnKeySelect->Update(dt);
		btnKeyCancel->Update(dt);
		btnKeyMenu->Update(dt);
		btnKeyUp->Update(dt);
		btnKeyDown->Update(dt);
		btnKeyLeft->Update(dt);
		btnKeyRight->Update(dt);
		btnBack2->Update(dt);
		btnPadSelect->Update(dt);
		btnPadCancel->Update(dt);
		btnPadMenu->Update(dt);
		btnPadUp->Update(dt);
		btnPadDown->Update(dt);
		btnPadLeft->Update(dt);
		btnPadRight->Update(dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->CheckButton("menu", KeyState::KEY_DOWN))
	{
		ResetButtons();
		flags = ToggleBit(flags, Flags::MENU);
	}
	LOG("%d", flags);

	return true;
}

bool SceneGameplay::Draw()
{
	app->render->background = { 0,0,150,255 };

	if ((flags & 1<<Flags::MENU) != 0 && (flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
	{
		app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);
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
	}
	else if ((flags & 1 << Flags::OPTIONS) != 0 && (flags & 1 << Flags::CONTROLS) == 0)
	{
		app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);
		SString titleOptions = "Options";
		app->render->DrawText(buttonFont, titleOptions.GetString(), app->render->camera.x + ((app->render->camera.w - (titleOptions.Length() * 24)) / 2), 100, 64, 2, { 255, 255, 255, 255 });
		sldrVolume->Draw();
		sldrFx->Draw();
		boxFullScreen->Draw();
		boxVSync->Draw();
		btnControls->Draw();
		btnBack->Draw();
	}
	else if ((flags & 1 << Flags::CONTROLS) != 0)
	{
		app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);
		SString titleControls = "Controls";
		app->render->DrawText(buttonFont, titleControls.GetString(), app->render->camera.x + ((app->render->camera.w - (titleControls.Length() * 24)) / 2), 100, 64, 2, { 255, 255, 255, 255 });
		btnKeySelect->Draw();
		btnKeyCancel->Draw();
		btnKeyMenu->Draw();
		btnKeyUp->Draw();
		btnKeyDown->Draw();
		btnKeyLeft->Draw();
		btnKeyRight->Draw();
		btnBack2->Draw();
		btnPadSelect->Draw();
		btnPadCancel->Draw();
		btnPadMenu->Draw();
		btnPadUp->Draw();
		btnPadDown->Draw();
		btnPadLeft->Draw();
		btnPadRight->Draw();
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
		flags = SetBit(flags, Flags::INVENTORY);
		break;
	case 2: //SKILLS
		ResetButtons();
		btnSkills->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::SKILLS);
		break;
	case 3: //SKILL TREE
		ResetButtons();
		btnSkillTree->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::SKILL_TREE);
		break;
	case 4: //EQUIPMENT
		ResetButtons();
		btnEquipment->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::EQUIPMENT);
		break;
	case 5: //STATS
		ResetButtons();
		btnStats->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::STATS);
		break;
	case 6: //OPTIONS
		ResetButtons();
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::OPTIONS);
		break;
	case 7: //TITLE SCREEN
		TransitionToScene(SceneType::TITLE_SCREEN);
		break;
	case 8: //VOLUME
		app->audio->SetMusicVolume(sldrVolume->value);
		break;
	case 9: //FX
		app->audio->SetFxVolumeValue(sldrFx->value);
		break;
	case 10: //FULLSCREEN
		app->win->ToggleFullscreen(boxFullScreen->checked);
		break;
	case 11: //VSYNC
		//app->win->ToggleFullscreen(false);
		//app->render->ToggleVsync(boxVSync->checked, (Module*)this);
		break;
	case 12: //CONTROLS
		flags = SetBit(flags, Flags::CONTROLS);
		break;
	case 13: //BACK
		flags = ClearBit(flags, Flags::OPTIONS);
		break;
	case 14: //KEY SELECT
		break;
	case 15: //KEY CANCEL
		break;
	case 16: //KEY MENU
		break;
	case 17: //KEY UP
		break;
	case 18: //KEY DOWN
		break;
	case 19: //KEY LEFT
		break;
	case 20: //KEY RIGHT
		break;
	case 21: //BACK 2
		flags = ClearBit(flags, Flags::CONTROLS);
		break;
	case 22: //PAD SELECT
		break;
	case 23: //PAD CANCEL
		break;
	case 24: //PAD MENU
		break;
	case 25: //PAD UP
		break;
	case 26: //PAD DOWN
		break;
	case 27: //PAD LEFT
		break;
	case 28: //PAD RIGHT
		break;
	default:
		break;
	}

	return true;
}
