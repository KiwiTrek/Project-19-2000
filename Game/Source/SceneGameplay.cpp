#include "SceneGameplay.h"

#include "Audio.h"
#include "EntityManager.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"

SceneGameplay::SceneGameplay()
{
	float tmpValue = 0;

	//MENU
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
	btnBack2 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 21, { 1280 / 2 - 200 / 2, 600, 200, 60 }, "BACK", 40, this);
	btnPadSelect = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 22, { 800, 120, 300, 60 }, "SELECT", 40, this);
	btnPadCancel = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 23, { 800, 200, 300, 60 }, "CANCEL", 40, this);
	btnPadMenu = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 24, { 800, 280, 300, 60 }, "MENU", 40, this);
	btnPadUp = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 25, { 800, 360, 300, 60 }, "UP", 40, this);
	btnPadDown = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 26, { 800, 440, 300, 60 }, "DOWN", 40, this);
	btnPadLeft = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 27, { 800, 520, 300, 60 }, "LEFT", 40, this);
	btnPadRight = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 28, { 800, 600, 300, 60 }, "RIGHT", 40, this);

	//COMBAT
	combatTextBox = { 0,0,1280,248 };
	combatMenuBox = { 305,249,1001,130 };
	btnCombatAttack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 29, { 34,505,200,60 }, "ATTACK", 40, this);
	btnCombatSkills = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 30, { 234,505,200,60 }, "SKILLS", 40, this);
	btnCombatItems = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 31, { 434,505,200,60 }, "ITEMS", 40, this);
	btnCombatSpecial = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 32, { 634,505,200,60 }, "SPECIAL", 40, this);
	btnCombatFlee = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 33, { 834,505,200,60 }, "FLEE", 40, this);

	flags = 0;
}

SceneGameplay::~SceneGameplay()
{}

bool SceneGameplay::Load()
{
	app->entities->Enable();

	dialogueFont = new Font("Assets/Fonts/DialogueFont.xml");
	buttonFont = new Font("Assets/Fonts/ButtonFont.xml");

	textBox = app->tex->Load("Assets/Textures/GUI/textBox.png");
	combatGui = app->tex->Load("Assets/Textures/GUI/combatGui.png");

	// Initialize player
	player = app->entities->CreateEntity(-1, -1, EntityType::PLAYER, EntityId::NOT_COMBAT, NULL);

	// COMBAT
	combatMenuFlags = 0;
	int tmp = 25;

	characterFlags = 0;
	characterFlags = SetBit(characterFlags, (uint)EntityId::MC);
	currentChar = &mainChar;
	mainChar.box = { 1280,0,204,190 };
	mainChar.character = app->entities->CreateEntity(36, app->render->camera.h - mainChar.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::MC, Stats(50, 50, 50, 50, 50, 0, 1.0f, 100));
	mainChar.characterTex = { 0,252,72,92 };
	mainChar.hp.Create("HP: %d/%d", tmp, mainChar.character->stats.hPoints);
	mainChar.mp.Create("ST: %d/%d", tmp, mainChar.character->stats.stress);

	characterFlags = SetBit(characterFlags, (uint)EntityId::VIOLENT);
	grandpa.box = { 1280,0,204,190 };
	grandpa.character = app->entities->CreateEntity(grandpa.box.w + 36, app->render->camera.h - grandpa.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::VIOLENT, Stats(70, 70, 30, 30, 40, 50, 0.5f, 0));
	grandpa.characterTex = { 73,252,68,100 };
	grandpa.hp.Create("HP: %d/%d", tmp, grandpa.character->stats.hPoints);
	grandpa.mp.Create("MP: %d/%d", tmp, grandpa.character->stats.mPoints);

	LOG("%d", characterFlags);

	app->audio->PlayMusic("Assets/Audio/Music/Tutorial.ogg", 0.0f);

	return false;
}

bool SceneGameplay::Update(float dt)
{
	// L02: DONE 3: Request Load / Save when pressing L/S
	//if (input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) app->LoadGameRequest();
	//if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) TransitionToScene(SceneType::DEV_ROOM);
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) combat = !combat;

	if (combat) UpdateCombat(dt);
	else
	{
		UpdatePauseMenu(dt);
		//Update normal scene
	}

	return true;
}

bool SceneGameplay::UpdatePauseMenu(float dt)
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

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->CheckButton("menu", KeyState::KEY_DOWN))
	{
		ResetButtons();
		flags = ToggleBit(flags, Flags::MENU);
	}
	return true;
}

bool SceneGameplay::UpdateCombat(float dt)
{
	//What you're doing in the Dev Room should go here
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) characterSelected = !characterSelected;
	if (characterSelected)
	{
		btnCombatAttack->Update(dt);
		btnCombatSkills->Update(dt);
		btnCombatItems->Update(dt);
		btnCombatSpecial->Update(dt);
		btnCombatFlee->Update(dt);
	}
	return true;
}

bool SceneGameplay::Draw()
{
	if (combat) DrawCombat();
	else
	{
		app->render->background = { 0,0,150,255 };
	}

	return false;
}

bool SceneGameplay::DrawPauseMenu()
{
	if ((flags & 1 << Flags::MENU) != 0 && (flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
	{
		app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);
		btnInventory->Draw(app->render->camera.x, app->render->camera.y);
		btnSkills->Draw(app->render->camera.x, app->render->camera.y);
		btnSkillTree->Draw(app->render->camera.x, app->render->camera.y);
		btnEquipment->Draw(app->render->camera.x, app->render->camera.y);
		btnStats->Draw(app->render->camera.x, app->render->camera.y);
		btnOptions->Draw(app->render->camera.x, app->render->camera.y);
		btnTitleScreen->Draw(app->render->camera.x, app->render->camera.y);

		if ((flags & 1 << Flags::INVENTORY) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, app->render->camera.x + 292, app->render->camera.y + 80, false, &menuBox);
		}
		else if ((flags & 1 << Flags::SKILLS) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, app->render->camera.x + 292, app->render->camera.y + 80, false, &menuBox);
		}
		else if ((flags & 1 << Flags::SKILL_TREE) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, app->render->camera.x + 292, app->render->camera.y + 80, false, &menuBox);
		}
		else if ((flags & 1 << Flags::EQUIPMENT) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, app->render->camera.x + 292, app->render->camera.y + 80, false, &menuBox);
		}
		else if ((flags & 1 << Flags::STATS) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, app->render->camera.x + 292, app->render->camera.y + 80, false, &menuBox);
		}

		// Character Boxes
		if (characterFlags >= 1)
		{
			app->render->DrawTexture(app->gui->atlas, app->render->camera.x + 984, app->render->camera.y + 80, false, &menuCharacterBox);
			app->render->DrawTexture(combatGui, app->render->camera.x + 984 + 10, app->render->camera.y + 80 + 20, false, &mainChar.characterTex);
			app->render->DrawText(dialogueFont, mainChar.hp.GetString(), app->render->camera.x + 984 + mainChar.characterTex.w + 15, app->render->camera.y + 80 + 20, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, mainChar.mp.GetString(), app->render->camera.x + 984 + mainChar.characterTex.w + 15, app->render->camera.y + 80 + 20 + 30, 28, 1, { 255,255,255,255 });
		}
		if (characterFlags >= 3)
		{
			app->render->DrawTexture(app->gui->atlas, app->render->camera.x + 984, app->render->camera.y + menuCharacterBox.h + 80, false, &menuCharacterBox);
			app->render->DrawTexture(combatGui, app->render->camera.x + 984 + 10, app->render->camera.y + menuCharacterBox.h + 80 + 20, false, &grandpa.characterTex);
			app->render->DrawText(dialogueFont, grandpa.hp.GetString(), app->render->camera.x + 984 + grandpa.characterTex.w + 15, app->render->camera.y + menuCharacterBox.h + 80 + 20, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, grandpa.mp.GetString(), app->render->camera.x + 984 + grandpa.characterTex.w + 15, app->render->camera.y + menuCharacterBox.h + 80 + 20 + 30, 28, 1, { 255,255,255,255 });
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
		app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);
		SString titleOptions = "Options";
		app->render->DrawText(buttonFont, titleOptions.GetString(), app->render->camera.x + ((app->render->camera.w - (titleOptions.Length() * 24)) / 2), 100, 64, 2, { 255, 255, 255, 255 });
		sldrVolume->Draw(app->render->camera.x, app->render->camera.y);
		sldrFx->Draw(app->render->camera.x, app->render->camera.y);
		boxFullScreen->Draw(app->render->camera.x, app->render->camera.y);
		boxVSync->Draw(app->render->camera.x, app->render->camera.y);
		btnControls->Draw(app->render->camera.x, app->render->camera.y);
		btnBack->Draw(app->render->camera.x, app->render->camera.y);
	}
	else if ((flags & 1 << Flags::CONTROLS) != 0)
	{
		app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);
		SString titleControls = "Controls";
		app->render->DrawText(buttonFont, titleControls.GetString(), app->render->camera.x + ((app->render->camera.w - (titleControls.Length() * 24)) / 2), 100, 64, 2, { 255, 255, 255, 255 });
		btnKeySelect->Draw(app->render->camera.x, app->render->camera.y);
		btnKeyCancel->Draw(app->render->camera.x, app->render->camera.y);
		btnKeyMenu->Draw(app->render->camera.x, app->render->camera.y);
		btnKeyUp->Draw(app->render->camera.x, app->render->camera.y);
		btnKeyDown->Draw(app->render->camera.x, app->render->camera.y);
		btnKeyLeft->Draw(app->render->camera.x, app->render->camera.y);
		btnKeyRight->Draw(app->render->camera.x, app->render->camera.y);
		btnBack2->Draw(app->render->camera.x, app->render->camera.y);
		btnPadSelect->Draw(app->render->camera.x, app->render->camera.y);
		btnPadCancel->Draw(app->render->camera.x, app->render->camera.y);
		btnPadMenu->Draw(app->render->camera.x, app->render->camera.y);
		btnPadUp->Draw(app->render->camera.x, app->render->camera.y);
		btnPadDown->Draw(app->render->camera.x, app->render->camera.y);
		btnPadLeft->Draw(app->render->camera.x, app->render->camera.y);
		btnPadRight->Draw(app->render->camera.x, app->render->camera.y);
	}

	return true;
}

bool SceneGameplay::DrawCombat()
{
	//switch (ZONE)
	app->render->background = { 125,33,129,255 };
	app->render->DrawTexture(combatGui, app->render->camera.x, app->render->camera.y, false, &combatTextBox);
	app->render->DrawTexture(combatGui, app->render->camera.x, app->render->camera.y + app->render->camera.h - combatTextBox.h, false, &combatTextBox);
	//app->render->DrawRectangle({ 1280 / 2 - 64,720 / 2 - 64,128,128 }, 0, 255, 255, 255);
	if (!characterSelected)
	{
		if (characterFlags >= 1)
		{
			app->render->DrawTexture(combatGui, app->render->camera.x + mainChar.character->entityRect.x, app->render->camera.y + mainChar.character->entityRect.y, false, &mainChar.box);
			app->render->DrawTexture(combatGui, app->render->camera.x + mainChar.character->entityRect.x + (mainChar.box.w/2 - mainChar.characterTex.w/2), app->render->camera.y + mainChar.character->entityRect.y + 25, false, &mainChar.characterTex);
			app->render->DrawText(dialogueFont, mainChar.hp.GetString(), app->render->camera.x + mainChar.character->entityRect.x + 40, app->render->camera.y + mainChar.character->entityRect.y + 25 + mainChar.characterTex.h + 1, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, mainChar.mp.GetString(), app->render->camera.x + mainChar.character->entityRect.x + 40, app->render->camera.y + mainChar.character->entityRect.y + 25 + mainChar.characterTex.h + 29, 28, 1, { 255,255,255,255 });
		}
		if (characterFlags >= 3)
		{
			app->render->DrawTexture(combatGui, app->render->camera.x + grandpa.character->entityRect.x, app->render->camera.y + grandpa.character->entityRect.y, false, &grandpa.box);
			app->render->DrawTexture(combatGui, app->render->camera.x + grandpa.character->entityRect.x + (grandpa.box.w / 2 - grandpa.characterTex.w / 2), app->render->camera.y + grandpa.character->entityRect.y + 25, false, &grandpa.characterTex);
			app->render->DrawText(dialogueFont, grandpa.hp.GetString(), app->render->camera.x + grandpa.character->entityRect.x + 40, app->render->camera.y + grandpa.character->entityRect.y + 25 + grandpa.characterTex.h - 8, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, grandpa.mp.GetString(), app->render->camera.x + grandpa.character->entityRect.x + 40, app->render->camera.y + grandpa.character->entityRect.y + 25 + grandpa.characterTex.h + 20, 28, 1, { 255,255,255,255 });
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
	else
	{
		app->render->DrawTexture(combatGui, app->render->camera.x + app->render->camera.w - currentChar->box.w - 34, app->render->camera.y + app->render->camera.h - currentChar->box.h - 25, false, &currentChar->box);
		app->render->DrawTexture(combatGui, app->render->camera.x + app->render->camera.w - currentChar->box.w - 34 + (currentChar->box.w / 2 - currentChar->characterTex.w / 2), app->render->camera.y + app->render->camera.h - currentChar->box.h, false, &currentChar->characterTex);
		app->render->DrawText(dialogueFont, currentChar->hp.GetString(), app->render->camera.x + app->render->camera.w - currentChar->box.w + 14, app->render->camera.y + app->render->camera.h - currentChar->box.h + currentChar->characterTex.h - 6, 28, 1, { 255,255,255,255 });
		app->render->DrawText(dialogueFont, currentChar->mp.GetString(), app->render->camera.x + app->render->camera.w - currentChar->box.w + 14, app->render->camera.y + app->render->camera.h - currentChar->box.h + currentChar->characterTex.h + 22, 28, 1, { 255,255,255,255 });

		btnCombatAttack->Draw(app->render->camera.x, app->render->camera.y);
		btnCombatSkills->Draw(app->render->camera.x, app->render->camera.y);
		btnCombatItems->Draw(app->render->camera.x, app->render->camera.y);
		btnCombatSpecial->Draw(app->render->camera.x, app->render->camera.y);
		btnCombatFlee->Draw(app->render->camera.x, app->render->camera.y);

		if ((combatMenuFlags & 1 << CombatFlags::SKILL) != 0)
		{
			app->render->DrawTexture(combatGui, app->render->camera.x + 36, app->render->camera.y + app->render->camera.h - combatMenuBox.h - 25, false, &combatMenuBox);
		}
		else if ((combatMenuFlags & 1 << CombatFlags::ITEMS) != 0)
		{
			app->render->DrawTexture(combatGui, app->render->camera.x + 36, app->render->camera.y + app->render->camera.h - combatMenuBox.h - 25, false, &combatMenuBox);
		}
		else if ((combatMenuFlags & 1 << CombatFlags::SPECIAL) != 0)
		{
			app->render->DrawTexture(combatGui, app->render->camera.x + 36, app->render->camera.y + app->render->camera.h - combatMenuBox.h - 25, false, &combatMenuBox);
		}
	}
	return true;
}

bool SceneGameplay::Unload()
{
	// TODO: Unload all resources
	app->entities->Disable();

	return false;
}

void SceneGameplay::ResetButtons()
{
	if (combat)
	{
		btnCombatAttack->state = GuiControlState::NORMAL;
		btnCombatSkills->state = GuiControlState::NORMAL;
		btnCombatItems->state = GuiControlState::NORMAL;
		btnCombatSpecial->state = GuiControlState::NORMAL;
		btnCombatFlee->state = GuiControlState::NORMAL;
	}
	else
	{
		btnInventory->state = GuiControlState::NORMAL;
		btnEquipment->state = GuiControlState::NORMAL;
		btnSkills->state = GuiControlState::NORMAL;
		btnSkillTree->state = GuiControlState::NORMAL;
		btnEquipment->state = GuiControlState::NORMAL;
		btnStats->state = GuiControlState::NORMAL;
		btnOptions->state = GuiControlState::NORMAL;
	}
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
		ResetButtons();
		btnInventory->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
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

	//COMBAT
	case 29: //ATTACK
		ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		break;
	case 30: //SKILLS
		ResetButtons();
		btnCombatSkills->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, CombatFlags::SKILL);
		break;
	case 31: //ITEMS
		ResetButtons();
		btnCombatItems->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, CombatFlags::ITEMS);
		break;
	case 32: //SPECIAL
		ResetButtons();
		btnCombatSpecial->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, CombatFlags::SPECIAL);
		break;
	case 33: //FLEE
		ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		break;
	default:
		break;
	}

	return true;
}
