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
#include "Item.h"
#include "PuzzlePieces.h"
#include "QuestManager.h"
#include "Blockers.h"
#include "ParticleSystem.h"


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
	app->quests->Enable();

	dialogSystem = new DialogSystem();
	dtSave = 0.0f;
	dtItem = 0.0f;

	char tmp[TEXT_LEN] = { 0 };
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderFonts, "DialogueFont.xml");
	dialogueFont = new Font(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderFonts, "ButtonFont.xml");
	buttonFont = new Font(tmp);

	float tmpValue = 0;
	//MENU
	app->gui->Enable();
	menuBox = { 324,0,692,540 };
	menuCharacterBox = { 324,539,204,135 };
	btnInventory = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 1, { 90, 80, 200, 60 }, "INVENTORY", 35, this);
	btnSkills = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 2, { 90, 160, 200, 60 }, "SKILLS", 40, this);
	btnSkillTree = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 3, { 90, 240, 200, 60 }, "       SKILL TREE", 35, this);
	btnQuests = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 4, { 90, 320, 200, 60 }, "QUESTS", 35, this);

	btnStats = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 5, { 90, 400, 200, 60 }, " STATS ", 40, this);
	statFlags = 0;
    onceStatsFx = true;

	btnOptions = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 6, { 90, 480, 200, 60 }, "OPTIONS", 35, this);
	btnTitleScreen = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 7, { 90, 560, 200, 60 }, "          TITLE SCREEN", 35, this);

	//OPTIONS
    options = false;
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
	btnBack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 12, { 800, 600, 200, 60 }, "BACK", 40, this);

	//ITEMS
	itemSelected = 0;
	btnItem1 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 13, { 330,120,300,60 }, "item one", 40, this, 0, app->entities->itemAtlas);
	btnItem2 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 14, { 646,120,300,60 }, "item two", 40, this, 0, app->entities->itemAtlas);
	btnItem3 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 15, { 330,200,300,60 }, "item three", 40, this, 0, app->entities->itemAtlas);
	btnItem4 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 16, { 646,200,300,60 }, "item four", 40, this, 0, app->entities->itemAtlas);
	btnItem5 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 17, { 330,280,300,60 }, "item five", 40, this, 0, app->entities->itemAtlas);
	btnItem6 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 18, { 646,280,300,60 }, "item six", 40, this, 0, app->entities->itemAtlas);
	btnItem7 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 19, { 330,360,300,60 }, "item seven", 40, this, 0, app->entities->itemAtlas);
	btnItem8 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 20, { 646,360,300,60 }, "item eight", 40, this, 0, app->entities->itemAtlas);
	btnItem9 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 21, { 330,440,300,60 }, "item nine", 40, this, 0, app->entities->itemAtlas);
	btnItem10 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 22, { 646,440,300,60 }, "item ten", 40, this, 0, app->entities->itemAtlas);
	btnItem11 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 23, { 330,520,300,60 }, "item eleven", 40, this, 0, app->entities->itemAtlas);
	btnItem12 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 24, { 646,520,300,60 }, "item twelve", 40, this, 0, app->entities->itemAtlas);

	btnSkill1 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 25, { 330,120,300,60 }, "skill one", 32, this, 0, app->entities->itemAtlas);
	btnSkill2 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 26, { 646,120,300,60 }, "skill two", 32, this, 0, app->entities->itemAtlas);
	btnSkill3 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 27, { 330,200,300,60 }, "skill three", 32, this, 0, app->entities->itemAtlas);
	btnSkill4 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 28, { 646,200,300,60 }, "skill four", 32, this, 0, app->entities->itemAtlas);

	app->entities->CreateEntity(14 * 64, 89 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::HP_POTION, 3);
	app->entities->CreateEntity(33 * 64, 89 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::ELIXIR, 2);
	app->entities->CreateEntity(36 * 64, 69 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::HP_POTION, 2);
	app->entities->CreateEntity(37 * 64, 59 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::BOTTLED_SMITE, 1);
	app->entities->CreateEntity(13 * 64, 72 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::HAPPILLS, 2);
	app->entities->CreateEntity(15 * 64, 43 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::HP_POTION, 3);
	app->entities->CreateEntity(15 * 64, 41 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::MANA_POTION, 3);
	app->entities->CreateEntity(11 * 64, 25 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::ELIXIR, 3);
	app->entities->CreateEntity(42 * 64, 91 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::HAPPILLS, 2);
	app->entities->CreateEntity(63 * 64, 86 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::BOTTLED_SMITE, 2);
	app->entities->CreateEntity(65 * 64, 86 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::HAPPILLS, 3);
	app->entities->CreateEntity(59 * 64, 76 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::HP_POTION, 3);
	app->entities->CreateEntity(59 * 64, 80 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::MAGIC_BUFFER , 3);
	app->entities->CreateEntity(69 * 64, 76 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::MANA_POTION, 3);
	app->entities->CreateEntity(69 * 64, 80 * 64, EntityType::ITEM, EntityId::NOT_COMBAT, NULL, NpcId::NONE, nullptr, ItemId::PHYS_BUFFER, 3);

	// Map
	app->map->Enable();
	if (app->map->data.type == MapTypes::MAPTYPE_UNKNOWN) app->map->LoadNewMap("tutorial.tmx");

	// Initialize player
	player = app->entities->CreateEntity(-1, -1, EntityType::PLAYER, EntityId::NOT_COMBAT, NULL);

	// Puzzle elements
	buttonOne = (PuzzlePieces*)app->entities->CreateEntity(13 * 64, 41 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::BUTTON);
	buttonTwo = (PuzzlePieces*)app->entities->CreateEntity(17 * 64, 41 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::BUTTON);
	buttonThree = (PuzzlePieces*)app->entities->CreateEntity(43 * 64, 14 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::BUTTON);
	buttonFour = (PuzzlePieces*)app->entities->CreateEntity(50 * 64, 14 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::BUTTON);
	buttonFive = (PuzzlePieces*)app->entities->CreateEntity(55 * 64, 13 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::BUTTON);
	buttonSix = (PuzzlePieces*)app->entities->CreateEntity(57 * 64, 16 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::BUTTON);

	(PuzzlePieces*)app->entities->CreateEntity(13 * 64, 43 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::ROCK);
	(PuzzlePieces*)app->entities->CreateEntity(42 * 64, 13 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::ROCK);

	(PuzzlePieces*)app->entities->CreateEntity(17 * 64, 43 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::SLIDING_ROCK);
	(PuzzlePieces*)app->entities->CreateEntity(48 * 64, 13 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::SLIDING_ROCK);
	(PuzzlePieces*)app->entities->CreateEntity(50 * 64, 15 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::SLIDING_ROCK);
	(PuzzlePieces*)app->entities->CreateEntity(58 * 64, 12 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::SLIDING_ROCK);
	(PuzzlePieces*)app->entities->CreateEntity(58 * 64, 16 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::SLIDING_ROCK);
	(PuzzlePieces*)app->entities->CreateEntity(54 * 64, 16 * 64, EntityType::PUZZLE_PIECE, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::SLIDING_ROCK);

	spikeRowOne = (Blockers*)app->entities->CreateEntity(46 * 64, 10 * 64, EntityType::BLOCKER, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::NONE, BlockerId::SPIKES);
	spikeRowTwo = (Blockers*)app->entities->CreateEntity(52 * 64, 10 * 64, EntityType::BLOCKER, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::NONE, BlockerId::SPIKES);
	spikeRowThree = (Blockers*)app->entities->CreateEntity(60 * 64, 10 * 64, EntityType::BLOCKER, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::NONE, BlockerId::SPIKES);
	lockedDoor = (Blockers*)app->entities->CreateEntity(15 * 64, 38 * 64, EntityType::BLOCKER, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::NONE, BlockerId::LOCKED_DOOR);

	hero = nullptr;
	grandpa = nullptr;
	shopDude = nullptr;
	cat = nullptr;

	// COMBAT
	combatScene->Load();

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->audio->PlayMusic("Audio/Music/Tutorial.ogg");

	// Used for the Gamepad GUI control
	app->scene->currentButton = app->gui->controls.start;
	changeMenu = false;
	enteringCombat = false;
	usingGamepad = false;
	if (strcmp(app->input->GetControllerName(), "unplugged") != 0) usingGamepad = true;
	app->input->mouseMotionX = 0;
	app->input->mouseMotionY = 0;
	flags = 0;
	inMenu = false;

	if (app->scene->continueLoadRequest) app->LoadRequest();

	return false;
}

bool SceneGameplay::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		int x, y;
		app->input->GetMousePosition(x, y);
		fPoint targetPos(x,y);
		app->particles->AddEmitter(targetPos, EmitterData::EmitterType::ITEM);
	}

	dtSave = dt;
	dtItem = dt;
	UpdateDialogue(dt);

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		combat = true;
		enteringCombat = true;
	}	

	//Remove puzzle blockers
	if (buttonOne->isPressed && buttonTwo->isPressed)
	{
		if (lockedDoor != nullptr)
		{
			app->audio->PlayFx(app->entities->solvedFx);
			lockedDoor->pendingToDelete = true;
			lockedDoor = nullptr;
		}
	}
	else if (lockedDoor == nullptr)
	{
		lockedDoor = (Blockers*)app->entities->CreateEntity(15 * 64, 38 * 64, EntityType::BLOCKER, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::NONE, BlockerId::LOCKED_DOOR);
	}

	if (buttonThree->isPressed)
	{
		if (spikeRowOne != nullptr)
		{
			app->audio->PlayFx(app->entities->solvedFx);
			spikeRowOne->pendingToDelete = true;
			spikeRowOne = nullptr;
		}
	}
	else if (spikeRowOne == nullptr)
	{
		spikeRowOne = (Blockers*)app->entities->CreateEntity(46 * 64, 10 * 64, EntityType::BLOCKER, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::NONE, BlockerId::SPIKES);
	}

	if (buttonFour->isPressed)
	{
		if (spikeRowTwo != nullptr)
		{
			app->audio->PlayFx(app->entities->solvedFx);
			spikeRowTwo->pendingToDelete = true;
			spikeRowTwo = nullptr;
		}
	}
	else if (spikeRowTwo == nullptr)
	{
		spikeRowTwo = (Blockers*)app->entities->CreateEntity(52 * 64, 10 * 64, EntityType::BLOCKER, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::NONE, BlockerId::SPIKES);
	}

	if (buttonFive->isPressed && buttonSix->isPressed)
	{
		if (spikeRowThree != nullptr)
		{
			app->audio->PlayFx(app->entities->solvedFx);
			spikeRowThree->pendingToDelete = true;
			spikeRowThree = nullptr;
		}
	}
	else if (spikeRowThree == nullptr)
	{
		spikeRowThree = (Blockers*)app->entities->CreateEntity(60 * 64, 10 * 64, EntityType::BLOCKER, EntityId::NOT_COMBAT, NULL, NpcId::NONE, player, ItemId::NONE, 0, PuzzleId::NONE, BlockerId::SPIKES);
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

	if (combatScene->backToGameplay)
	{
		combatScene->UpdateTransition(dt, TransitionStatus::SCENE);
	}

	if (combatScene->heDed) TransitionToScene(SceneType::ENDING);

	return true;
}

bool SceneGameplay::UpdateDialogue(float dt)
{
	// STARTING DIALOG STUFF
	if (app->entities->dialogCounter > 0.0f && dialogSystem->currentDialog == nullptr)
	{
		app->entities->dialogCounter += dt;
		if (app->entities->dialogCounter >= 0.545f)
		{
			app->entities->dialogCounter = 0.0f;

			app->entities->flagsShopkeeper = 0;
			app->entities->flagsCat = 0;
			app->entities->flagsSuperhero = 0;
			app->entities->flagsGrandpa = 0;
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
		if ((app->entities->flagsShopkeeper & 1 << (int)DialogueFlags::FINISHED_TALK_REQUEST) != 0)
		{
			app->entities->flagsShopkeeper = ClearBit(app->entities->flagsShopkeeper, DialogueFlags::FINISHED_TALK);
		}
		else if ((app->entities->flagsCat & 1 << (int)DialogueFlags::FINISHED_TALK_REQUEST) != 0)
		{
			app->entities->flagsCat = ClearBit(app->entities->flagsCat, DialogueFlags::FINISHED_TALK);
		}
		else if ((app->entities->flagsSuperhero & 1 << (int)DialogueFlags::FINISHED_TALK_REQUEST) != 0)
		{
			app->entities->flagsSuperhero = ClearBit(app->entities->flagsSuperhero, DialogueFlags::FINISHED_TALK);
		}
		else if ((app->entities->flagsGrandpa & 1 << (int)DialogueFlags::FINISHED_TALK_REQUEST) != 0)
		{
			app->entities->flagsGrandpa = ClearBit(app->entities->flagsGrandpa, DialogueFlags::FINISHED_TALK);
		}
		dialogSystem->NextDialog();
	}

	if (dialogSystem->currentDialog == nullptr)
	{
		if ((app->entities->flagsGrandpa & 1 << (int)DialogueFlags::TALKING_TO) != 0)
		{
			dialogSystem->StartDialog("1");
			app->entities->flagsGrandpa = ClearBit(app->entities->flagsGrandpa, DialogueFlags::TALKING_TO);
		}

		if ((app->entities->flagsShopkeeper & 1 << (int)DialogueFlags::TALKING_TO) != 0)
		{
			dialogSystem->StartDialog("2");
			app->entities->flagsShopkeeper = ClearBit(app->entities->flagsShopkeeper, DialogueFlags::TALKING_TO);
		}

		if ((app->entities->flagsCat & 1 << (int)DialogueFlags::TALKING_TO) != 0)
		{
			dialogSystem->StartDialog("3");
			app->entities->flagsCat = ClearBit(app->entities->flagsCat, DialogueFlags::TALKING_TO);
		}

		if ((app->entities->flagsSuperhero & 1 << (int)DialogueFlags::TALKING_TO) != 0)
		{
			dialogSystem->StartDialog("4");
			app->entities->flagsSuperhero = ClearBit(app->entities->flagsSuperhero, DialogueFlags::TALKING_TO);
		}
	}
	else
	{
		// Select the next option.
		if (app->input->CheckButton("down", KEY_DOWN)) {
			dialogSystem->selectedOption += 1;
			if (dialogSystem->selectedOption == dialogSystem->currentDialog->children->size())
				dialogSystem->selectedOption = dialogSystem->currentDialog->children->size() - 1;
		}

		// Select the previous option.
		if (app->input->CheckButton("up", KEY_DOWN)) {
			dialogSystem->selectedOption -= 1;
			if (dialogSystem->selectedOption < 0) dialogSystem->selectedOption = 0;
		}
	}

	// END OF DIALOG STUFF

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		combat = true;
		enteringCombat = true;
	}

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

	for (int i = 0; i < combatScene->items.Count(); i++)
	{
		switch (i)
		{
		case 0:
			btnItem1->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem1->text, 0, TEXT_LEN);
			strcpy_s(btnItem1->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem1->count, 0, TEXT_LEN);
			strcpy_s(btnItem1->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem1->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem1;
			break;
		case 1:
			btnItem2->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem2->text, 0, TEXT_LEN);
			strcpy_s(btnItem2->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem2->count, 0, TEXT_LEN);
			strcpy_s(btnItem2->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem2->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem2;
			break;
		case 2:
			btnItem3->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem3->text, 0, TEXT_LEN);
			strcpy_s(btnItem3->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem3->count, 0, TEXT_LEN);
			strcpy_s(btnItem3->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem3->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem3;
			break;
		case 3:
			btnItem4->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem4->text, 0, TEXT_LEN);
			strcpy_s(btnItem4->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem4->count, 0, TEXT_LEN);
			strcpy_s(btnItem4->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem4->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem4;
			break;
		case 4:
			btnItem5->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem5->text, 0, TEXT_LEN);
			strcpy_s(btnItem5->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem5->count, 0, TEXT_LEN);
			strcpy_s(btnItem5->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem5->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem5;
			break;
		case 5:
			btnItem6->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem6->text, 0, TEXT_LEN);
			strcpy_s(btnItem6->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem6->count, 0, TEXT_LEN);
			strcpy_s(btnItem6->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem6->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem6;
			break;
		case 6:
			btnItem7->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem7->text, 0, TEXT_LEN);
			strcpy_s(btnItem7->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem7->count, 0, TEXT_LEN);
			strcpy_s(btnItem7->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem7->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem7;
			break;
		case 7:
			btnItem8->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem8->text, 0, TEXT_LEN);
			strcpy_s(btnItem8->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem8->count, 0, TEXT_LEN);
			strcpy_s(btnItem8->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem8->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem8;
			break;
		case 8:
			btnItem9->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem9->text, 0, TEXT_LEN);
			strcpy_s(btnItem9->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem9->count, 0, TEXT_LEN);
			strcpy_s(btnItem9->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem9->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem9;
			break;
		case 9:
			btnItem10->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem10->text, 0, TEXT_LEN);
			strcpy_s(btnItem10->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem10->count, 0, TEXT_LEN);
			strcpy_s(btnItem10->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem10->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem10;
			break;
		case 10:
			btnItem11->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem11->text, 0, TEXT_LEN);
			strcpy_s(btnItem11->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem11->count, 0, TEXT_LEN);
			strcpy_s(btnItem11->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem11->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem11;
			break;
		case 11:
			btnItem12->itemId = combatScene->items.At(i)->data->id;
			memset(btnItem12->text, 0, TEXT_LEN);
			strcpy_s(btnItem12->text, TEXT_LEN, combatScene->items.At(i)->data->effect.attackName);
			memset(btnItem12->count, 0, TEXT_LEN);
			strcpy_s(btnItem12->count, TEXT_LEN, combatScene->items.At(i)->data->countText);
			btnItem12->sec = combatScene->items.At(i)->data->texSec;
			combatScene->items.At(i)->data->button = btnItem12;
			break;
		default:
			break;
		}
	}

	// Calls update with gamepad parameters (GUI)
	if (usingGamepad)
	{
		if (!options)
		{
			if ((flags & 1 << Flags::INVENTORY) != 0)
			{
				// For now, we'll use mouse for this
				statFlags = 0;
				int x, y;
				app->input->GetMousePosition(x, y);
				if (combatScene->characterFlags >= 2)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= 80 && y < (80 + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::MC);
						if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
						{
							if (itemSelected != 0)
							{
								app->gui->ResetButtons();
								btnInventory->state = GuiControlState::DISABLED;
								combatScene->items.At(itemSelected - 1)->data->Use(combatScene->mainChar.character);
								if (combatScene->items.At(itemSelected - 1)->data->count == 0)
								{
									combatScene->items.Del(combatScene->items.At(itemSelected - 1));
								}
								itemSelected = 0;
							}
						}
					}
				}
				if (combatScene->characterFlags >= 6)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= (80 + menuCharacterBox.h) && y < (80 + menuCharacterBox.h + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::VIOLENT);
						if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
						{
							if (itemSelected != 0)
							{
								app->gui->ResetButtons();
								btnInventory->state = GuiControlState::DISABLED;
								combatScene->items.At(itemSelected - 1)->data->Use(combatScene->grandpa.character);
								if (combatScene->items.At(itemSelected - 1)->data->count == 0)
								{
									combatScene->items.Del(combatScene->items.At(itemSelected - 1));
								}
								itemSelected = 0;
							}
						}
					}
				}

				if (changeMenu)
				{
					if (combatScene->items.Count() != 0) app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnItem1));
					changeMenu = false;
				}
				if (combatScene->items.Count() != 0 && itemSelected == 0) app->scene->currentButton->data->Update(dt, 31, 31 + combatScene->items.Count());
				if (app->input->CheckButton("cancel",KEY_DOWN))
				{
					flags = ClearBit(flags, Flags::INVENTORY);
					changeMenu = true;
					app->gui->ResetButtons();
					itemSelected = 0;
					usingGamepad = true;
				}
			}
			else if ((flags & 1 << Flags::SKILLS) != 0)
			{
				statFlags = 0;
				int x, y;
				app->input->GetMousePosition(x, y);
				if (combatScene->characterFlags >= 2)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= 80 && y < (75 + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::MC);
					}
				}
				if (combatScene->characterFlags >= 6)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= (85 + menuCharacterBox.h) && y < (80 + menuCharacterBox.h + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::VIOLENT);
					}
				}

				int i = 1;
				int maxSkills = 5;
				for (i; i < maxSkills; i++)
				{
					switch (i)
					{
					case 1:
						if (statFlags == 1 << (int)EntityId::MC)
						{
							memset(btnSkill1->text, 0, TEXT_LEN);
							strcpy_s(btnSkill1->text, TEXT_LEN, combatScene->mainChar.character->attackPool.At(i)->data->attackName);
							btnSkill1->sec = combatScene->SkillSec(i, EntityId::MC);
						}
						else if (statFlags == 1 << (int)EntityId::VIOLENT)
						{
							memset(btnSkill1->text, 0, TEXT_LEN);
							strcpy_s(btnSkill1->text, TEXT_LEN, combatScene->grandpa.character->attackPool.At(i)->data->attackName);
							btnSkill1->sec = combatScene->SkillSec(i, EntityId::VIOLENT);
						}
						break;
					case 2:
						if (statFlags == 1 << (int)EntityId::MC)
						{
							memset(btnSkill2->text, 0, TEXT_LEN);
							strcpy_s(btnSkill2->text, TEXT_LEN, combatScene->mainChar.character->attackPool.At(i)->data->attackName);
							btnSkill2->sec = combatScene->SkillSec(i, EntityId::MC);
						}
						else if (statFlags == 1 << (int)EntityId::VIOLENT)
						{
							memset(btnSkill2->text, 0, TEXT_LEN);
							strcpy_s(btnSkill2->text, TEXT_LEN, combatScene->grandpa.character->attackPool.At(i)->data->attackName);
							btnSkill2->sec = combatScene->SkillSec(i, EntityId::VIOLENT);
						}
						break;
					case 3:
						if (statFlags == 1 << (int)EntityId::MC)
						{
							memset(btnSkill3->text, 0, TEXT_LEN);
							strcpy_s(btnSkill3->text, TEXT_LEN, combatScene->mainChar.character->attackPool.At(i)->data->attackName);
							btnSkill3->sec = combatScene->SkillSec(i, EntityId::MC);
						}
						else if (statFlags == 1 << (int)EntityId::VIOLENT)
						{
							memset(btnSkill3->text, 0, TEXT_LEN);
							strcpy_s(btnSkill3->text, TEXT_LEN, combatScene->grandpa.character->attackPool.At(i)->data->attackName);
							btnSkill3->sec = combatScene->SkillSec(i, EntityId::VIOLENT);
						}

						break;
					case 4:
						if (statFlags == 1 << (int)EntityId::MC)
						{
							memset(btnSkill4->text, 0, TEXT_LEN);
							strcpy_s(btnSkill4->text, TEXT_LEN, combatScene->mainChar.character->attackPool.At(i)->data->attackName);
							btnSkill4->sec = combatScene->SkillSec(i, EntityId::MC);
						}
						else if (statFlags == 1 << (int)EntityId::VIOLENT)
						{
							memset(btnSkill4->text, 0, TEXT_LEN);
							strcpy_s(btnSkill4->text, TEXT_LEN, combatScene->grandpa.character->attackPool.At(i)->data->attackName);
							btnSkill4->sec = combatScene->SkillSec(i, EntityId::VIOLENT);
						}
						break;
					//case 5:
					//	if (statFlags == 1 << (int)EntityId::MC)
					//	{
					//		memset(btnSkill5->text, 0, TEXT_LEN);
					//		strcpy_s(btnSkill5->text, TEXT_LEN, combatScene->mainChar.character->attackPool.At(i)->data->attackName);
					//		btnSkill5->sec = combatScene->SkillSec(i, EntityId::MC);
					//	}
					//	else if (statFlags == 1 << (int)EntityId::VIOLENT)
					//	{
					//		memset(btnSkill5->text, 0, TEXT_LEN);
					//		strcpy_s(btnSkill5->text, TEXT_LEN, combatScene->grandpa.character->attackPool.At(i)->data->attackName);
					//		btnSkill5->sec = combatScene->SkillSec(i, EntityId::VIOLENT);
					//	}
					//	break;
					//case 6:
					//	if (statFlags == 1 << (int)EntityId::MC)
					//	{
					//		memset(btnSkill6->text, 0, TEXT_LEN);
					//		strcpy_s(btnSkill6->text, TEXT_LEN, combatScene->mainChar.character->attackPool.At(i)->data->attackName);
					//		btnSkill6->sec = combatScene->SkillSec(i, EntityId::MC);
					//	}
					//	else if (statFlags == 1 << (int)EntityId::VIOLENT)
					//	{
					//		memset(btnSkill6->text, 0, TEXT_LEN);
					//		strcpy_s(btnSkill6->text, TEXT_LEN, combatScene->grandpa.character->attackPool.At(i)->data->attackName);
					//		btnSkill6->sec = combatScene->SkillSec(i, EntityId::VIOLENT);
					//	}
					//	break;
					}
				}
				if (statFlags == 1 << (int)EntityId::MC || statFlags == 1 << (int)EntityId::VIOLENT)
				{
					btnSkill1->Update(dt);
					btnSkill2->Update(dt);
					btnSkill3->Update(dt);
					btnSkill4->Update(dt);
				}

				if (app->input->CheckButton("cancel", KEY_DOWN))
				{
					flags = ClearBit(flags, Flags::SKILLS);
					changeMenu = true;
					app->gui->ResetButtons();
					itemSelected = 0;
					usingGamepad = true;
				}
			}
			else if ((flags & 1 << Flags::STATS) != 0)
			{
				statFlags = 0;
				int x, y;
				app->input->GetMousePosition(x, y);
				if (combatScene->characterFlags >= 2)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= 80 && y < (75 + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::MC);
					}
				}
				if (combatScene->characterFlags >= 6)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= (85 + menuCharacterBox.h) && y < (80 + menuCharacterBox.h + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::VIOLENT);
					}
				}
				if (app->input->CheckButton("cancel", KEY_DOWN))
				{
					flags = ClearBit(flags, Flags::STATS);
					changeMenu = true;
					app->gui->ResetButtons();
					itemSelected = 0;
					usingGamepad = true;
				}
			}
			else
			{
				if (changeMenu)
				{
					app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnInventory));
					changeMenu = false;
				}
				app->scene->currentButton->data->Update(dt, 1, 7);
			}
		}
		else if (options)
		{
			if (changeMenu)
			{
				app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(sldrVolume));
				changeMenu = false;
			}
			app->scene->currentButton->data->Update(dt, 8, 13);

			if (app->input->CheckButton("cancel", KEY_DOWN))
			{
                options = false;
				if (strcmp(app->map->data.name, "tutorial.tmx") == 0) app->audio->PlayMusic("Audio/Music/Tutorial.ogg");
				else if (strcmp(app->map->data.name, "home.tmx") == 0) app->audio->PlayMusic("Audio/Music/Home.ogg");
				changeMenu = true;
				app->gui->ResetButtons();
				usingGamepad = true;
			}
		}
	}
	// Calls update for mouse parameters (GUI)
	else
	{
		if ((flags & 1 << Flags::MENU) != 0 && options == false)
		{
			btnInventory->Update(dt);
			btnSkills->Update(dt);
			btnSkillTree->Update(dt);
			btnQuests->Update(dt);
			btnStats->Update(dt);
			btnOptions->Update(dt);
			btnTitleScreen->Update(dt);

			if ((flags & 1 << Flags::INVENTORY) != 0)
			{
				statFlags = 0;
				int x, y;
				app->input->GetMousePosition(x, y);
				if (combatScene->characterFlags >= 2)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= 80 && y < (80 + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::MC);
						if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
						{
							if (itemSelected != 0)
							{
								app->gui->ResetButtons();
								btnInventory->state = GuiControlState::DISABLED;
								combatScene->items.At(itemSelected - 1)->data->Use(combatScene->mainChar.character);
								if (combatScene->items.At(itemSelected - 1)->data->count == 0)
								{
									combatScene->items.Del(combatScene->items.At(itemSelected - 1));
								}
								itemSelected = 0;
							}
						}
					}
				}
				if (combatScene->characterFlags >= 6)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= (80 + menuCharacterBox.h) && y < (80 + menuCharacterBox.h + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::VIOLENT);
						if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
						{
							if (itemSelected != 0)
							{
								app->gui->ResetButtons();
								btnInventory->state = GuiControlState::DISABLED;
								combatScene->items.At(itemSelected - 1)->data->Use(combatScene->grandpa.character);
								if (combatScene->items.At(itemSelected - 1)->data->count == 0)
								{
									combatScene->items.Del(combatScene->items.At(itemSelected - 1));
								}
								itemSelected = 0;
							}
						}
					}
				}

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
				statFlags = 0;
				int x, y;
				app->input->GetMousePosition(x, y);
				if (combatScene->characterFlags >= 2)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= 80 && y < (75 + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::MC);
					}
				}
				if (combatScene->characterFlags >= 6)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= (85 + menuCharacterBox.h) && y < (80 + menuCharacterBox.h + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::VIOLENT);
					}
				}

				int i = 1;
				int maxSkills = 5;
				for (i; i < maxSkills; i++)
				{
					switch (i)
					{
					case 1:
						if (statFlags == 1 << (int)EntityId::MC)
						{
							memset(btnSkill1->text, 0, TEXT_LEN);
							strcpy_s(btnSkill1->text, TEXT_LEN, combatScene->mainChar.character->attackPool.At(i)->data->attackName);
							btnSkill1->sec = combatScene->SkillSec(i, EntityId::MC);
						}
						else if (statFlags == 1 << (int)EntityId::VIOLENT)
						{
							memset(btnSkill1->text, 0, TEXT_LEN);
							strcpy_s(btnSkill1->text, TEXT_LEN, combatScene->grandpa.character->attackPool.At(i)->data->attackName);
							btnSkill1->sec = combatScene->SkillSec(i, EntityId::VIOLENT);
						}
						btnSkill1->Update(dt);
						break;
					case 2:
						if (statFlags == 1 << (int)EntityId::MC)
						{
							memset(btnSkill2->text, 0, TEXT_LEN);
							strcpy_s(btnSkill2->text, TEXT_LEN, combatScene->mainChar.character->attackPool.At(i)->data->attackName);
							btnSkill2->sec = combatScene->SkillSec(i, EntityId::MC);
						}
						else if (statFlags == 1 << (int)EntityId::VIOLENT)
						{
							memset(btnSkill2->text, 0, TEXT_LEN);
							strcpy_s(btnSkill2->text, TEXT_LEN, combatScene->grandpa.character->attackPool.At(i)->data->attackName);
							btnSkill2->sec = combatScene->SkillSec(i, EntityId::VIOLENT);
						}
						btnSkill2->Update(dt);
						break;
					case 3:
						if (statFlags == 1 << (int)EntityId::MC)
						{
							memset(btnSkill3->text, 0, TEXT_LEN);
							strcpy_s(btnSkill3->text, TEXT_LEN, combatScene->mainChar.character->attackPool.At(i)->data->attackName);
							btnSkill2->sec = combatScene->SkillSec(i, EntityId::MC);
						}
						else if (statFlags == 1 << (int)EntityId::VIOLENT)
						{
							memset(btnSkill3->text, 0, TEXT_LEN);
							strcpy_s(btnSkill3->text, TEXT_LEN, combatScene->grandpa.character->attackPool.At(i)->data->attackName);
							btnSkill3->sec = combatScene->SkillSec(i, EntityId::VIOLENT);
						}
						btnSkill3->Update(dt);
						break;
					case 4:
						if (statFlags == 1 << (int)EntityId::MC)
						{
							memset(btnSkill4->text, 0, TEXT_LEN);
							strcpy_s(btnSkill4->text, TEXT_LEN, combatScene->mainChar.character->attackPool.At(i)->data->attackName);
							btnSkill4->sec = combatScene->SkillSec(i, EntityId::MC);
						}
						else if (statFlags == 1 << (int)EntityId::VIOLENT)
						{
							memset(btnSkill4->text, 0, TEXT_LEN);
							strcpy_s(btnSkill4->text, TEXT_LEN, combatScene->grandpa.character->attackPool.At(i)->data->attackName);
							btnSkill4->sec = combatScene->SkillSec(i, EntityId::VIOLENT);
						}
						btnSkill4->Update(dt);
						break;
					}
				}
			}
			else if ((flags & 1 << Flags::SKILL_TREE) != 0)
			{
				// arrow buttons maybe?
				// buttons for each skill in the skill tree?
				// the skill tree lines change when you get the node?
			}
			else if ((flags & 1 << Flags::QUESTS) != 0)
			{
				// buttons for every equipment??? ps: oh lord
			}
			else if ((flags & 1 << Flags::STATS) != 0)
			{
				statFlags = 0;
				int x, y;
				app->input->GetMousePosition(x, y);
				if (combatScene->characterFlags >= 2)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= 80 && y < (75 + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::MC);
					}
				}
				if (combatScene->characterFlags >= 6)
				{
					if (x >= 984 && x < (984 + menuCharacterBox.w) && y >= (85 + menuCharacterBox.h) && y < (80 + menuCharacterBox.h + menuCharacterBox.h))
					{
						statFlags = SetBit(statFlags, (uint)EntityId::VIOLENT);
					}
				}
			}
		}
		else if (options)
		{
			sldrVolume->Update(dt);
			sldrFx->Update(dt);
			boxFullScreen->Update(dt);
			boxVSync->Update(dt);
			btnBack->Update(dt);
		}
	}
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->CheckButton("menu", KeyState::KEY_DOWN))
	{
		// Logic to manage pause menu and menu flags bit
		app->gui->ResetButtons();
		if (!options)
		{
			flags = ToggleBit(flags, Flags::MENU);
			if ((flags & 1 << Flags::MENU) == 0)
			{
				flags = 0;
			}
			app->entities->inPause = !app->entities->inPause;
		}

		// Volume between pause menu and gameplay logic
		if (app->entities->inPause && !options)
		{
			app->audio->auxVolume = app->audio->GetMusicVolume();
			app->audio->SetMusicVolume(app->audio->GetMusicVolume() / 3);
		}
		else if (options);
		else
		{
			app->audio->SetMusicVolume(app->audio->auxVolume);
		}
	}

	return true;
}

bool SceneGameplay::Draw()
{
	if (combatScene->waitForTransition == TransitionStatus::NONE || combatScene->waitForTransition == TransitionStatus::SCENE)
	{
		if (!combat) app->render->background = { 0,0,0,255 };

		app->map->Draw();
	}
	
	if (combat || combatScene->waitForTransition == TransitionStatus::BATTLE || combatScene->waitForTransition == TransitionStatus::END || !app->scene->transitionAlpha >= 1.0f)
	{
		combatScene->Draw(dialogueFont);
	}

	if (dialogSystem->currentDialog != nullptr)
	{
		app->render->DrawTexture(dialogSystem->dialogGui, -app->render->camera.x, -app->render->camera.y + 466, false, &dialogSystem->dialogTextBox);
		app->render->DrawTexture(dialogSystem->dialogGui, -app->render->camera.x + 1028, -app->render->camera.y + 498, false, &dialogSystem->portraitBox);
		if ((app->entities->flagsShopkeeper & 1 << (int)DialogueFlags::FINISHED_TALK) == 0 && (app->entities->flagsShopkeeper & 1 << (int)DialogueFlags::ACTIVE) != 0)
		{
			app->render->DrawTexture(dialogSystem->dialogGui, -app->render->camera.x + 1099, -app->render->camera.y + 547, false, &dialogSystem->shopKeeperPortrait);
		}
		if ((app->entities->flagsCat & 1 << (int)DialogueFlags::FINISHED_TALK) == 0 && (app->entities->flagsCat & 1 << (int)DialogueFlags::ACTIVE) != 0)
		{
			app->render->DrawTexture(dialogSystem->dialogGui, -app->render->camera.x + 1099, -app->render->camera.y + 580, false, &dialogSystem->catPortrait);
		}
		if ((app->entities->flagsSuperhero & 1 << (int)DialogueFlags::FINISHED_TALK) == 0 && (app->entities->flagsSuperhero & 1 << (int)DialogueFlags::ACTIVE) != 0)
		{
			app->render->DrawTexture(dialogSystem->dialogGui, -app->render->camera.x + 1099, -app->render->camera.y + 547, false, &dialogSystem->superheroPortrait);
		}
		if ((app->entities->flagsGrandpa & 1 << (int)DialogueFlags::FINISHED_TALK) == 0 && (app->entities->flagsGrandpa & 1 << (int)DialogueFlags::ACTIVE) != 0)
		{
			app->render->DrawTexture(dialogSystem->dialogGui, -app->render->camera.x + 1099, -app->render->camera.y + 547, false, &dialogSystem->grandpaPortrait);
		}
		dialogSystem->DrawDialog(dialogueFont);
	}

	if (app->saveRequest == true)
	{
		dialogSystem->savingBool = true;
	}
	if (dialogSystem->savingBool == true)
	{
		dialogSystem->savingCounter += dtSave;
	}
	if (dialogSystem->savingCounter >= 1.5f)
	{
		dialogSystem->savingBool = false;
		dialogSystem->savingCounter = 0.0f;
	}

	if (dialogSystem->savingBool == true)
	{
		app->render->DrawTexture(dialogSystem->dialogGui, -app->render->camera.x, -app->render->camera.y + 466, false, &dialogSystem->dialogTextBox);
		app->render->DrawTexture(dialogSystem->dialogGui, -app->render->camera.x + 1028, -app->render->camera.y + 498, false, &dialogSystem->portraitBox);
		app->render->DrawTexture(dialogSystem->dialogGui, -app->render->camera.x + 1099, -app->render->camera.y + 547, false, &dialogSystem->hatsunePortrait);
		app->render->DrawText(dialogueFont, dialogSystem->savingText, 60, (app->render->camera.h / 3) * 2 + 30, 34, 1, white);
	}

	// Textbox when taking an item

	if (app->entities->takingItem == true)
	{
		dialogSystem->itemCounter += dtItem;
	}

	if (dialogSystem->itemCounter >= 1.0f)
	{
		app->entities->takingItem = false;
		dialogSystem->itemCounter = 0.0f;
	}

	if (app->entities->takingItem == true)
	{
		app->render->DrawTexture(dialogSystem->dialogGui, -app->render->camera.x, -app->render->camera.y + 466, false, &dialogSystem->dialogTextBox);
		app->render->DrawText(dialogueFont, app->entities->itemPasser, 60, (app->render->camera.h / 3) * 2 + 30, 34, 1, white);
	}

	if (combatScene->backToGameplay)
	{
		app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y, app->render->camera.w, app->render->camera.h }, 0, 0, 0, Uint8(255 - (255 * combatScene->alpha)));
	}

	return false;
}

bool SceneGameplay::DrawPauseMenu()
{
	if ((flags & 1 << Flags::MENU) != 0 && options == false)
	{
		app->render->DrawRectangle({ -app->render->camera.x,-app->render->camera.y,app->render->camera.w,app->render->camera.h }, 0, 0, 0, 200);
		btnInventory->Draw(-app->render->camera.x, -app->render->camera.y);
		btnSkills->Draw(-app->render->camera.x, -app->render->camera.y);
		btnSkillTree->Draw(-app->render->camera.x, -app->render->camera.y);
		btnQuests->Draw(-app->render->camera.x, -app->render->camera.y);
		btnStats->Draw(-app->render->camera.x, -app->render->camera.y);
		btnOptions->Draw(-app->render->camera.x, -app->render->camera.y);
		btnTitleScreen->Draw(-app->render->camera.x, -app->render->camera.y);

		// Character Boxes
		if (combatScene->characterFlags >= 2)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 984, -app->render->camera.y + 80, false, &menuCharacterBox);
			app->render->DrawTexture(combatScene->combatGui, -app->render->camera.x + 984 + 10, -app->render->camera.y + 80 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), false, &combatScene->mainChar.characterTex);
			app->render->DrawText(dialogueFont, combatScene->mainChar.hp, /*-app->render->camera.x +*/ 984 + combatScene->mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ 80 + 20, 28, 1, white);
			app->render->DrawText(dialogueFont, combatScene->mainChar.mp, /*-app->render->camera.x + */984 + combatScene->mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ 80 + 20 + 30, 28, 1, white);
			app->render->DrawText(dialogueFont, combatScene->mainChar.stress, /*-app->render->camera.x +*/ 984 + combatScene->mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ 80 + 20 + 60, 28, 1, white);
		}
		if (combatScene->characterFlags >= 6)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 984, -app->render->camera.y + menuCharacterBox.h + 80, false, &menuCharacterBox);
			app->render->DrawTexture(combatScene->combatGui, -app->render->camera.x + 984 + 10, -app->render->camera.y + menuCharacterBox.h + 80 + (menuCharacterBox.h / 2 - combatScene->grandpa.characterTex.h / 2), false, &combatScene->grandpa.characterTex);
			app->render->DrawText(dialogueFont, combatScene->grandpa.hp,/* -app->render->camera.x +*/ 984 + combatScene->grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ menuCharacterBox.h + 80 + 20, 28, 1, white);
			app->render->DrawText(dialogueFont, combatScene->grandpa.mp,/* -app->render->camera.x +*/ 984 + combatScene->grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ menuCharacterBox.h + 80 + 20 + 30, 28, 1, white);
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

			if (itemSelected != 0)
			{
				if (statFlags == 1 << (int)EntityId::MC)
				{
					app->render->DrawRectangle({ -app->render->camera.x + 984, -app->render->camera.y + 80, menuCharacterBox.w, menuCharacterBox.h }, 255, 0, 0, 50, true);
				}
				else if (statFlags == 1 << (int)EntityId::VIOLENT)
				{
					app->render->DrawRectangle({ -app->render->camera.x + 984, -app->render->camera.y + menuCharacterBox.h + 80, menuCharacterBox.w, menuCharacterBox.h }, 0, 255, 255, 50, true);
				}
				app->render->DrawText(dialogueFont, "Use item on...", 485, 20, 64, 2, white);
			}

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
			if (statFlags == 1 << (int)EntityId::MC || statFlags == 1 << (int)EntityId::VIOLENT)
			{
				btnSkill1->Draw(-app->render->camera.x, -app->render->camera.y);
				btnSkill2->Draw(-app->render->camera.x, -app->render->camera.y);
				btnSkill3->Draw(-app->render->camera.x, -app->render->camera.y);
				btnSkill4->Draw(-app->render->camera.x, -app->render->camera.y);
                int x, y;
                app->input->GetMousePosition(x, y);
                if (onceStatsFx)
                {
                    onceStatsFx = false;
                    if (y < (75 + menuCharacterBox.h))
                    {
                        app->audio->PlayFx(app->entities->playerFx);
                    }
                    else
                    {
                        app->audio->PlayFx(app->entities->interactGrandpa);
                    }
                }
			}
			else
			{
				char tmp1[TEXT_LEN] = { 0 };
				strcpy_s(tmp1, TEXT_LEN, "Select a character");
				char tmp2[TEXT_LEN] = { 0 };
				strcpy_s(tmp2, TEXT_LEN, "to see its skills");
				app->render->DrawText(dialogueFont, tmp1, (app->render->camera.w / 2) - ((strlen(tmp1) * 48) / 2) + 200, (app->render->camera.h / 2) - 64, 64, 1, white);
				app->render->DrawText(dialogueFont, tmp2, (app->render->camera.w / 2) - ((strlen(tmp2) * 48) / 2) + 200, (app->render->camera.h / 2), 64, 1, white);
                onceStatsFx = true;
            }

		}
		else if ((flags & 1 << Flags::SKILL_TREE) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 292, -app->render->camera.y + 80, false, &menuBox);
		}
		else if ((flags & 1 << Flags::QUESTS) != 0)
		{
        app->quests->DrawActiveQuests();
		}
		else if ((flags & 1 << Flags::STATS) != 0)
		{
			app->render->DrawTexture(app->gui->atlas, -app->render->camera.x + 292, -app->render->camera.y + 80, false, &menuBox);
			if (statFlags == 1 << (int)EntityId::MC)
			{
				app->render->DrawRectangle({ -app->render->camera.x + 984, -app->render->camera.y + 80, menuCharacterBox.w, menuCharacterBox.h }, 255, 0, 0, 50, true);
				
				app->render->DrawTexture(combatScene->combatGui, -app->render->camera.x + 292 + 40, -app->render->camera.y + 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), false, &combatScene->mainChar.characterTex);
				app->render->DrawText(dialogueFont, "Main Character", 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), 64, 1, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.lvl, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.xp, 292 + 40 + combatScene->mainChar.characterTex.w + 20 + (strlen(combatScene->mainChar.lvl)* 8) + 90, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.nextLvl, 292 + 40 + combatScene->mainChar.characterTex.w + 20 + (strlen(combatScene->mainChar.xp) * 16) + 180, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				
				// XP Bar (?)

				app->render->DrawText(buttonFont, combatScene->mainChar.pAtk, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 1), 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.mAtk, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 2) + 5, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.pDef, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 3) + 10, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.mDef, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 4) + 15, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->mainChar.speed, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 5) + 20, 64, 2, white);

                if (onceStatsFx)
                {
                    onceStatsFx = false;
                    app->audio->PlayFx(app->entities->playerFx);
                }
			}
			else if (statFlags == 1 << (int)EntityId::VIOLENT)
			{
				app->render->DrawRectangle({ -app->render->camera.x + 984, -app->render->camera.y + menuCharacterBox.h + 80, menuCharacterBox.w, menuCharacterBox.h }, 0, 255, 255, 50, true);

				app->render->DrawTexture(combatScene->combatGui, -app->render->camera.x + 292 + 40, -app->render->camera.y + 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), false, &combatScene->grandpa.characterTex);
				app->render->DrawText(dialogueFont, "Grandpa", 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2), 64, 1, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.lvl, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.xp, 292 + 40 + combatScene->mainChar.characterTex.w + 20 + (strlen(combatScene->mainChar.lvl) * 8) + 90, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.nextLvl, 292 + 40 + combatScene->mainChar.characterTex.w + 20 + (strlen(combatScene->mainChar.xp) * 16) + 180, 100 + (menuCharacterBox.h / 2 - combatScene->mainChar.characterTex.h / 2) + 64, 48, 1, white);
				
                // XP Bar (?)

				app->render->DrawText(buttonFont, combatScene->grandpa.pAtk, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 1), 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.mAtk, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 2) + 5, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.pDef, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 3) + 10, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.mDef, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 4) + 15, 64, 2, white);
				app->render->DrawText(buttonFont, combatScene->grandpa.speed, 292 + 40 + combatScene->mainChar.characterTex.w + 20, 100 + combatScene->mainChar.characterTex.h + (48 * 5) + 20, 64, 2, white);
			    
                if (onceStatsFx)
                {
                    onceStatsFx = false;
                    app->audio->PlayFx(app->entities->interactGrandpa);
                }
            }
			else
			{
				char tmp1[TEXT_LEN] = { 0 };
				strcpy_s(tmp1, TEXT_LEN, "Select a character");
				char tmp2[TEXT_LEN] = { 0 };
				strcpy_s(tmp2, TEXT_LEN, "to see its stats");
				app->render->DrawText(dialogueFont, tmp1, (app->render->camera.w / 2) - ((strlen(tmp1) * 48) / 2) + 200, (app->render->camera.h / 2) - 64, 64, 1, white);
				app->render->DrawText(dialogueFont, tmp2, (app->render->camera.w / 2) - ((strlen(tmp2) * 48) / 2) + 200, (app->render->camera.h / 2), 64, 1, white);
                onceStatsFx = true;
            }
		}
	}
	else if (options)
	{
		app->render->DrawRectangle({ -app->render->camera.x,-app->render->camera.y,app->render->camera.w,app->render->camera.h }, 0, 0, 0, 200);
		char titleOptions[TEXT_LEN] = { 0 };
		strcpy_s(titleOptions, TEXT_LEN, "Options");
		app->render->DrawText(buttonFont, titleOptions, /*-app->render->camera.x +*/ ((app->render->camera.w - (strlen(titleOptions) * 24)) / 2), 100, 64, 2, { 255, 255, 255, 255 });
		sldrVolume->Draw(-app->render->camera.x, -app->render->camera.y);
		sldrFx->Draw(-app->render->camera.x, -app->render->camera.y);
		if (app->win->fullscreenWindow)
			boxFullScreen->checked = true;
		boxFullScreen->Draw(-app->render->camera.x, -app->render->camera.y);
		if (app->vsync)
			boxVSync->checked = true;
		boxVSync->Draw(-app->render->camera.x, -app->render->camera.y);
		btnBack->Draw(-app->render->camera.x, -app->render->camera.y);
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
	app->quests->Disable();

	RELEASE(dialogSystem);

	RELEASE(buttonFont);
	RELEASE(dialogueFont);
	app->tex->UnLoad(textBox);

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
		itemSelected = 0;
		btnInventory->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		changeMenu = true;
		flags = SetBit(flags, Flags::INVENTORY);
		break;
	case 2: //SKILLS
		app->gui->ResetButtons();
		itemSelected = 0;
		btnSkills->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::SKILLS);
		break;
	case 3: //SKILL TREE
		app->gui->ResetButtons();
		itemSelected = 0;
		btnSkillTree->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::SKILL_TREE);
		break;
	case 4: //QUESTS
		app->gui->ResetButtons();
		itemSelected = 0;
		btnQuests->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::QUESTS);
		break;
	case 5: //STATS
		app->gui->ResetButtons();
		itemSelected = 0;
		btnStats->state = GuiControlState::DISABLED;
		flags = 1 << Flags::MENU;
		flags = SetBit(flags, Flags::STATS);
		break;
	case 6: //OPTIONS
		flags = 1 << Flags::MENU;
		itemSelected = 0;
        options = true;
		app->audio->PlayMusic("Audio/Music/Options.ogg");
		changeMenu = true;
		app->gui->ResetButtons();
		usingGamepad = true;
		break;
	case 7: //TITLE SCREEN
		app->entities->inPause = false;
		itemSelected = 0;
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
	case 12: //BACK (OPTIONS BACK)
        options = false;
		if (strcmp(app->map->data.name, "tutorial.tmx") == 0) app->audio->PlayMusic("Audio/Music/Tutorial.ogg");
		else if (strcmp(app->map->data.name, "home.tmx") == 0) app->audio->PlayMusic("Audio/Music/Home.ogg");
		changeMenu = true;
		app->gui->ResetButtons();
		usingGamepad = true;
		break;
	case 13: //ITEM 1
		app->gui->ResetButtons();
		btnItem1->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem1->text);
		itemSelected = 1;
		break;
	case 14: //ITEM 2
		app->gui->ResetButtons();
		btnItem2->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem2->text);
		itemSelected = 2;
		break;
	case 15: //ITEM 3
		app->gui->ResetButtons();
		btnItem3->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem3->text);
		itemSelected = 3;
		break;
	case 16: //ITEM 4
		app->gui->ResetButtons();
		btnItem4->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem4->text);
		itemSelected = 4;
		break;
	case 17: //ITEM 5
		app->gui->ResetButtons();
		btnItem5->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem5->text);
		itemSelected = 5;
		break;
	case 18: //ITEM 6
		app->gui->ResetButtons();
		btnItem6->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem6->text);
		itemSelected = 6;
		break;
	case 19: //ITEM 7
		app->gui->ResetButtons();
		btnItem7->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem7->text);
		itemSelected = 7;
		break;
	case 20: //ITEM 8
		app->gui->ResetButtons();
		btnItem8->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem8->text);
		itemSelected = 8;
		break;
	case 21: //ITEM 9
		app->gui->ResetButtons();
		btnItem9->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem9->text);
		itemSelected = 9;
		break;
	case 22: //ITEM 10
		app->gui->ResetButtons();
		btnItem10->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem10->text);
		itemSelected = 10;
		break;
	case 23: //ITEM 11
		app->gui->ResetButtons();
		btnItem11->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem11->text);
		itemSelected = 11;
		break;
	case 24: //ITEM 12
		app->gui->ResetButtons();
		btnItem12->state = GuiControlState::DISABLED;
		btnInventory->state = GuiControlState::DISABLED;
		LOG("USED %s", btnItem12->text);
		itemSelected = 12;
		break;
	default:
		break;
	}
	return true;
}
