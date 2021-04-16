#include "SceneGameplay.h"
#include "SceneCombat.h"

#include "Audio.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "CombatEntity.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Map.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "DialogSystem.h"
#include "DialogUtils.h"

SceneGameplay::SceneGameplay()
{
	//COMBAT
	combatScene = new SceneCombat();
}

SceneGameplay::~SceneGameplay()
{}

bool SceneGameplay::Load()
{
	app->collisions->Enable();
	app->entities->Enable();

	dialogSystem = new DialogSystem();

	dialogueFont = new Font("Assets/Fonts/DialogueFont.xml");
	buttonFont = new Font("Assets/Fonts/ButtonFont.xml");

	textBox = app->tex->Load("Assets/Textures/GUI/textBox.png");

	float tmpValue = 0;
	//MENU
	app->gui->Enable();
	menuBox = { 324,0,692,540 };
	menuCharacterBox = { 324,539,204,135 };
	btnInventory = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 1, { 90, 80, 200, 60 }, "INVENTORY", 35, this);
	btnSkills = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 2, { 90, 160, 200, 60 }, "SKILLS", 40, this);
	btnSkillTree = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 3, { 90, 240, 200, 60 }, "       SKILL TREE", 35, this);
	btnEquipment = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 4, { 90, 320, 200, 60 }, " EQUIPMENT", 35, this);
	btnStats = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 5, { 90, 400, 200, 60 }, " STATS ", 40, this);
	btnOptions = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 6, { 90, 480, 200, 60 }, "OPTIONS", 35, this);
	btnTitleScreen = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 7, { 90, 560, 200, 60 }, "          TITLE SCREEN", 35, this);

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
	btnControls = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 12, { 180, 600, 200, 60 }, "CONTROLS", 35, this);
	btnBack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 13, { 800, 600, 200, 60 }, "BACK", 40, this);

	//CONTROLS
	btnKeySelect = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 14, { 180, 120, 300, 60 }, "SELECT", 40, this);
	btnKeyCancel = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 15, { 180, 200, 300, 60 }, "CANCEL", 40, this);
	btnKeyMenu = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 16, { 180, 280, 300, 60 }, "MENU", 40, this);
	btnKeyUp = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 17, { 180, 360, 300, 60 }, "UP", 40, this);
	btnKeyDown = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 18, { 180, 440, 300, 60 }, "DOWN", 40, this);
	btnKeyLeft = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 19, { 180, 520, 300, 60 }, "LEFT", 40, this);
	btnKeyRight = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 20, { 180, 600, 300, 60 }, "RIGHT", 40, this);
	btnPadSelect = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 21, { 800, 120, 300, 60 }, "SELECT", 40, this);
	btnPadCancel = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 22, { 800, 200, 300, 60 }, "CANCEL", 40, this);
	btnPadMenu = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 23, { 800, 280, 300, 60 }, "MENU", 40, this);
	btnPadUp = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 24, { 800, 360, 300, 60 }, "UP", 40, this);
	btnPadDown = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 25, { 800, 440, 300, 60 }, "DOWN", 40, this);
	btnPadLeft = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 26, { 800, 520, 300, 60 }, "LEFT", 40, this);
	btnPadRight = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 27, { 800, 600, 300, 60 }, "RIGHT", 40, this);
	btnBack2 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 28, { 1280 / 2 - 200 / 2, 600, 200, 60 }, "BACK", 40, this);

	app->map->Enable();
	app->map->LoadNewMap("tutorial.tmx");
	// Initialize player
	player = app->entities->CreateEntity(-1, -1, EntityType::PLAYER, EntityId::NOT_COMBAT, NULL);

	hero = nullptr;
	shopDude = nullptr;
	cat = nullptr;

	// COMBAT
	combatScene->Load();

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->audio->PlayMusic("Assets/Audio/Music/Tutorial.ogg", 0.0f);

	// Used for the Gamepad GUI control
	app->scene->currentButton = app->gui->controls.start;
	changeMenu = false;
	enteringCombat = false;
	if (app->input->GetControllerName() != "unplugged") usingGamepad = true;
	app->input->mouseMotionX = 0;
	app->input->mouseMotionY = 0;
	flags = 0;
	inMenu = false;

	if (app->scene->continueLoadRequest) app->LoadRequest();

	return false;
}

bool SceneGameplay::Update(float dt)
{
	// L02: DONE 3: Request Load / Save when pressing L/S
	//if (input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) app->LoadGameRequest();
	//if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) app->SaveGameRequest();

	// So player doesnt move while in dialog
	Player* tmp = (Player*)player;

	if (dialogSystem->currentDialog != nullptr)
	{
		tmp->inDialog = true;
	}
	else
	{
		tmp->inDialog = false;
	}

	/* ONLY FOR TESTING */

	if (app->input->GetKey(SDL_SCANCODE_K) == KeyState::KEY_DOWN) {
		dialogSystem->StartDialog("1");
	}

	if (app->input->GetKey(SDL_SCANCODE_L) == KeyState::KEY_DOWN) {
		dialogSystem->StartDialog("2");
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

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) TransitionToScene(SceneType::DEV_ROOM);
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		combat = true;
		enteringCombat = true;
	}


	if (combat)
	{
		if (enteringCombat)
		{
			enteringCombat = false;
			combatScene->Start();
		}
		combatScene->Update(dt);
	}
	else
	{
		UpdatePauseMenu(dt);
		//Update normal scene
	}

	if (combatScene->heDed) TransitionToScene(SceneType::TITLE_SCREEN);

	return true;
}

bool SceneGameplay::UpdatePauseMenu(float dt)
{
	// Logic for using Gamepad or mouse (GUI)
	ListItem<InputButton*>* gamepadControls = app->input->controlConfig.start;
	while (gamepadControls->next != nullptr)
	{
		if (app->input->GetPadKey(gamepadControls->data->gamePadId) == KEY_DOWN)
		{
			usingGamepad = true;
			break;
		}
		gamepadControls = gamepadControls->next;
	}
	int tmpX = 0, tmpY = 0;
	app->input->GetMouseMotion(tmpX, tmpY);
	if (((tmpX > 3 || tmpX < -3) || (tmpY > 3 || tmpY < -3)) || (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN))
		usingGamepad = false;

	// Calls update with gamepad parameters (GUI)
	if (usingGamepad)
	{
		if ((flags & 1 << Flags::MENU) != 0 && (flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
		{
			if (changeMenu)
			{
				app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnInventory));
				changeMenu = false;
			}
			app->scene->currentButton->data->Update(dt, 1, 7);
		}
		else if ((flags & 1 << Flags::OPTIONS) != 0 && (flags & 1 << Flags::CONTROLS) == 0)
		{
			if (changeMenu)
			{
				app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(sldrVolume));
				changeMenu = false;
			}
			app->scene->currentButton->data->Update(dt, 8, 13);
		}
		else if ((flags & 1 << Flags::CONTROLS) != 0)
		{
			if (changeMenu)
			{
				app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnKeySelect));
				changeMenu = false;
			}
			app->scene->currentButton->data->Update(dt, 14, 28);
		}
	}
	// Calls update for mouse parameters (GUI)
	else
	{
		if ((flags & 1 << Flags::MENU) != 0 && (flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
		{
			btnInventory->Update(dt);
			btnSkills->Update(dt);
			btnSkillTree->Update(dt);
			btnEquipment->Update(dt);
			btnStats->Update(dt);
			btnOptions->Update(dt);
			btnTitleScreen->Update(dt);

			if ((flags & 1 << Flags::INVENTORY) != 0)
			{
				// arrow buttons maybe?
				// buttons for each item i supose?
			}
			else if ((flags & 1 << Flags::SKILLS) != 0)
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
	}
	

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->CheckButton("menu", KeyState::KEY_DOWN))
	{
		// Logic to manage pause menu and menu flags bit
		if ((flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
		{
			app->gui->ResetButtons();
			flags = ToggleBit(flags, Flags::MENU);
			app->entities->inPause = !app->entities->inPause;
		}

		// Volume between pause menu and gameplay logic
		if (app->entities->inPause && (flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
		{
			app->audio->auxVolume = app->audio->GetMusicVolume();
			app->audio->SetMusicVolume(app->audio->GetMusicVolume() / 3);
		}
		else if ((flags & 1 << Flags::OPTIONS) != 0 || (flags & 1 << Flags::CONTROLS) != 0);
		else
		{
			app->audio->SetMusicVolume(app->audio->auxVolume);
		}
	}

	return true;
}

bool SceneGameplay::Draw()
{
	if (combat) combatScene->Draw(dialogueFont);
	else
	{
		app->render->background = { 0,0,0,255 };
		app->map->Draw();
	}

	if (dialogSystem->currentDialog != nullptr)
	{
		dialogSystem->DrawDialog(dialogueFont);
	}

	return false;
}

bool SceneGameplay::DrawPauseMenu()
{
	if ((flags & 1 << Flags::MENU) != 0 && (flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
	{
		app->render->DrawRectangle({ -app->render->camera.x,-app->render->camera.y,app->render->camera.w,app->render->camera.h }, 0, 0, 0, 200);
		btnInventory->Draw(-app->render->camera.x, -app->render->camera.y);
		btnSkills->Draw(-app->render->camera.x, -app->render->camera.y);
		btnSkillTree->Draw(-app->render->camera.x, -app->render->camera.y);
		btnEquipment->Draw(-app->render->camera.x, -app->render->camera.y);
		btnStats->Draw(-app->render->camera.x, -app->render->camera.y);
		btnOptions->Draw(-app->render->camera.x, -app->render->camera.y);
		btnTitleScreen->Draw(-app->render->camera.x, -app->render->camera.y);

		if ((flags & 1 << Flags::INVENTORY) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 292, -app->render->camera.y + 80, false, &menuBox);
		}
		else if ((flags & 1 << Flags::SKILLS) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 292, -app->render->camera.y + 80, false, &menuBox);
		}
		else if ((flags & 1 << Flags::SKILL_TREE) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 292, -app->render->camera.y + 80, false, &menuBox);
		}
		else if ((flags & 1 << Flags::EQUIPMENT) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 292, -app->render->camera.y + 80, false, &menuBox);
		}
		else if ((flags & 1 << Flags::STATS) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 292, -app->render->camera.y + 80, false, &menuBox);
		}

		// Character Boxes
		if (combatScene->characterFlags >= 2)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 984, -app->render->camera.y + 80, false, &menuCharacterBox);
			app->render->DrawTexture(combatScene->combatGui, -app->render->camera.x + 984 + 10, -app->render->camera.y + 80 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), false, &combatScene->mainChar.characterTex);
			app->render->DrawText(dialogueFont, combatScene->mainChar.hp.GetString(), /*-app->render->camera.x +*/ 984 + combatScene->mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ 80 + 20, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, combatScene->mainChar.mp.GetString(), /*-app->render->camera.x + */984 + combatScene->mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ 80 + 20 + 30, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, combatScene->mainChar.stress.GetString(), /*-app->render->camera.x +*/ 984 + combatScene->mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ 80 + 20 + 60, 28, 1, { 255,255,255,255 });
		}
		if (combatScene->characterFlags >= 6)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 984, -app->render->camera.y + menuCharacterBox.h + 80, false, &menuCharacterBox);
			app->render->DrawTexture(combatScene->combatGui, -app->render->camera.x + 984 + 10, -app->render->camera.y + menuCharacterBox.h + 80 + (menuCharacterBox.h / 2 - combatScene->grandpa.characterTex.h / 2), false, &combatScene->grandpa.characterTex);
			app->render->DrawText(dialogueFont, combatScene->grandpa.hp.GetString(),/* -app->render->camera.x +*/ 984 + combatScene->grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ menuCharacterBox.h + 80 + 20, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, combatScene->grandpa.mp.GetString(),/* -app->render->camera.x +*/ 984 + combatScene->grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ menuCharacterBox.h + 80 + 20 + 30, 28, 1, { 255,255,255,255 });
		}
		/*
		if (characterFlags >= 7)
		{
			//3rd
		}
		if (characterFlags >= 15)
		{
			//4th
		}
		*/
	}
	else if ((flags & 1 << Flags::OPTIONS) != 0 && (flags & 1 << Flags::CONTROLS) == 0)
	{
		app->render->DrawRectangle({ -app->render->camera.x,-app->render->camera.y,app->render->camera.w,app->render->camera.h }, 0, 0, 0, 200);
		SString titleOptions = "Options";
		app->render->DrawText(buttonFont, titleOptions.GetString(), /*-app->render->camera.x +*/ ((app->render->camera.w - (titleOptions.Length() * 24)) / 2), 100, 64, 2, { 255, 255, 255, 255 });
		sldrVolume->Draw(-app->render->camera.x, -app->render->camera.y);
		sldrFx->Draw(-app->render->camera.x, -app->render->camera.y);
		if (app->scene->fullscreenChecked) boxFullScreen->checked = true;
		boxFullScreen->Draw(-app->render->camera.x, -app->render->camera.y);
		boxVSync->Draw(-app->render->camera.x, -app->render->camera.y);
		btnControls->Draw(-app->render->camera.x, -app->render->camera.y);
		btnBack->Draw(-app->render->camera.x, -app->render->camera.y);
	}
	else if ((flags & 1 << Flags::CONTROLS) != 0)
	{
		app->render->DrawRectangle({ -app->render->camera.x,-app->render->camera.y,app->render->camera.w,app->render->camera.h }, 0, 0, 0, 200);
		SString titleControls = "Controls";
		app->render->DrawText(buttonFont, titleControls.GetString(), /*-app->render->camera.x + */((-app->render->camera.w - (titleControls.Length() * 24)) / 2), 100, 64, 2, { 255, 255, 255, 255 });
		btnKeySelect->Draw(-app->render->camera.x, -app->render->camera.y);
		btnKeyCancel->Draw(-app->render->camera.x, -app->render->camera.y);
		btnKeyMenu->Draw(-app->render->camera.x, -app->render->camera.y);
		btnKeyUp->Draw(-app->render->camera.x, -app->render->camera.y);
		btnKeyDown->Draw(-app->render->camera.x, -app->render->camera.y);
		btnKeyLeft->Draw(-app->render->camera.x, -app->render->camera.y);
		btnKeyRight->Draw(-app->render->camera.x, -app->render->camera.y);
		btnBack2->Draw(-app->render->camera.x, -app->render->camera.y);
		btnPadSelect->Draw(-app->render->camera.x, -app->render->camera.y);
		btnPadCancel->Draw(-app->render->camera.x, -app->render->camera.y);
		btnPadMenu->Draw(-app->render->camera.x, -app->render->camera.y);
		btnPadUp->Draw(-app->render->camera.x, -app->render->camera.y);
		btnPadDown->Draw(-app->render->camera.x, -app->render->camera.y);
		btnPadLeft->Draw(-app->render->camera.x, -app->render->camera.y);
		btnPadRight->Draw(-app->render->camera.x, -app->render->camera.y);
	}

	return true;
}

bool SceneGameplay::Unload()
{
	// TODO: Unload all resources
	if (combatScene != nullptr)
	{
		combatScene->Unload();
		combatScene = nullptr;
	}
	app->entities->Disable();
	app->collisions->Disable();
	app->map->Disable();

	if (buttonFont != nullptr) delete buttonFont;
	if (dialogueFont != nullptr) delete dialogueFont;
	if (textBox != nullptr) app->tex->UnLoad(textBox);
	if (dialogueFont != nullptr) delete dialogueFont;

	app->gui->Disable();
	if (app->scene->currentButton != nullptr) app->scene->currentButton = nullptr;

	return false;
}


//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneGameplay::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	//MENU
	case 1: //INVENTORY
		app->gui->ResetButtons();
		btnInventory->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::INVENTORY);
		break;
	case 2: //SKILLS
		app->gui->ResetButtons();
		btnSkills->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::SKILLS);
		break;
	case 3: //SKILL TREE
		app->gui->ResetButtons();
		btnSkillTree->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::SKILL_TREE);
		break;
	case 4: //EQUIPMENT
		app->gui->ResetButtons();
		btnEquipment->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::EQUIPMENT);
		break;
	case 5: //STATS
		app->gui->ResetButtons();
		btnStats->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::STATS);
		break;
	case 6: //OPTIONS
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::OPTIONS);

		changeMenu = true;
		app->gui->ResetButtons();
		usingGamepad = true;

		break;
	case 7: //TITLE SCREEN
		app->entities->inPause = false;
		TransitionToScene(SceneType::TITLE_SCREEN);
		break;
	case 8: //VOLUME
		app->audio->SetMusicVolume(sldrVolume->value / 2);
		app->audio->auxVolume = sldrVolume->value;
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

	//CONTROLS
	case 12: //CONTROLS
		flags = SetBit(flags, Flags::CONTROLS);

		changeMenu = true;
		app->gui->ResetButtons();
		usingGamepad = true;
		break;
	case 13: //BACK (OPTIONS BACK)
		flags = ClearBit(flags, Flags::OPTIONS);

		changeMenu = true;
		app->gui->ResetButtons();
		usingGamepad = true;
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
	case 21: //PAD SELECT
		break;
	case 22: //PAD CANCEL
		break;
	case 23: //PAD MENU
		break;
	case 24: //PAD UP
		break;
	case 25: //PAD DOWN
		break;
	case 26: //PAD LEFT
		break;
	case 27: //PAD RIGHT
		break;
	case 28: //BACK 2 (CONTROLS BACK)
		flags = ClearBit(flags, Flags::CONTROLS);

		changeMenu = true;
		app->gui->ResetButtons();
		usingGamepad = true;

		break;
	default:
		break;
	}
	return true;
}
