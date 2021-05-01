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
	white = { 255,255,255,255 };
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
	// Saving dialog thingies
	savingText = { "Game saved succsessfully." };
	savingBool = false;
	savingCounter = 0;

	textBox = app->tex->Load("Assets/Textures/GUI/textBox.png");
	itemsTex = app->tex->Load("Assets/Textures/GUI/Items.png");

	dialogGui = app->tex->Load("Assets/Textures/GUI/combatGui.png");
	dialogTextBox = { 0,0,1280,248 };
	portraitBox = { 1276,0,208,190 };
	shopKeeperPortrait = { 0,355,72,93 };
	catPortrait = { 78,391,78,52 };
	superheroPortrait = { 1324,351,72,95 };
	grandpaPortrait = { 74,248,68,100 };
	hatsunePortrait = { 1400,346,84,100 };

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
	statFlags = 0;

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

	//ITEMS
	SDL_Rect sec = { 0,0,32,32 };
	btnItem1 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 29, { 330,120,300,60 }, "ITEM HEAL", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 32,0,32,32 };
	btnItem2 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 30, { 330,200,300,60 }, "ITEM", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 64,0,32,32 };
	btnItem3 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 31, { 330,280,300,60 }, "ITEMVERYLONG", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 96,0,32,32 };
	btnItem4 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 32, { 330,360,300,60 }, "ITEMMMMMMMMM", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 128,0,32,32 };
	btnItem5 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 33, { 330,440,300,60 }, "item", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 160,0,32,32 };
	btnItem6 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 34, { 330,520,300,60 }, "FLASH BANG!", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 192,0,32,32 };
	btnItem7 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 35, { 646,120,300,60 }, "POISON HEAL", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 224,0,32,32 };
	btnItem8 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 36, { 646,200,300,60 }, "STRESS HEAL", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 256,0,32,32 };
	btnItem9 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 37, { 646,280,300,60 }, "STUN HEAL", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 288,0,32,32 };
	btnItem10 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 38, { 646,360,300,60 }, "MANA HEAL", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 0,32,32,32 };
	btnItem11 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 39, { 646,440,300,60 }, "ELIXIR", 40, this, 0, "x0", itemsTex, &sec);
	sec = { 32,32,32,32 };
	btnItem12 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 40, { 646,520,300,60 }, "TACTICAL NUKE", 40, this, 0, "x0", itemsTex, &sec);

	SString str = "item one";
	Attack* eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	int c = 2;
	Item* i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item two";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 1;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item three";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 3;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item four";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 3;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item five";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 3;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item six";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 3;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item seven";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 3;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item eight";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 3;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item nine";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 3;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item ten";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 3;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item eleven";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 3;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	str = "item twelve";
	eff = new Attack(str, AttackType::HEAL, TargetType::SELF, 30);
	c = 3;
	i = new Item(str, *eff, c);
	combatScene->items.Add(i);

	app->map->Enable();
	if (app->map->data.type == MapTypes::MAPTYPE_UNKNOWN) app->map->LoadNewMap("tutorial.tmx");
	// Initialize player
	player = app->entities->CreateEntity(-1, -1, EntityType::PLAYER, EntityId::NOT_COMBAT, NULL);

	hero = nullptr;
	grandpa = nullptr;
	shopDude = nullptr;
	cat = nullptr;

	// COMBAT
	combatScene->Load();

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->audio->PlayMusic("Assets/Audio/Music/Tutorial.ogg");

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
	// STARTING DIALOG STUFF
	if (app->entities->dialogCounter > 0 && dialogSystem->currentDialog == nullptr)
	{
		app->entities->dialogCounter++;
		if (app->entities->dialogCounter >= 3)
		{
			app->entities->dialogCounter = 0;

			app->entities->shopkeeperFinishedTalk = false;
			app->entities->shopkeeperActive = false;

			app->entities->catFinishedTalk = false;
			app->entities->catActive = false;

			app->entities->superheroFinishedTalk = false;
			app->entities->superheroActive = false;

			app->entities->grandpaFinishedTalk = false;
			app->entities->grandpaActive = false;
		}
	}

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

	// The key to skip to the next dialog line.
	if (app->input->CheckButton("select", KEY_DOWN) && dialogSystem->currentDialog != nullptr) {
		if (app->entities->shopkeeperFinishedTalkRequest == true)
		{
			app->entities->shopkeeperFinishedTalk = false;
		}
		else if (app->entities->catFinishedTalkRequest == true)
		{
			app->entities->catFinishedTalk = false;
		}
		else if (app->entities->superheroFinishedTalkRequest == true)
		{
			app->entities->superheroFinishedTalk = false;
		}
		else if (app->entities->grandpaFinishedTalkRequest == true)
		{
			app->entities->grandpaFinishedTalk = false;
		}
		dialogSystem->NextDialog();
	}

	if (app->entities->talkingToGrandpa == true && dialogSystem->currentDialog == nullptr)
	{
		dialogSystem->StartDialog("1");
		app->entities->talkingToGrandpa = false;
	}

	if (app->entities->talkingToShopkeeper == true && dialogSystem->currentDialog == nullptr)
	{
		dialogSystem->StartDialog("2");
		app->entities->talkingToShopkeeper = false;
	}

	if (app->entities->talkingToCat == true && dialogSystem->currentDialog == nullptr)
	{
		dialogSystem->StartDialog("3");
		app->entities->talkingToCat = false;
	}

	if (app->entities->talkingToSuperhero == true && dialogSystem->currentDialog == nullptr)
	{
		dialogSystem->StartDialog("4");
		app->entities->talkingToSuperhero = false;
	}

	// Select the next option.
	if (dialogSystem->currentDialog != nullptr && app->input->CheckButton("down", KEY_DOWN)) {
		dialogSystem->selectedOption += 1;
		if (dialogSystem->selectedOption == dialogSystem->currentDialog->children->size())
			dialogSystem->selectedOption = dialogSystem->currentDialog->children->size() - 1;
	}

	// Select the previous option.
	if (dialogSystem->currentDialog != nullptr && app->input->CheckButton("up", KEY_DOWN)) {
		dialogSystem->selectedOption -= 1;
		if (dialogSystem->selectedOption < 0) dialogSystem->selectedOption = 0;
	}

	// END OF DIALOG STUFF

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
				for (int i = 0; i < combatScene->items.Count(); i++)
				{
					switch (i)
					{
					case 0:
						btnItem1->Update(dt);
						break;
					case 1:
						btnItem2->Update(dt);
						break;
					case 2:
						btnItem3->Update(dt);
						break;
					case 3:
						btnItem4->Update(dt);
						break;
					case 4:
						btnItem5->Update(dt);
						break;
					case 5:
						btnItem6->Update(dt);
						break;
					case 6:
						btnItem7->Update(dt);
						break;
					case 7:
						btnItem8->Update(dt);
						break;
					case 8:
						btnItem9->Update(dt);
						break;
					case 9:
						btnItem10->Update(dt);
						break;
					case 10:
						btnItem11->Update(dt);
						break;
					case 11:
						btnItem12->Update(dt);
						break;
					default:
						break;
					}
				}
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
				statFlags = 0;
				if (usingGamepad)
				{}
				else
				{
					int x, y;
					app->input->GetMousePosition(x, y);
					if (combatScene->characterFlags >= 2)
					{
						if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= 80 && y < (80 + menuCharacterBox.h))
						{
							statFlags = SetBit(statFlags, (uint)EntityId::MC);
						}
					}
					if (combatScene->characterFlags >= 6)
					{
						if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= (80 + menuCharacterBox.h) && y < (80 + menuCharacterBox.h + menuCharacterBox.h))
						{
							statFlags = SetBit(statFlags, (uint)EntityId::VIOLENT);
						}
					}
				}
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
		app->gui->ResetButtons();
		if ((flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
		{
			flags = ToggleBit(flags, Flags::MENU);
			if ((flags & 1 << Flags::MENU) == 0)
			{
				flags = 0;
			}
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
		app->render->DrawTexture(dialogGui, -app->render->camera.x, -app->render->camera.y + 466, false, &dialogTextBox);
		app->render->DrawTexture(dialogGui, -app->render->camera.x + 1028, -app->render->camera.y + 498, false, &portraitBox);
		if (app->entities->shopkeeperFinishedTalk == false && app->entities->shopkeeperActive == true)
		{
			app->render->DrawTexture(dialogGui, -app->render->camera.x + 1099, -app->render->camera.y + 547, false, &shopKeeperPortrait);
		}
		if (app->entities->catFinishedTalk == false && app->entities->catActive == true)
		{
			app->render->DrawTexture(dialogGui, -app->render->camera.x + 1099, -app->render->camera.y + 580, false, &catPortrait);
		}
		if (app->entities->superheroFinishedTalk == false && app->entities->superheroActive == true)
		{
			app->render->DrawTexture(dialogGui, -app->render->camera.x + 1099, -app->render->camera.y + 547, false, &superheroPortrait);
		}
		if (app->entities->grandpaFinishedTalk == false && app->entities->grandpaActive == true)
		{
			app->render->DrawTexture(dialogGui, -app->render->camera.x + 1099, -app->render->camera.y + 547, false, &grandpaPortrait);
		}
		dialogSystem->DrawDialogAnimated(dialogueFont);
	}

	if (app->saveRequest == true)
	{
		savingBool = true;
	}
	if (savingBool == true)
	{
		savingCounter++;
	}
	if (savingCounter >= 120)
	{
		savingBool = false;
		savingCounter = 0;
	}

	if (savingBool == true)
	{
		app->render->DrawTexture(dialogGui, -app->render->camera.x, -app->render->camera.y + 466, false, &dialogTextBox);
		app->render->DrawTexture(dialogGui, -app->render->camera.x + 1028, -app->render->camera.y + 498, false, &portraitBox);
		app->render->DrawTexture(dialogGui, -app->render->camera.x + 1099, -app->render->camera.y + 547, false, &hatsunePortrait);
		app->render->DrawText(dialogueFont, savingText.GetString(), 60, (app->render->camera.h / 3) * 2 + 30, 34, 1, white);
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

		// Character Boxes
		if (combatScene->characterFlags >= 2)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 984, -app->render->camera.y + 80, false, &menuCharacterBox);
			app->render->DrawTexture(combatScene->combatGui, -app->render->camera.x + 984 + 10, -app->render->camera.y + 80 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), false, &combatScene->mainChar.characterTex);
			app->render->DrawText(dialogueFont, combatScene->mainChar.hp.GetString(), /*-app->render->camera.x +*/ 984 + combatScene->mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ 80 + 20, 28, 1, white);
			app->render->DrawText(dialogueFont, combatScene->mainChar.mp.GetString(), /*-app->render->camera.x + */984 + combatScene->mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ 80 + 20 + 30, 28, 1, white);
			app->render->DrawText(dialogueFont, combatScene->mainChar.stress.GetString(), /*-app->render->camera.x +*/ 984 + combatScene->mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ 80 + 20 + 60, 28, 1, white);
		}
		if (combatScene->characterFlags >= 6)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 984, -app->render->camera.y + menuCharacterBox.h + 80, false, &menuCharacterBox);
			app->render->DrawTexture(combatScene->combatGui, -app->render->camera.x + 984 + 10, -app->render->camera.y + menuCharacterBox.h + 80 + (menuCharacterBox.h / 2 - combatScene->grandpa.characterTex.h / 2), false, &combatScene->grandpa.characterTex);
			app->render->DrawText(dialogueFont, combatScene->grandpa.hp.GetString(),/* -app->render->camera.x +*/ 984 + combatScene->grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ menuCharacterBox.h + 80 + 20, 28, 1, white);
			app->render->DrawText(dialogueFont, combatScene->grandpa.mp.GetString(),/* -app->render->camera.x +*/ 984 + combatScene->grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ menuCharacterBox.h + 80 + 20 + 30, 28, 1, white);
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

		if ((flags & 1 << Flags::INVENTORY) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 292, -app->render->camera.y + 80, false, &menuBox);
			for (int i = 0; i < combatScene->items.Count(); i++)
			{
				switch (i)
				{
				case 0:
					btnItem1->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 1:
					btnItem2->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 2:
					btnItem3->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 3:
					btnItem4->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 4:
					btnItem5->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 5:
					btnItem6->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 6:
					btnItem7->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 7:
					btnItem8->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 8:
					btnItem9->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 9:
					btnItem10->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 10:
					btnItem11->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				case 11:
					btnItem12->Draw(-app->render->camera.x, -app->render->camera.y);
					break;
				default:
					break;
				}
			}
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
			if (statFlags == 1 << (int)EntityId::MC)
			{
				app->render->DrawRectangle({ -app->render->camera.x + 984, -app->render->camera.y + 80, menuCharacterBox.w, menuCharacterBox.h }, 255, 0, 0, 50, true);
				
				app->render->DrawTexture(combatScene->combatGui, -app->render->camera.x + 292 + 40, -app->render->camera.y + 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), false, &combatScene->mainChar.characterTex);
				app->render->DrawText(dialogueFont, "Main Character", 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), 64, 1, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.lvl.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.xp.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20 + (combatScene->mainChar.lvl.Length()* 8) + 90, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.nextLvl.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20 + (combatScene->mainChar.xp.Length() * 16) + 180, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				
				// XP Bar (?)

				app->render->DrawText(buttonFont, combatScene->mainChar.pAtk.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 1), 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.mAtk.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 2) + 5, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.pDef.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 3) + 10, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.mDef.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 4) + 15, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.speed.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 5) + 20, 64, 2, white);

			}
			else if (statFlags == 1 << (int)EntityId::VIOLENT)
			{
				app->render->DrawRectangle({ -app->render->camera.x + 984, -app->render->camera.y + menuCharacterBox.h + 80, menuCharacterBox.w, menuCharacterBox.h }, 0, 255, 255, 50, true);

				app->render->DrawTexture(combatScene->combatGui, -app->render->camera.x + 292 + 40, -app->render->camera.y + 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), false, &combatScene->grandpa.characterTex);
				app->render->DrawText(dialogueFont, "Grandpa", 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), 64, 1, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.lvl.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.xp.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20 + (combatScene->mainChar.lvl.Length() * 8) + 90, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.nextLvl.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20 + (combatScene->mainChar.xp.Length() * 16) + 180, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);

				// XP Bar (?)

				app->render->DrawText(buttonFont, combatScene->grandpa.pAtk.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 1), 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.mAtk.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 2) + 5, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.pDef.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 3) + 10, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.mDef.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 4) + 15, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.speed.GetString(), 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 5) + 20, 64, 2, white);
			}
			else
			{
				SString tmp1 = "Select a character";
				SString tmp2 = "to see its stats";
				app->render->DrawText(dialogueFont, tmp1.GetString(), (app->render->camera.w / 2) - ((tmp1.Length() * 48) / 2) + 200, (app->render->camera.h / 2) - 64, 64, 1, white);
				app->render->DrawText(dialogueFont, tmp2.GetString(), (app->render->camera.w / 2) - ((tmp2.Length() * 48) / 2) + 200, (app->render->camera.h / 2), 64, 1, white);
			}
		}
	}
	else if ((flags & 1 << Flags::OPTIONS) != 0 && (flags & 1 << Flags::CONTROLS) == 0)
	{
		app->render->DrawRectangle({ -app->render->camera.x,-app->render->camera.y,app->render->camera.w,app->render->camera.h }, 0, 0, 0, 200);
		SString titleOptions = "Options";
		app->render->DrawText(buttonFont, titleOptions.GetString(), /*-app->render->camera.x +*/ ((app->render->camera.w - (titleOptions.Length() * 24)) / 2), 100, 64, 2, { 255, 255, 255, 255 });
		sldrVolume->Draw(-app->render->camera.x, -app->render->camera.y);
		sldrFx->Draw(-app->render->camera.x, -app->render->camera.y);
		if (app->win->fullscreenWindow)
			boxFullScreen->checked = true;
		boxFullScreen->Draw(-app->render->camera.x, -app->render->camera.y);
		if (app->vsync)
			boxVSync->checked = true;
		boxVSync->Draw(-app->render->camera.x, -app->render->camera.y);
		btnControls->Draw(-app->render->camera.x, -app->render->camera.y);
		btnBack->Draw(-app->render->camera.x, -app->render->camera.y);
	}
	else if ((flags & 1 << Flags::CONTROLS) != 0)
	{
		app->render->DrawRectangle({ -app->render->camera.x,-app->render->camera.y,app->render->camera.w,app->render->camera.h }, 0, 0, 0, 200);
		SString titleControls = "Controls";
		app->render->DrawText(buttonFont, titleControls.GetString(), /*app->render->camera.x +*/ ((app->render->camera.w - (titleControls.Length() * 24)) / 2), 100, 64, 2, white);

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

	RELEASE(dialogSystem);

	RELEASE(buttonFont);
	RELEASE(dialogueFont);
	app->tex->UnLoad(textBox);
	app->tex->UnLoad(dialogGui);
	savingText.Clear();

	app->gui->Disable();
	app->scene->currentButton = nullptr;

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
		app->audio->PlayMusic("Assets/Audio/Music/Options.ogg");
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
		if (app->map->data.name == "tutorial.tmx") app->audio->PlayMusic("Assets/Audio/Music/Tutorial.ogg");
		else if (app->map->data.name == "home.tmx") app->audio->PlayMusic("Assets/Audio/Music/Home.ogg");
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
	case 29:
		LOG("USED ITEM 1");
		break;
	case 30:
		LOG("USED ITEM 2");
		break;
	case 31:
		LOG("USED ITEM 3");
		break;
	case 32:
		LOG("USED ITEM 4");
		break;
	case 33:
		LOG("USED ITEM 5");
		break;
	case 34:
		LOG("USED ITEM 6");
		break;
	case 35:
		LOG("USED ITEM 7");
		break;
	case 36:
		LOG("USED ITEM 8");
		break;
	case 37:
		LOG("USED ITEM 9");
		break;
	case 38:
		LOG("USED ITEM 10");
		break;
	case 39:
		LOG("USED ITEM 11");
		break;
	case 40:
		LOG("USED ITEM 12");
		break;
	default:
		break;
	}
	return true;
}
