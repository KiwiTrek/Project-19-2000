#define _CRT_SECURE_NO_WARNINGS
#include "SceneCombat.h"

#include "Audio.h"
#include "EntityManager.h"
#include "CombatEntity.h"
#include "Player.h"
#include "Textures.h"
#include "GuiManager.h"
#include "SceneManager.h"
#include "SceneGameplay.h"
#include "Map.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Item.h"
#include "QuestManager.h"
#include "ParticleSystem.h"

SceneCombat::SceneCombat()
{
	flags = 0;
	white = { 255,255,255,255 };

    transitionTx = nullptr;
	for (int i = 0; i != 27; ++i)
	{
		transition.PushBack({ 600 * i, 0, 600, 338 });
	}
	transition.loop = false;
	transition.speed = 10.0f;

    combatTextBox = { 0,0,0,0 };
    combatMenuBox = { 0,0,0,0 };
    arrowCombat = { 0,0,0,0 };

	characterFlags = 0;
	characterFlags = SetBit(characterFlags, (uint)EntityId::MC);
	characterFlags = SetBit(characterFlags, (uint)EntityId::VIOLENT);

    currentEntity = nullptr;
}

SceneCombat::~SceneCombat()
{}

bool SceneCombat::Load()
{
	// COMBAT
	srand(time(NULL));

	char tmp[TEXT_LEN] = { 0 };
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderTexture, "GUI/combatGui.png");
	combatGui = app->tex->Load(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderTexture, "BattleTransition.png");
	transitionTx = app->tex->Load(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Smack.wav");
	smackFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Slap.wav");
	slapFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Confort.wav");
	confortFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Speech.wav");
	speechFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Smite.wav");
	smiteFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "MagicBlow.wav");
	magicBlowFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "MagicBarrage.wav");
	magicBarrageFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "MagicSlap.wav");
	magicSlapFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Stare.wav");
	stareFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Stronger.wav");
	strongerFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Blades.wav");
	bladesFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Stress.wav");
	stressFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "BadDream.wav");
	badDreamFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "CloseEyes.wav");
	closeEyesFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Dead.wav");
	deadFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "Grasp.wav");
	graspFx = app->audio->LoadFx(tmp);

	waitForTransition = TransitionStatus::NONE;
	scripted = false;
	backToGameplay = false;

	currentChar = nullptr;
	target = nullptr;
	currentEntity = nullptr;
	pastEntity = nullptr;

	menuCharacterBox = { 324,539,204,135 };
	mainChar.box = { 1280,0,204,190 };
	mainChar.characterTex = { 0,252,72,92 };
	mainChar.character = (CombatEntity*)app->entities->CreateEntity(36, app->render->camera.h - mainChar.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::MC, Stats(15, 0, 10, 10, 100, 100, 50, 100, 100, 0, 100));
	mainChar.x = 1280;
	mainChar.y = 80;
	sprintf_s(mainChar.hp, TEXT_LEN, "HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
	sprintf_s(mainChar.mp, TEXT_LEN, "MP: %d/%d", mainChar.character->stats.mPoints, mainChar.character->stats.mPointsMax);
	sprintf_s(mainChar.stress, TEXT_LEN, "ST: %d/%d", mainChar.character->stats.stress, mainChar.character->stats.stressMax);
	
	sprintf_s(mainChar.pAtk, TEXT_LEN, "Physical Attack: %d", mainChar.character->stats.pAtk);
	sprintf_s(mainChar.mAtk, TEXT_LEN, "Magic Attack: %d", mainChar.character->stats.mAtk);
	sprintf_s(mainChar.pDef, TEXT_LEN, "Physical Defense: %d", mainChar.character->stats.pDef);
	sprintf_s(mainChar.mDef, TEXT_LEN, "Magic Defense: %d", mainChar.character->stats.mDef);
	sprintf_s(mainChar.speed, TEXT_LEN, "Speed: %d", mainChar.character->stats.speed);

	grandpa.box = { 1280,0,204,190 };
	grandpa.characterTex = { 73,252,68,100 };
	grandpa.character = (CombatEntity*)app->entities->CreateEntity(grandpa.box.w + 36, app->render->camera.h - grandpa.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::VIOLENT, Stats(25, 25, 0, 0, 100, 100, 25, 100, 100));
	grandpa.x = 1880;
	grandpa.y = 80 + menuCharacterBox.h;
	sprintf_s(grandpa.hp, TEXT_LEN, "HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
	sprintf_s(grandpa.mp, TEXT_LEN, "MP: %d/%d", grandpa.character->stats.mPoints, grandpa.character->stats.mPointsMax);

	sprintf_s(grandpa.pAtk, TEXT_LEN, "Physical Attack: %d", grandpa.character->stats.pAtk);
	sprintf_s(grandpa.mAtk, TEXT_LEN, "Magic Attack: %d", grandpa.character->stats.mAtk);
	sprintf_s(grandpa.pDef, TEXT_LEN, "Physical Defense: %d", grandpa.character->stats.pDef);
	sprintf_s(grandpa.mDef, TEXT_LEN, "Magic Defense: %d", grandpa.character->stats.mDef);
	sprintf_s(grandpa.speed, TEXT_LEN, "Speed: %d", grandpa.character->stats.speed);

	combatTextBox = { 0,0,1280,248 };
	combatMenuBox = { 305,249,1001,130 };
	btnCombatAttack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 28, { 34,505,200,60 }, "ATTACK", 40, this);
	btnCombatSkills = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 29, { 301,505,200,60 }, "SKILLS", 40, this);
	btnCombatItems = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 30, { 568,505,200,60 }, "ITEMS", 40, this);
	btnCombatFlee = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 31, { 835,505,200,60 }, "FLEE", 40, this);

	btnCombatSkill1 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 32, { 84,570,300,60 }, "skill 1", 32, this, 0, app->entities->itemAtlas);
	btnCombatSkill2 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 33, { 384,570,300,60 }, "skill 2", 32, this, 0, app->entities->itemAtlas);
	btnCombatSkill3 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 34, { 684,570,300,60 }, "skill 3", 32, this, 0, app->entities->itemAtlas);
	btnCombatSkill4 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 35, { 84,630,300,60 }, "skill 4", 32, this, 0, app->entities->itemAtlas);
	//btnCombatSkill5 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 56, { 384,630,300,60 }, "skill 5", 40, this);
	//btnCombatSkill6 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 57, { 684,630,300,60 }, "skill 6", 40, this);

	btnCombatItem1 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 36, { 84,570,300,60 }, "item one", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem2 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 37, { 384,570,300,60 }, "item two", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem3 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 38, { 684,570,300,60 }, "item three", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem4 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 39, { 84,630,300,60 }, "item four", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem5 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 40, { 384,630,300,60 }, "item five", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem6 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 41, { 684,630,300,60 }, "item six", 40, this, 0, app->entities->itemAtlas);

	btnRightArrow = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 42, { 984,605,50,50 }, "", 40, this);
	btnLeftArrow = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 43, { 34,605,50,50 }, "", 40, this);

	btnCombatItem7 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 44, { 84,570,300,60 }, "item seven", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem8 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 45, { 384,570,300,60 }, "item eight", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem9 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 46, { 684,570,300,60 }, "item nine", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem10 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 47, { 84,630,300,60 }, "item ten", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem11 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 48, { 384,630,300,60 }, "item eleven", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem12 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 49, { 684,630,300,60 }, "item twelve", 40, this, 0, app->entities->itemAtlas);

	LOG("%d", characterFlags);

	heDed = false;
	pastStress = -1;
	stressStatus = StressStatus::UNDER;

	return false;
}

bool SceneCombat::Start(EntityId id1, EntityId id2, EntityId id3)
{
	currentChar = nullptr;
	target = nullptr;
	targetFrameTime = 0.0f;
	increaseSelect = true;
	turnFrameTime = 0.0f;
	increaseTurn = true;
	currentEntity = nullptr;
	currentTarget = nullptr;
	pastEntity = nullptr;
	characterSelected = false;
	targetAttack = false;
	finishedAction = false;
	hasTicked = false;
	memset(attackSelected, 0, TEXT_LEN);
	combatMenuFlags = 0;
	once = true;
	scripted = false;
	itemSelected = 0;
	pageOne = true;
	targetItem = false;

	backToGameplay = false;
	alpha = 1.0f;
	wait = false;
	waitForTransition = TransitionStatus::SCENE;

	memset(firstLine, 0, TEXT_LEN);
	memset(secondLine, 0, TEXT_LEN);
	memset(thirdLine, 0, TEXT_LEN);
	arrowCombat = { 1280,192,50,50 };
	blink = false;
	blinkTimer = 0.0f;

	ListItem<CombatEntity*>* e = turnOrder.start;
	while (e != nullptr)
	{
		ListItem<CombatEntity*>* eNext = e->next;
		turnOrder.Del(e);
		e = eNext;
	}
	turnOrder.Clear();
	enemiesList.Clear();

	app->audio->PlayMusic("Audio/Music/Battle.ogg");
	SpawnEnemies(id1, id2, id3);

	app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatAttack));
	usingGamepad = false;
	if (strcmp(app->input->GetControllerName(), "unplugged") != 0) usingGamepad = true;
	app->input->mouseMotionX = 0;
	app->input->mouseMotionY = 0;

	heDed = false;

	combatState = CombatStateType::COMBAT_START;

	return true;
}

bool SceneCombat::Update(float dt)
{
	if (waitForTransition == TransitionStatus::SCENE || waitForTransition == TransitionStatus::BATTLE)
	{
		UpdateTransition(dt,TransitionStatus::BATTLE);
	}
	else if (waitForTransition == TransitionStatus::END)
	{
		//WAIT FOR PLAYER INPUT
		if (wait)
		{
			if (app->input->CheckButton("select", KEY_DOWN)) wait = false;

			blinkTimer += dt;
			if (blinkTimer >= 0.5f)
			{
				blinkTimer = 0.0f;
				blink = !blink;
			}
		}
		else
		{
			if (app->input->CheckButton("cancel", KEY_DOWN))
			{
				characterSelected = true;
				targetAttack = false;
				targetItem = false;
				combatMenuFlags = 0;
				changeMenu = true;
				app->gui->ResetButtons();
				memset(attackSelected, 0, TEXT_LEN);
				if (itemSelected != 0)
				{
					btnCombatItems->state = GuiControlState::DISABLED;
					btnLeftArrow->state = GuiControlState::DISABLED;
					combatMenuFlags = SetBit(combatMenuFlags, Flags::ITEMS);
					itemSelected = 0;
				}
			}

			switch (combatState)
			{
			case CombatStateType::COMBAT_START:
			{
				//CHECK WHO IS FIRST
				ListItem<Entity*>* e = app->entities->entities.start;

				while (e != nullptr)
				{
					//ADDS THEM TO THE LIST
					if (e->data->type == EntityType::COMBAT_ENTITY)
					{
						CombatEntity* cEntity = (CombatEntity*)e->data;
						if (cEntity->stats.hPoints > 0) turnOrder.Add(cEntity);
					}
					e = e->next;
				}

				ListItem<CombatEntity*>* l = turnOrder.start;
				while (l != nullptr)
				{
					//ADDS THEM TO THE LIST
					if (!IsCharacter(l->data))
					{
						enemiesList.Add(l->data);
					}
					l = l->next;
				}
				l = turnOrder.start;
				while (l != nullptr)
				{
					//ADDS THEM TO THE LIST
					if (IsCharacter(l->data))
					{
						enemiesList.Add(l->data);
					}
					l = l->next;
				}

				SortSpeed(false);
				combatState = CombatStateType::COMBAT_MIDGAME;
			}
			break;
			case CombatStateType::COMBAT_MIDGAME:
			{
				if (currentEntity == nullptr)
				{
					SortSpeed(false);
					currentEntity = turnOrder.start;
					if (IsCharacter(currentEntity->data))
					{
						switch (currentEntity->data->id)
						{
						case EntityId::MC:
							currentChar = &mainChar;
							break;
						case EntityId::VIOLENT:
							currentChar = &grandpa;
							break;
						default:
							break;
						}
					}
				}
				else
				{
					ListItem<CombatEntity*>* e = turnOrder.start;
					while (e != nullptr)
					{
						ListItem<CombatEntity*>* eNext = e->next;
						if (e->data->stats.hPoints <= 0)
						{
							LOG("%s is dead!", e->data->name);
							char tmp[50];
							sprintf_s(tmp, TEXT_LEN, "%s is dead!", e->data->name);
							NextLine(tmp);
							app->audio->PlayFx(deadFx);
							wait = true;
							currentChar = nullptr;
							characterSelected = false;

							if (IsCharacter(e->data))
							{
								combatState = CombatStateType::COMBAT_END;
								currentEntity = e;
								TickDownBuffs();
								combatState = CombatStateType::COMBAT_MIDGAME;
							}
							else app->entities->DestroyEntity(e->data);

							enemiesList.Del(enemiesList.At(enemiesList.Find(e->data)));
							turnOrder.Del(turnOrder.At(turnOrder.Find(e->data)));
							SortSpeed(false);
							if (currentEntity == nullptr) currentEntity = eNext;
						}
						e = eNext;
					}

					VictoryCondition();
					DefeatCondition();

					if (combatState == CombatStateType::COMBAT_END)
					{
						ListItem<CombatEntity*>* e = turnOrder.start;
						while (e != nullptr)
						{
							currentEntity = e;
							TickDownBuffs();
							e = e->next;
						}
					}
					else if (combatState != CombatStateType::COMBAT_END)
					{
						if (!hasTicked)
						{
							StressPower();
							TickDownBuffs();
							hasTicked = true;
							ListItem<CombatEntity*>* e = turnOrder.start;
							while (e != nullptr)
							{
								LOG("%s", e->data->name);
								e = e->next;
							}
						}

						if (IsCharacter(currentEntity->data))													// CHARACTER
						{
							if (once) //i think we should use the flags menu to turn this true only when there are no "main buttons" selected
							{
								once = false;
								characterSelected = true;
								changeMenu = true;

								if (currentEntity->data->id == EntityId::MC) NextLine("It's your turn!");
								else
								{
									char tmp[50];
									sprintf_s(tmp, TEXT_LEN, "It's %s's turn!", currentEntity->data->name);
									NextLine(tmp);
								}
							}

							//Item logic
							if (itemSelected != 0)
							{
								if (target == nullptr)
									SelectTarget();
								else
								{
									app->gui->ResetButtons();
									items.At(itemSelected - 1)->data->Use(target);
									char tmp[50];
									sprintf_s(tmp, TEXT_LEN, "%s used %s item.", currentEntity->data->name, items.At(itemSelected - 1)->data->effect.attackName);
									NextLine(tmp);
									if (items.At(itemSelected - 1)->data->count == 0) items.Del(items.At(itemSelected - 1));
									fPoint targetPos(target->entityRect.x + (target->entityRect.w / 2), target->entityRect.y + (target->entityRect.h / 2));
									switch (items.At(itemSelected - 1)->data->id)
									{
									case ItemId::NONE:
										break;
									case ItemId::HP_POTION:
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::HEAL);
										app->audio->PlayFx(app->entities->itemFx);
										break;
									case ItemId::MANA_POTION:
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::HEAL);
										app->audio->PlayFx(app->entities->itemFx);
										break;
									case ItemId::ELIXIR:
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::HEAL);
										app->audio->PlayFx(app->entities->itemFx);
										break;
									case ItemId::GRANDMA_STEW:
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::HEAL);
										app->audio->PlayFx(app->entities->itemFx);
										break;
									case ItemId::BOTTLED_SMITE:
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::NOVA);
										app->audio->PlayFx(smiteFx);
										break;
									case ItemId::HAPPILLS:
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::HEAL);
										app->audio->PlayFx(app->entities->itemFx);
										break;
									case ItemId::PHYS_BUFFER:
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::BUFF);
										app->audio->PlayFx(strongerFx);
										break;
									case ItemId::MAGIC_BUFFER:
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::BUFF);
										app->audio->PlayFx(strongerFx);
										break;
									default:
										break;
									}


									itemSelected = 0;
									target = nullptr;
									targetItem = false;
									finishedAction = true;
									wait = true;
								}
							}

							switch (currentEntity->data->id)
							{
							case EntityId::MC:
								currentChar = &mainChar;

								if (currentEntity->data->isStunned != 0)
								{
									finishedAction = true;
									char tmp[75];
									sprintf_s(tmp, TEXT_LEN, "%s is stunned and can't move!", currentEntity->data->name);
									NextLine(tmp);
								}
								else
								{
									if (currentEntity->data->isTaunted != 0)
									{
										if (currentEntity->data->isTaunted == 1) currentEntity->data->isTaunted = 0;
										target = currentEntity->data->tauntedBy;
										memset(attackSelected, 0, TEXT_LEN);
										char tmp[75];
										sprintf_s(tmp, TEXT_LEN, "%s is taunted by %s!", currentEntity->data->name, target->name);
										NextLine(tmp);
									}

									if (target == nullptr)
									{
										if (strcmp(attackSelected, "") != 0)
										{
											targetAttack = true;
											SelectTarget();
										}
									}
									else if (itemSelected == 0)
									{
										if (strcmp(attackSelected, "Smack") == 0)
										{
											LOG("%d", currentEntity->data->stats.pAtk);
											app->audio->PlayFx(smackFx);
											Damage(target, currentEntity->data->stats.pAtk, false);
											fPoint targetPos(target->collider->rect.x + target->collider->rect.w / 2, target->collider->rect.y + target->collider->rect.h / 2);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::SLASH);
										}
										else if (strcmp(attackSelected, "Comfort") == 0)
										{
											app->audio->PlayFx(confortFx);
											float p = rand() % 10 + 11;
											Heal(target, p / 100);
											fPoint targetPos(target->collider->rect.x + target->collider->rect.w / 2, target->collider->rect.y + target->collider->rect.h / 2);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::HEAL);
											ManaCost(10);
										}
										else if (strcmp(attackSelected, "Slap") == 0)
										{
											app->audio->PlayFx(slapFx);
											Damage(target, currentEntity->data->stats.pAtk, false);
											fPoint targetPos(target->collider->rect.x + target->collider->rect.w / 2, target->collider->rect.y + target->collider->rect.h / 2);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::SLASH);
											ManaCost(10);
										}
										else if (strcmp(attackSelected, "Encourage") == 0)
										{
											app->audio->PlayFx(confortFx);
											LOG("%s buffs %s!", currentEntity->data->name, target->name);
											char tmp[75];
											sprintf_s(tmp, TEXT_LEN, "%s buffs %s!", currentEntity->data->name, target->name);
											NextLine(tmp);
											Attack* a = new Attack("25 buff");
											a->turns = 3;
											a->beforeBuff1 = target->stats.pAtk;
											a->beforeBuff2 = target->stats.mAtk;
											target->attackPool.Add(a);
											target->stats.pAtk += (target->stats.pAtk * 25) / 100;
											target->stats.mAtk += (target->stats.mAtk * 25) / 100;
											fPoint targetPos(target->collider->rect.x + target->collider->rect.w / 2, target->collider->rect.y + target->collider->rect.h / 2);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::BUFF);
											ManaCost(10);
										}
										else if (strcmp(attackSelected, "Boring Speech") == 0)
										{
											app->audio->PlayFx(speechFx);
											LOG("%s debuffs %s!", currentEntity->data->name, target->name);
											char tmp[75];
											sprintf_s(tmp, TEXT_LEN, "%s debuffs %s!", currentEntity->data->name, target->name);
											NextLine(tmp);
											Attack* a = new Attack("10 debuff defenses");
											a->turns = 3;
											a->beforeBuff1 = target->stats.pAtk;
											a->beforeBuff2 = target->stats.mAtk;
											target->attackPool.Add(a);
											target->stats.pDef -= (target->stats.pDef * 10) / 100;
											target->stats.mDef -= (target->stats.mDef * 10) / 100;
											fPoint targetPos(target->collider->rect.x + target->collider->rect.w / 2, target->collider->rect.y + target->collider->rect.h / 2);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::DEBUFF);
											ManaCost(10);
										}
										finishedAction = true;
										wait = true;
										if (target != nullptr)
										{
											target = nullptr;
											targetAttack = false;
											currentTarget = nullptr;
											memset(attackSelected, 0, TEXT_LEN);
										}
									}
								}
								break;
							case EntityId::VIOLENT:
								currentChar = &grandpa;

								if (currentEntity->data->isStunned != 0)
								{
									finishedAction = true;
									char tmp[75];
									sprintf_s(tmp, TEXT_LEN, "%s is stunned and can't move!", currentEntity->data->name);
									NextLine(tmp);
								}
								else
								{
									if (currentEntity->data->isTaunted != 0)
									{
										if (currentEntity->data->isTaunted == 1) currentEntity->data->isTaunted = 0;
										target = currentEntity->data->tauntedBy;
										memset(attackSelected, 0, TEXT_LEN);
										char tmp[75];
										sprintf_s(tmp, TEXT_LEN, "%s is taunted by %s!", currentEntity->data->name, target->name);
										NextLine(tmp);
									}

									if (strcmp(attackSelected, "W. Magic Barrage") == 0)
									{
										app->audio->PlayFx(magicBarrageFx);
										ListItem<CombatEntity*>* e = turnOrder.start;
										while (e != nullptr)
										{
											if (!IsCharacter(e->data))
											{
												Damage(e->data, currentEntity->data->stats.mAtk / 1.4f, true);
												fPoint targetPos(e->data->collider->rect.x + e->data->collider->rect.w / 2, e->data->collider->rect.y + e->data->collider->rect.h / 2);
												app->particles->AddEmitter(targetPos, EmitterData::EmitterType::MAGIC);
											}
											e = e->next;
										}
										ManaCost(10);
										finishedAction = true;
										wait = true;
										if (target != nullptr)
										{
											target = nullptr;
											targetAttack = false;
											currentTarget = nullptr;
											memset(attackSelected, 0, TEXT_LEN);
										}
									}
									else if (target == nullptr)
									{
										if (strcmp(attackSelected, "") != 0)
										{
											targetAttack = true;
											SelectTarget();
										}
									}
									else if (itemSelected == 0)
									{
										if (strcmp(attackSelected, "Smite foes") == 0)
										{
											LOG("%d", currentEntity->data->stats.mAtk);
											app->audio->PlayFx(smiteFx);
											Damage(target, currentEntity->data->stats.mAtk, true);
											fPoint targetPos(target->collider->rect.x + target->collider->rect.w / 2, target->collider->rect.y + target->collider->rect.h / 2);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::MAGIC);
										}
										else if (strcmp(attackSelected, "W. Magic missile") == 0)
										{
											app->audio->PlayFx(magicBlowFx);
											Damage(target, currentEntity->data->stats.mAtk / 1.3f, true);
											Damage(target, currentEntity->data->stats.mAtk / 1.3f, true);
											Damage(target, currentEntity->data->stats.mAtk / 1.3f, true);
											fPoint targetPos(target->collider->rect.x + target->collider->rect.w / 2, target->collider->rect.y + target->collider->rect.h / 2);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::MAGIC);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::MAGIC);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::MAGIC);
											ManaCost(10);
										}
										else if (strcmp(attackSelected, "Magic Hand Slap") == 0)
										{
											app->audio->PlayFx(magicSlapFx);
											Damage(target, currentEntity->data->stats.mAtk / 1.1f, true);
											Damage(target, currentEntity->data->stats.mAtk / 1.1f, true);
											fPoint targetPos(target->collider->rect.x + target->collider->rect.w / 2, target->collider->rect.y + target->collider->rect.h / 2);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::MAGIC);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::MAGIC);
											ManaCost(10);
										}
										else if (strcmp(attackSelected, "Judgemental Stare") == 0)
										{
											app->audio->PlayFx(stareFx);
											Damage(target, currentEntity->data->stats.mAtk * 1.5f, true);
											fPoint targetPos(target->collider->rect.x + target->collider->rect.w / 2, target->collider->rect.y + target->collider->rect.h / 2);
											app->particles->AddEmitter(targetPos, EmitterData::EmitterType::MAGIC);
											ManaCost(10);
										}
										finishedAction = true;
										wait = true;
										if (target != nullptr)
										{
											target = nullptr;
											targetAttack = false;
											currentTarget = nullptr;
											memset(attackSelected, 0, TEXT_LEN);
										}
									}
								}
								break;
							case EntityId::STUBBORN:
								finishedAction = true;
								break;
							case EntityId::KIND:
								finishedAction = true;
								break;
							default:
								break;
							}
						}
						else																					// ENEMY
						{
							//ENEMY ATTACK PATTERN?
							switch (currentEntity->data->id)
							{
							case EntityId::STRESSING_SHADOW:
							{
								int p = rand() % 10 + 1;
								if (p >= 6) //Stressing attack
								{
									app->audio->PlayFx(stressFx);
									Stress(10);
								}
								else //Magical blow
								{
									app->audio->PlayFx(magicBlowFx);

									int t = EnemyTarget();
									switch (t)
									{
									case 1: //MC
									{
										Damage(mainChar.character, currentEntity->data->stats.mAtk, true);
										fPoint targetPos(mainChar.character->collider->rect.x + mainChar.character->collider->rect.w / 2, mainChar.character->collider->rect.y + mainChar.character->collider->rect.h / 2);
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::SLASH);
										break;
									}
									case 2: //GRANDPA
									{
										Damage(grandpa.character, currentEntity->data->stats.mAtk, true);
										fPoint targetPos(grandpa.character->collider->rect.x + grandpa.character->collider->rect.w / 2, grandpa.character->collider->rect.y + grandpa.character->collider->rect.h / 2);
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::SLASH);
										break;
									}
									//case 3:
									//	break;
									//case 4:
									//	break;
									default:
										break;
									}
								}
								finishedAction = true;
								wait = true;
								break;
							}
							case EntityId::FURIOUS_SHADOW:
							{
								int p = rand() % 10 + 1;
								if (p >= 5) //Getting stronger
								{
									app->audio->PlayFx(strongerFx);
									LOG("Furious Shadow got stronger!");
									NextLine("Furious Shadow got stronger!");
									Attack* a = new Attack("5 buff defenses");
									a->turns = 2;
									currentEntity->data->attackPool.Add(a);
									currentEntity->data->stats.pDef += (currentEntity->data->stats.pDef * 5) / 100;
									currentEntity->data->stats.mDef += (currentEntity->data->stats.mDef * 5) / 100;
									fPoint targetPos(currentEntity->data->collider->rect.x + currentEntity->data->collider->rect.w / 2, currentEntity->data->collider->rect.y + currentEntity->data->collider->rect.h / 2);
									app->particles->AddEmitter(targetPos, EmitterData::EmitterType::BUFF);
								}
								else //Fury of blades
								{
									app->audio->PlayFx(bladesFx);
									Damage(mainChar.character, currentEntity->data->stats.pAtk, false);
									Damage(grandpa.character, currentEntity->data->stats.pAtk, false);
									LOG("Furious Shadow attack to all!\n");
									NextLine("Furious Shadow attack to all!");
									fPoint targetPos(mainChar.character->collider->rect.x + mainChar.character->collider->rect.w / 2, mainChar.character->collider->rect.y + mainChar.character->collider->rect.h / 2);
									app->particles->AddEmitter(targetPos, EmitterData::EmitterType::SLASH);
									fPoint targetPos2(grandpa.character->collider->rect.x + grandpa.character->collider->rect.w / 2, grandpa.character->collider->rect.y + grandpa.character->collider->rect.h / 2);
									app->particles->AddEmitter(targetPos2, EmitterData::EmitterType::SLASH);
								}
								finishedAction = true;
								wait = true;
								break;
							}
							case EntityId::NIGHTMARE:
							{
								int p = rand() % 100 + 1;
								if (p >= 60) //Bad dream
								{
									app->audio->PlayFx(badDreamFx);

									int t = EnemyTarget();
									switch (t)
									{
									case 1: //MC
									{
										Damage(mainChar.character, currentEntity->data->stats.pAtk, false);
										fPoint targetPos(mainChar.character->collider->rect.x + mainChar.character->collider->rect.w / 2, mainChar.character->collider->rect.y + mainChar.character->collider->rect.h / 2);
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::SLASH);
										break;
									}
									case 2: //GRANDPA
									{
										Damage(grandpa.character, currentEntity->data->stats.pAtk, false);
										fPoint targetPos2(grandpa.character->collider->rect.x + grandpa.character->collider->rect.w / 2, grandpa.character->collider->rect.y + grandpa.character->collider->rect.h / 2);
										app->particles->AddEmitter(targetPos2, EmitterData::EmitterType::SLASH);
										break;
									}
									//case 3:
									//	break;
									//case 4:
									//	break;
									default:
										break;
									}
								}
								else if (p >= 30) //Nightmarish
								{
									app->audio->PlayFx(strongerFx);
									LOG("Nightmarish got stronger!");
									NextLine("Nightmarish got stronger!");
									Attack* a = new Attack("10 buff defenses");
									a->turns = 2;
									currentEntity->data->attackPool.Add(a);
									currentEntity->data->stats.pDef += (currentEntity->data->stats.pDef / 10) * 100;
									currentEntity->data->stats.mDef += (currentEntity->data->stats.mDef / 10) * 100;
									fPoint targetPos(currentEntity->data->collider->rect.x + currentEntity->data->collider->rect.w / 2, currentEntity->data->collider->rect.y + currentEntity->data->collider->rect.h / 2);
									app->particles->AddEmitter(targetPos, EmitterData::EmitterType::BUFF);
								}
								else if (p >= 10) //Close your eyes
								{
									app->audio->PlayFx(closeEyesFx);

									int t = EnemyTarget();
									switch (t)
									{
									case 1: //MC
									{
										LOG("Nightmarish used Close Your Eyes on You!");
										NextLine("Nightmarish used Close Your Eyes on You!");
										mainChar.character->isTaunted = 1;
										mainChar.character->tauntedBy = currentEntity->data;
										fPoint targetPos(mainChar.character->collider->rect.x + mainChar.character->collider->rect.w / 2, mainChar.character->collider->rect.y + mainChar.character->collider->rect.h / 2);
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::TAUNT);
										break;
									}
									case 2: //GRANDPA
									{
										LOG("Nightmarish used Close Your Eyes on Grandpa!");
										NextLine("Nightmarish used Close Your Eyes on Grandpa!");
										grandpa.character->isTaunted = 1;
										grandpa.character->tauntedBy = currentEntity->data;
										fPoint targetPos(grandpa.character->collider->rect.x + grandpa.character->collider->rect.w / 2, grandpa.character->collider->rect.y + grandpa.character->collider->rect.h / 2);
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::TAUNT);
										break;
									}
									//case 3:
									//	break;
									//case 4:
									//	break;
									default:
										break;
									}
								}
								else //Grasp of depression
								{
									app->audio->PlayFx(graspFx);

									int t = EnemyTarget();
									switch (t)
									{
									case 1: //MC
									{
										LOG("Nightmarish used Grasp of Depression on You!");
										NextLine("Nightmarish used Grasp of Depression on You!");
										mainChar.character->isStunned = 1;
										Attack* a = new Attack("10 debuff defenses");
										a->turns = 1;
										mainChar.character->attackPool.Add(a);
										mainChar.character->stats.pDef -= (mainChar.character->stats.pDef / 10) * 100;
										mainChar.character->stats.mDef -= (mainChar.character->stats.mDef / 10) * 100;
										fPoint targetPos(mainChar.character->collider->rect.x + mainChar.character->collider->rect.w / 2, mainChar.character->collider->rect.y + mainChar.character->collider->rect.h / 2);
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::STUN);
										break;
									}
									case 2: //GRANDPA
									{
										LOG("Nightmarish used Grasp of Depression on Grandpa!");
										NextLine("Nightmarish used Grasp of Depression on Grandpa!");
										grandpa.character->isStunned = 1;
										Attack* a = new Attack("10 debuff defenses");
										a->turns = 1;
										grandpa.character->attackPool.Add(a);
										grandpa.character->stats.pDef -= (grandpa.character->stats.pDef / 10) * 100;
										grandpa.character->stats.mDef -= (grandpa.character->stats.mDef / 10) * 100;
										fPoint targetPos(grandpa.character->collider->rect.x + grandpa.character->collider->rect.w / 2, grandpa.character->collider->rect.y + grandpa.character->collider->rect.h / 2);
										app->particles->AddEmitter(targetPos, EmitterData::EmitterType::STUN);
										break;
									}
									//case 3:
									//	break;
									//case 4:
									//	break;
									default:
										break;
									}
								}
								finishedAction = true;
								wait = true;
								break;
							}
							default:
								break;
							}
						}

						if (finishedAction)
						{
							//if (currentEntity != nullptr) LOG("HP: %d/%d", currentEntity->data->stats.hPoints, currentEntity->data->stats.hPointsMax);
							finishedAction = false;
							hasTicked = false;
							memset(attackSelected, 0, TEXT_LEN);
							once = true;
							currentEntity = currentEntity->next;
						}
					}
				}
			}
			break;
			case CombatStateType::COMBAT_END:
			{
				if (enemy1 != nullptr) enemiesCount++;
				if (enemy2 != nullptr) enemiesCount++;
				if (enemy3 != nullptr) enemiesCount++;

				if (!heDed && ((app->quests->tpFlags & QuestManager::TpFlags::FIGHT) != 0))
					app->quests->boolFlags |= QuestManager::BoolFlags::FIRSTCOMBAT;

				if (!heDed && (enemiesCount == 3) && (enemy2->id == EntityId::NIGHTMARE) && ((app->quests->tpFlags & QuestManager::TpFlags::BOSS_FIGHT) != 0))
					app->quests->boolFlags |= QuestManager::BoolFlags::FIRSTBOSS;


				Finish();
				break;
			}
			default:
				break;
			}
		}

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

		// Calls update for both gamepad and mouse parameters (GUI)
		if (usingGamepad && changeMenu && btnCombatItems->state != GuiControlState::DISABLED)
		{
			app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatAttack));
			changeMenu = false;
		}

		if (characterSelected)
		{
			if (usingGamepad) app->scene->currentButton->data->Update(dt, 28, 31);
			else
			{
				btnCombatAttack->Update(dt);
				btnCombatSkills->Update(dt);
				btnCombatItems->Update(dt);
				btnCombatFlee->Update(dt);
			}

			if ((combatMenuFlags & 1 << Flags::SKILL) != 0)
			{
				int i = 1;
				int maxSkills = 5;
				for (i; i < maxSkills; i++)
				{
					switch (i)
					{
					case 1:
						memset(btnCombatSkill1->text, 0, TEXT_LEN);
						strcpy_s(btnCombatSkill1->text, TEXT_LEN, currentChar->character->attackPool.At(i)->data->attackName);
						btnCombatSkill1->sec = SkillSec(i, currentChar->character->id);
						if (!usingGamepad) btnCombatSkill1->Update(dt);
						break;
					case 2:
						memset(btnCombatSkill2->text, 0, TEXT_LEN);
						strcpy_s(btnCombatSkill2->text, TEXT_LEN, currentChar->character->attackPool.At(i)->data->attackName);
						btnCombatSkill2->sec = SkillSec(i, currentChar->character->id);
						if (!usingGamepad) btnCombatSkill2->Update(dt);
						break;
					case 3:
						memset(btnCombatSkill3->text, 0, TEXT_LEN);
						strcpy_s(btnCombatSkill3->text, TEXT_LEN, currentChar->character->attackPool.At(i)->data->attackName);
						btnCombatSkill3->sec = SkillSec(i, currentChar->character->id);
						if (!usingGamepad) btnCombatSkill3->Update(dt);
						break;
					case 4:
						memset(btnCombatSkill4->text, 0, TEXT_LEN);
						strcpy_s(btnCombatSkill4->text, TEXT_LEN, currentChar->character->attackPool.At(i)->data->attackName);
						btnCombatSkill4->sec = SkillSec(i, currentChar->character->id);
						if (!usingGamepad) btnCombatSkill4->Update(dt);
						break;
						//case 5:
							//memset(btnCombatSkill5->text, 0, TEXT_LEN);
							//strcpy_s(btnCombatSkill5->text, TEXT_LEN, currentChar->character->attackPool.At(i)->data->attackName);
							//btnCombatSkill5->sec = SkillSec(i, currentChar->character->id);
							//btnCombatSkill5->Update(dt);
							//break;
						//case 6:
							//memset(btnCombatSkill6->text, 0, TEXT_LEN);
							//strcpy_s(btnCombatSkill6->text, TEXT_LEN, currentChar->character->attackPool.At(i)->data->attackName);
							//btnCombatSkill6->sec = SkillSec(i, currentChar->character->id);
							//btnCombatSkill6->Update(dt);
							//break;
					}
				}

				if (usingGamepad)
				{
					if (changeMenu)
					{
						app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatSkill1));
						changeMenu = false;
					}
					else
					{
						app->scene->currentButton->data->Update(dt, 32, 35);
						if (app->input->CheckButton("cancel", KeyState::KEY_DOWN))
						{
							app->gui->ResetButtons();
							app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatAttack));
							combatMenuFlags = 0;
							changeMenu = true;
						}
					}
				}
			}
			else if ((combatMenuFlags & 1 << Flags::ITEMS) != 0)
			{
				int i = 0;
				int maxItems = -1;
				if (pageOne)
				{
					i = 0;
					if (items.Count() > 6) maxItems = 6;
					else maxItems = items.Count();
				}
				else
				{
					i = 6;
					maxItems = items.Count();
				}
				for (i; i < maxItems; i++)
				{
					switch (i)
					{
					case 0:
						btnCombatItem1->itemId = items.At(i)->data->id;
						memset(btnCombatItem1->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem1->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem1->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem1->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem1->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem1;
						if (!usingGamepad) btnCombatItem1->Update(dt);
						break;
					case 1:
						btnCombatItem2->itemId = items.At(i)->data->id;
						memset(btnCombatItem2->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem2->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem2->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem2->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem2->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem2;
						if (!usingGamepad) btnCombatItem2->Update(dt);
						break;
					case 2:
						btnCombatItem3->itemId = items.At(i)->data->id;
						memset(btnCombatItem3->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem3->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem3->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem3->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem3->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem3;
						if (!usingGamepad) btnCombatItem3->Update(dt);
						break;
					case 3:
						btnCombatItem4->itemId = items.At(i)->data->id;
						memset(btnCombatItem4->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem4->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem4->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem4->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem4->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem4;
						if (!usingGamepad) btnCombatItem4->Update(dt);
						break;
					case 4:
						btnCombatItem5->itemId = items.At(i)->data->id;
						memset(btnCombatItem5->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem5->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem5->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem5->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem5->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem5;
						if (!usingGamepad) btnCombatItem5->Update(dt);
						break;
					case 5:
						btnCombatItem6->itemId = items.At(i)->data->id;
						memset(btnCombatItem6->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem6->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem6->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem6->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem6->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem6;
						if (!usingGamepad) btnCombatItem6->Update(dt);
						break;
					case 6:
						btnCombatItem7->itemId = items.At(i)->data->id;
						memset(btnCombatItem7->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem7->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem7->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem7->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem7->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem7;
						if (!usingGamepad) btnCombatItem7->Update(dt);
						break;
					case 7:
						btnCombatItem8->itemId = items.At(i)->data->id;
						memset(btnCombatItem8->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem8->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem8->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem8->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem8->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem8;
						if (!usingGamepad) btnCombatItem8->Update(dt);
						break;
					case 8:
						btnCombatItem9->itemId = items.At(i)->data->id;
						memset(btnCombatItem9->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem9->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem9->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem9->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem9->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem9;
						if (!usingGamepad) btnCombatItem9->Update(dt);
						break;
					case 9:
						btnCombatItem10->itemId = items.At(i)->data->id;
						memset(btnCombatItem10->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem10->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem10->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem10->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem10->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem10;
						if (!usingGamepad) btnCombatItem10->Update(dt);
						break;
					case 10:
						btnCombatItem11->itemId = items.At(i)->data->id;
						memset(btnCombatItem11->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem11->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem11->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem11->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem11->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem11;
						if (!usingGamepad) btnCombatItem11->Update(dt);
						break;
					case 11:
						btnCombatItem12->itemId = items.At(i)->data->id;
						memset(btnCombatItem12->text, 0, TEXT_LEN);
						strcpy_s(btnCombatItem12->text, TEXT_LEN, items.At(i)->data->effect.attackName);
						memset(btnCombatItem12->count, 0, TEXT_LEN);
						strcpy_s(btnCombatItem12->count, TEXT_LEN, items.At(i)->data->countText);
						btnCombatItem12->sec = items.At(i)->data->texSec;
						items.At(i)->data->button = btnCombatItem12;
						if (!usingGamepad) btnCombatItem12->Update(dt);
						break;
					default:
						break;
					}
				}
				if (!usingGamepad && items.Count() >= 6)
				{
					btnLeftArrow->Update(dt);
					btnRightArrow->Update(dt);
				}

				if (usingGamepad)
				{
					if (changeMenu)
					{
						app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatItem1));
						changeMenu = false;
					}
					else
					{
						if (items.Count() >= 6)
						{
							if (pageOne)
							{
								app->scene->currentButton->data->Update(dt, 36, 42);
							}
							else
							{
								app->scene->currentButton->data->Update(dt, 43, 43 + items.Count() - 6);
							}
						}
						else
						{
							app->scene->currentButton->data->Update(dt, 36, 35 + items.Count());
						}

						if (app->input->CheckButton("cancel", KeyState::KEY_DOWN))
						{
							app->gui->ResetButtons();
							app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatAttack));
							changeMenu = true;
							combatMenuFlags = 0;
						}
					}
				}
			}
		}
	}

	return true;
}

bool SceneCombat::UpdateTransition(float dt, TransitionStatus transitionTo)
{
	if (transitionTo == TransitionStatus::BATTLE)
	{
		transition.Update(dt);
		if (transition.currentFrame >= 23)
		{
			waitForTransition = TransitionStatus::BATTLE;
		}
		if (transition.HasFinished())
		{
			waitForTransition = TransitionStatus::END;
		}
	}
	else if (transitionTo == TransitionStatus::SCENE)
	{
		if (waitForTransition != TransitionStatus::SCENE)
		{
			alpha -= dt;
		}
		else
		{
			alpha += dt;
		}
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
			waitForTransition = TransitionStatus::SCENE;
			if (strcmp(app->map->data.name, "tutorial.tmx") == 0) app->audio->PlayMusic("Audio/Music/Tutorial.ogg");
			else if (strcmp(app->map->data.name, "home.tmx") == 0) app->audio->PlayMusic("Audio/Music/Home.ogg");
		}
		else if (alpha >= 1.01f)
		{
			alpha = 1.0f;
			waitForTransition = TransitionStatus::NONE;
			backToGameplay = false;
		}
	}
	return true;
}

bool SceneCombat::Draw(Font* dialogueFont)
{
	if (waitForTransition == TransitionStatus::BATTLE || waitForTransition == TransitionStatus::END)
	{
		app->render->DrawTexture(combatGui, -app->render->camera.x, -app->render->camera.y, false, &combatTextBox);
		app->render->DrawTexture(combatGui, -app->render->camera.x, -app->render->camera.y + app->render->camera.h - combatTextBox.h, false, &combatTextBox);

		if (!characterSelected)
		{
			if (characterFlags >= 1)
			{
				app->render->DrawTexture(combatGui, -app->render->camera.x + mainChar.character->entityRect.x, -app->render->camera.y + mainChar.character->entityRect.y, false, &mainChar.box);
				app->render->DrawTexture(combatGui, -app->render->camera.x + mainChar.character->entityRect.x + 10, -app->render->camera.y + mainChar.character->entityRect.y + (mainChar.box.h / 2 - mainChar.characterTex.h / 2), false, &mainChar.characterTex);
				app->render->DrawText(dialogueFont, mainChar.hp, /*-app->render->camera.x +*/ mainChar.character->entityRect.x + mainChar.characterTex.w + 15,/* -app->render->camera.y +*/ mainChar.character->entityRect.y + 45, 28, 1, white);
				app->render->DrawText(dialogueFont, mainChar.mp, /*-app->render->camera.x +*/ mainChar.character->entityRect.x + mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ mainChar.character->entityRect.y + 45 + 30, 28, 1, white);
				app->render->DrawText(dialogueFont, mainChar.stress, /*-app->render->camera.x +*/ mainChar.character->entityRect.x + mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ mainChar.character->entityRect.y + 45 + 60, 28, 1, white);
			}
			if (characterFlags >= 3)
			{
				app->render->DrawTexture(combatGui, -app->render->camera.x + grandpa.character->entityRect.x, -app->render->camera.y + grandpa.character->entityRect.y, false, &grandpa.box);
				app->render->DrawTexture(combatGui, -app->render->camera.x + grandpa.character->entityRect.x + 10, -app->render->camera.y + grandpa.character->entityRect.y + (grandpa.box.h / 2 - grandpa.characterTex.h / 2), false, &grandpa.characterTex);
				app->render->DrawText(dialogueFont, grandpa.hp, /*-app->render->camera.x +*/ grandpa.character->entityRect.x + grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ grandpa.character->entityRect.y + 50, 28, 1, white);
				app->render->DrawText(dialogueFont, grandpa.mp, /*-app->render->camera.x +*/ grandpa.character->entityRect.x + grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ grandpa.character->entityRect.y + 50 + 30, 28, 1, white);
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
			if (currentChar != nullptr)
			{
				app->render->DrawTexture(combatGui, -app->render->camera.x + app->render->camera.w - currentChar->box.w - 34, -app->render->camera.y + app->render->camera.h - currentChar->box.h - 25, false, &currentChar->box);
				app->render->DrawTexture(combatGui, -app->render->camera.x + app->render->camera.w - currentChar->box.w - 24, -app->render->camera.y + app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4), false, &currentChar->characterTex);
				app->render->DrawText(dialogueFont, currentChar->hp, app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w, app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4), 28, 1, white);
				app->render->DrawText(dialogueFont, currentChar->mp, app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w, app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4) + 30, 28, 1, white);
				if (currentChar->character->id == EntityId::MC) app->render->DrawText(dialogueFont, currentChar->stress, app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w, app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4) + 60, 28, 1, white);
			}
		}
	}

	if (waitForTransition == TransitionStatus::END)
	{
		UpdateTurnArrow();
		app->render->DrawText(dialogueFont, firstLine, 44, 52, 48, 2, white);
		app->render->DrawText(dialogueFont, secondLine, 44, 52 + 48, 48, 2, white);
		app->render->DrawText(dialogueFont, thirdLine, 44, 52 + 96, 48, 2, white);
		if (wait && !blink)
		{
			app->render->DrawText(dialogueFont, "Press", combatTextBox.w - (arrowCombat.w / 2) - arrowCombat.w - 20, combatTextBox.h - arrowCombat.h - (arrowCombat.h / 2) - 46, 16, 2, white);
			app->render->DrawText(dialogueFont, "Select", combatTextBox.w - (arrowCombat.w / 2) - arrowCombat.w - 20, combatTextBox.h - arrowCombat.h - (arrowCombat.h / 2) - 30, 16, 2, white);
			app->render->DrawTexture(combatGui, -app->render->camera.x + combatTextBox.w - (arrowCombat.w / 2) - arrowCombat.w - 20, -app->render->camera.y + combatTextBox.h - arrowCombat.h - (arrowCombat.h / 2) - 10, false, &arrowCombat);
		}

		if (characterSelected)
		{
			btnCombatAttack->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatSkills->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatItems->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatFlee->Draw(-app->render->camera.x, -app->render->camera.y);

			if ((combatMenuFlags & 1 << Flags::SKILL) != 0)
			{
				btnCombatSkill1->Draw(-app->render->camera.x, -app->render->camera.y);
				btnCombatSkill2->Draw(-app->render->camera.x, -app->render->camera.y);
				btnCombatSkill3->Draw(-app->render->camera.x, -app->render->camera.y);
				btnCombatSkill4->Draw(-app->render->camera.x, -app->render->camera.y);
				//btnCombatSkill5->Draw(-app->render->camera.x, -app->render->camera.y);
				//btnCombatSkill6->Draw(-app->render->camera.x, -app->render->camera.y);
			}
			else if ((combatMenuFlags & 1 << Flags::ITEMS) != 0)
			{
				int i = 0;
				int maxItems = -1;
				if (pageOne)
				{
					i = 0;
					if (items.Count() > 6) maxItems = 6;
					else maxItems = items.Count();
				}
				else
				{
					i = 6;
					maxItems = items.Count();
				}
				for (i; i < maxItems; i++)
				{
					switch (i)
					{
					case 0:
						btnCombatItem1->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 1:
						btnCombatItem2->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 2:
						btnCombatItem3->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 3:
						btnCombatItem4->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 4:
						btnCombatItem5->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 5:
						btnCombatItem6->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 6:
						btnCombatItem7->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 7:
						btnCombatItem8->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 8:
						btnCombatItem9->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 9:
						btnCombatItem10->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 10:
						btnCombatItem11->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					case 11:
						btnCombatItem12->Draw(-app->render->camera.x, -app->render->camera.y);
						break;
					default:
						break;
					}
				}
				if (items.Count() >= 6)
				{
					btnLeftArrow->Draw(-app->render->camera.x, -app->render->camera.y);
					btnRightArrow->Draw(-app->render->camera.x, -app->render->camera.y);
				}
			}
		}

		if (targetAttack || targetItem) //add more as we go
		{
			if (!usingGamepad)
			{
				int x, y;
				app->input->GetMousePosition(x, y);
				ListItem<CombatEntity*>* e = turnOrder.start;
				while (e != nullptr)
				{
					if (e->data->collider->Intersects({ x, y, 1, 1 }))
					{
						SDL_Rect r = { 0,0,0,0 };
						if (IsCharacter(e->data))
						{
							if (e->data->id == EntityId::MC)
								r = { mainChar.character->entityRect.x + 10, mainChar.character->entityRect.y + (mainChar.box.h / 2 - mainChar.characterTex.h / 2), mainChar.characterTex.w + 3, mainChar.characterTex.h };
							else if (e->data->id == EntityId::VIOLENT)
								r = { grandpa.character->entityRect.x + 10, grandpa.character->entityRect.y + (grandpa.box.h / 2 - grandpa.characterTex.h / 2) - 4, grandpa.characterTex.w + 2, grandpa.characterTex.h };
						}
						else r = e->data->collider->rect;
						UpdateSelection(r);
					}
					e = e->next;
				}
			}
			else
			{
				if (currentTarget == nullptr)
				{
					currentTarget = enemiesList.start;
				}
				else
				{
					SDL_Rect r = { 0,0,0,0 };
					if (IsCharacter(currentTarget->data))
					{
						if (currentTarget->data->id == EntityId::MC)
							r = { mainChar.character->entityRect.x + 10, mainChar.character->entityRect.y + (mainChar.box.h / 2 - mainChar.characterTex.h / 2), mainChar.characterTex.w + 3, mainChar.characterTex.h };
						else if (currentTarget->data->id == EntityId::VIOLENT)
							r = { grandpa.character->entityRect.x + 10, grandpa.character->entityRect.y + (grandpa.box.h / 2 - grandpa.characterTex.h / 2) - 4, grandpa.characterTex.w + 2, grandpa.characterTex.h };
					}
					else r = currentTarget->data->collider->rect;
					UpdateSelection(r);
				}
			}
		}
	}

	if (waitForTransition != TransitionStatus::END)
	{
		app->render->DrawTexture(transitionTx, -app->render->camera.x, -app->render->camera.y, true, &transition.GetCurrentFrame());
	}

	return true;
}

bool SceneCombat::Finish()
{
	ListItem<CombatEntity*>* e = turnOrder.start;
	while (e != nullptr)
	{
		ListItem<CombatEntity*>* eNext = e->next;
		if (!IsCharacter(e->data))
		{
			e->data->pendingToDelete = true;
			app->entities->DestroyEntity(e->data);
		}
		e = eNext;
	}
	turnOrder.Clear();
	enemiesList.Clear();

	memset(firstLine, 0, TEXT_LEN);
	memset(secondLine, 0, TEXT_LEN);
	memset(thirdLine, 0, TEXT_LEN);

	transition.Reset();
	if (!heDed) backToGameplay = true;

	enemy1 = nullptr;
	enemy2 = nullptr;
	enemy3 = nullptr;
	currentChar = nullptr;
	target = nullptr;
	targetFrameTime = 0.0f;
	increaseSelect = true;
	turnFrameTime = 0.0f;
	increaseTurn = true;
	currentEntity = nullptr;
	currentTarget = nullptr;
	pastEntity = nullptr;
	characterSelected = false;
	targetAttack = false;
	finishedAction = false;
	hasTicked = false;
	memset(attackSelected, 0, TEXT_LEN);
	combatMenuFlags = 0;
	itemSelected = -1;
	pageOne = true;
	targetItem = false;
	once = true;
	scripted = false;
	app->scene->current->combatCooldown = 1.0f;
	app->scene->current->combat = false;
	if (app->scene->current->currentScene == SceneType::GAMEPLAY)
	{
		SceneGameplay* s = (SceneGameplay*)app->scene->current;
		app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(s->btnInventory));
	}
	else
	{
		app->scene->currentButton = nullptr;
	}

	return true;
}

bool SceneCombat::Unload()
{
	items.Clear();
	turnOrder.Clear();
	memset(attackSelected, 0, TEXT_LEN);

	memset(mainChar.hp, 0, TEXT_LEN);
	memset(mainChar.mp, 0, TEXT_LEN);
	memset(mainChar.stress, 0, TEXT_LEN);
	memset(mainChar.pAtk, 0, TEXT_LEN);
	memset(mainChar.mAtk, 0, TEXT_LEN);
	memset(mainChar.pDef, 0, TEXT_LEN);
	memset(mainChar.mDef, 0, TEXT_LEN);
	memset(mainChar.speed, 0, TEXT_LEN);

	memset(grandpa.hp, 0, TEXT_LEN);
	memset(grandpa.mp, 0, TEXT_LEN);
	memset(grandpa.stress, 0, TEXT_LEN);
	memset(grandpa.pAtk, 0, TEXT_LEN);
	memset(grandpa.mAtk, 0, TEXT_LEN);
	memset(grandpa.pDef, 0, TEXT_LEN);
	memset(grandpa.mDef, 0, TEXT_LEN);
	memset(grandpa.speed, 0, TEXT_LEN);

	memset(firstLine, 0, TEXT_LEN);
	memset(secondLine, 0, TEXT_LEN);
	memset(thirdLine, 0, TEXT_LEN);

	app->tex->UnLoad(combatGui);
	app->tex->UnLoad(transitionTx);
	
	app->audio->UnloadFx(smackFx);
	app->audio->UnloadFx(slapFx);
	app->audio->UnloadFx(confortFx);
	app->audio->UnloadFx(speechFx);

	app->audio->UnloadFx(smiteFx);
	app->audio->UnloadFx(magicBlowFx);
	app->audio->UnloadFx(magicBarrageFx);
	app->audio->UnloadFx(magicSlapFx);
	app->audio->UnloadFx(stareFx);

	app->audio->UnloadFx(strongerFx);
	app->audio->UnloadFx(bladesFx);
	app->audio->UnloadFx(stressFx);
	app->audio->UnloadFx(badDreamFx);
	app->audio->UnloadFx(closeEyesFx);
	app->audio->UnloadFx(deadFx);
	app->audio->UnloadFx(graspFx);

	return true;
}

bool SceneCombat::SortSpeed(bool isSorted)
{
	int count = 0;
	ListItem<CombatEntity*>* e = turnOrder.start;
	while (e != nullptr && e->next != nullptr)
	{
		if (e->data->stats.speed > e->next->data->stats.speed)
		{
			SWAP<CombatEntity*>(e->data, e->next->data);
			count++;
		}
		e = e->next;
	}

	if (count != 0) SortSpeed(false);
	return true;
}

bool SceneCombat::IsCharacter(CombatEntity* e)
{
	if (e->id != EntityId::MC && e->id != EntityId::KIND && e->id != EntityId::STUBBORN && e->id != EntityId::VIOLENT)
		return false;

	return true;
}

void SceneCombat::SelectTarget()
{
	if (usingGamepad)
	{
		// Select target with gamepad :)
		if (app->input->CheckButton("right", KeyState::KEY_DOWN))
		{
			if (currentTarget->next != nullptr)
				currentTarget = currentTarget->next;
			else
				currentTarget = enemiesList.start;
		}
		if (app->input->CheckButton("left", KeyState::KEY_DOWN))
		{
			if (currentTarget->prev != nullptr)
				currentTarget = currentTarget->prev;
			else
				currentTarget = enemiesList.end;
		}
		if (app->input->CheckButton("select", KeyState::KEY_DOWN))
		{
			target = currentTarget->data;
			currentTarget = nullptr;
		}
	}
	else
	{
		int x, y;
		app->input->GetMousePosition(x, y);
		ListItem<CombatEntity*>* e = turnOrder.start;
		while (e != nullptr)
		{
			if (e->data->collider->Intersects({ x, y, 1, 1 }) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
				target = e->data;

			e = e->next;
		}
	}
}

void SceneCombat::UpdateSelection(const SDL_Rect r)
{
	app->render->DrawRectangle({ r.x - (int)targetFrameTime - app->render->camera.x,r.y - (int)targetFrameTime - app->render->camera.y,r.w + (int)targetFrameTime * 2,r.h + (int)targetFrameTime * 2 }, 255, 255, 255, 150, false);
	app->render->DrawRectangle({ r.x - (int)(targetFrameTime + 1) - app->render->camera.x,r.y - (int)(targetFrameTime + 1) - app->render->camera.y,r.w + (int)(targetFrameTime + 1) * 2,r.h + (int)(targetFrameTime + 1) * 2 }, 255, 255, 255, 150, false);
	app->render->DrawRectangle({ r.x - (int)(targetFrameTime + 2) - app->render->camera.x,r.y - (int)(targetFrameTime + 2) - app->render->camera.y,r.w + (int)(targetFrameTime + 2) * 2,r.h + (int)(targetFrameTime + 2) * 2 }, 255, 255, 255, 250, false);
	app->render->DrawRectangle({ r.x - (int)(targetFrameTime + 3) - app->render->camera.x,r.y - (int)(targetFrameTime + 3) - app->render->camera.y,r.w + (int)(targetFrameTime + 3) * 2,r.h + (int)(targetFrameTime + 3) * 2 }, 255, 255, 255, 250, false);
	app->render->DrawRectangle({ r.x - (int)(targetFrameTime + 4) - app->render->camera.x,r.y - (int)(targetFrameTime + 4) - app->render->camera.y,r.w + (int)(targetFrameTime + 4) * 2,r.h + (int)(targetFrameTime + 4) * 2 }, 255, 255, 255, 150, false);
	app->render->DrawRectangle({ r.x - (int)(targetFrameTime + 7) - app->render->camera.x,r.y - (int)(targetFrameTime + 7) - app->render->camera.y,r.w + (int)(targetFrameTime + 7) * 2,r.h + (int)(targetFrameTime + 7) * 2 }, 255, 255, 255, 250, false);

	if (increaseSelect) targetFrameTime += 0.15f;
	else targetFrameTime -= 0.15f;

	if (targetFrameTime >= 6.0f) increaseSelect = false;
	else if (targetFrameTime <= 0.0f) increaseSelect = true;
}

void SceneCombat::UpdateTurnArrow()
{
	SDL_Rect sec = { 1332,192,61,54 };
	CombatEntity* e = nullptr;
	if (currentEntity != nullptr)
	{
		if (pastEntity != currentEntity->data && !wait) pastEntity = currentEntity->data;
		e = pastEntity;
	}
	if (e == nullptr && turnOrder.start != nullptr) e = turnOrder.end->data;

	if (e != nullptr)
	{
		iPoint pos;
		if (IsCharacter(e))
		{
			SDL_Rect box = { 0,0,0,0 };
			if (e->id == EntityId::MC) box = mainChar.box;
			else if (e->id == EntityId::VIOLENT) box = grandpa.box;

			pos = { e->entityRect.x + (box.w - sec.w) / 2, e->entityRect.y - sec.h };
		}
		else pos = { e->entityRect.x + (e->entityRect.w - sec.w) / 2, e->entityRect.y - sec.h };

		if (!IsCharacter(e)) app->render->DrawTexture(combatGui, pos.x - app->render->camera.x, pos.y - (int)turnFrameTime - app->render->camera.y, false, &sec);
		else if (!characterSelected) app->render->DrawTexture(combatGui, pos.x - app->render->camera.x, pos.y - (int)turnFrameTime - app->render->camera.y, false, &sec);
	}

	if (increaseTurn) turnFrameTime += 0.1f;
	else turnFrameTime -= 0.1f;

	if (turnFrameTime >= 7.0f) increaseTurn = false;
	else if (turnFrameTime <= 0.0f) increaseTurn = true;
}

void SceneCombat::TickDownBuffs()
{
	if (currentEntity != nullptr)
	{
		ListItem<Attack*>* a = currentEntity->data->attackPool.start;
		while (a != nullptr)
		{
			ListItem<Attack*>* aNext = a->next;
			if (a->data->turns > 0)
			{
				a->data->turns--;
				if (a->data->turns == 0 || combatState == CombatStateType::COMBAT_END)
				{
					if (strcmp(a->data->attackName, "10 debuff defenses") == 0)
					{
						currentEntity->data->stats.pDef += (a->data->beforeBuff1 / 10) * 100;
						currentEntity->data->stats.mDef += (a->data->beforeBuff2 / 10) * 100;
					}
					else if (strcmp(a->data->attackName, "5 buff defenses") == 0)
					{
						currentEntity->data->stats.pDef -= (a->data->beforeBuff1 * 5) / 100;
						currentEntity->data->stats.mDef -= (a->data->beforeBuff2 * 5) / 100;
					}
					else if (strcmp(a->data->attackName, "10 buff defenses") == 0)
					{
						currentEntity->data->stats.pDef -= (a->data->beforeBuff1 / 10) * 100;
						currentEntity->data->stats.mDef -= (a->data->beforeBuff2 / 10) * 100;
					}
					else if (strcmp(a->data->attackName, "25 buff") == 0)
					{
						currentEntity->data->stats.pAtk -= (a->data->beforeBuff1 * 25) / 100;
						currentEntity->data->stats.mAtk -= (a->data->beforeBuff2 * 25) / 100;
					}
					else if (strcmp(a->data->attackName, "5 buff") == 0)
					{
						currentEntity->data->stats.pAtk -= (a->data->beforeBuff1 * 5) / 100;
						currentEntity->data->stats.mAtk -= (a->data->beforeBuff2 * 5) / 100;
					}
					currentEntity->data->attackPool.Del(a);
					if (a != nullptr) a = nullptr;
				}
			}
			if (aNext != nullptr) a = aNext;
			else break;
		}
	}
}

void SceneCombat::Damage(CombatEntity* target, int damage, bool isMagic)
{
	ListItem<Entity*>* e = app->entities->entities.start;
	Player* player = nullptr;
	while (e != nullptr)
	{
		if (e->data->type == EntityType::PLAYER) break;
		e = e->next;
	}
	if (e != nullptr)
	{
		player = (Player*)e->data;
	}

	if (player != nullptr && !player->godMode)
	{
		LOG("%s does damage attack to %s!", currentEntity->data->name, target->name);
		char tmp[75];
		sprintf_s(tmp, TEXT_LEN, "%s does damage attack to %s!", currentEntity->data->name, target->name);
		NextLine(tmp);

		int attack = 0;
		currentEntity->data->CalculatePrecision(damage);
		if (!isMagic) attack = (damage - target->stats.pDef);
		else attack = (damage - target->stats.mDef);
		LOG("%d = %d - %d", attack, damage, target->stats.mDef);

		if (attack <= 0) attack = 0;
		target->stats.hPoints -= attack;
		if (target->stats.hPoints <= 0) target->stats.hPoints = 0;
	}
	else
	{
		if (!IsCharacter(target))
		{
			LOG("%s does damage attack to %s!", currentEntity->data->name, target->name);
			char tmp[75];
			sprintf_s(tmp, TEXT_LEN, "%s does damage attack to %s!", currentEntity->data->name, target->name);
			NextLine(tmp);

			int attack = 0;
			currentEntity->data->CalculatePrecision(damage);
			if (!isMagic) attack = (damage - target->stats.pDef);
			else attack = (damage - target->stats.mDef);
			LOG("%d = %d - %d", attack, damage, target->stats.mDef);

			if (attack <= 0) attack = 0;
			target->stats.hPoints -= attack;
			if (target->stats.hPoints <= 0) target->stats.hPoints = 0;
		}
		else
		{
			LOG("You're god, no damage taken!");
			NextLine("God Mode active. No damage recieved.");
		}
	}
	sprintf_s(mainChar.hp, TEXT_LEN, "HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
	sprintf_s(grandpa.hp, TEXT_LEN, "HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
	//Should add the rest of the characters
}

void SceneCombat::Heal(CombatEntity* target, float p)
{
	ListItem<Entity*>* e = app->entities->entities.start;
	Player* player = nullptr;
	while (e != nullptr)
	{
		if (e->data->type == EntityType::PLAYER) break;
		e = e->next;
	}
	if (e != nullptr)
	{
		player = (Player*)e->data;
	}

	if (player != nullptr)
	{
		LOG("%s heals to %s!", currentEntity->data->name, target->name);
		char tmp[75];
		sprintf_s(tmp, TEXT_LEN, "%s heals %s!", currentEntity->data->name, target->name);
		NextLine(tmp);

		int heal = p * target->stats.hPointsMax;
		LOG("healed %d", heal);

		if (heal <= 0) heal = 0;
		target->stats.hPoints += heal;
		if (target->stats.hPoints >= target->stats.hPointsMax) target->stats.hPoints = target->stats.hPointsMax;
	}
	sprintf_s(mainChar.hp, TEXT_LEN, "HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
	sprintf_s(grandpa.hp, TEXT_LEN, "HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
	//Should add the rest of the characters
}

void SceneCombat::Stress(int value)
{
	ListItem<Entity*>* e = app->entities->entities.start;
	Player* player = nullptr;
	while (e != nullptr)
	{
		if (e->data->type == EntityType::PLAYER) break;
		e = e->next;
	}
	if (e != nullptr)
	{
		player = (Player*)e->data;
	}

	if (player != nullptr)
	{
		LOG("%s does a stressing attack to %s!", currentEntity->data->name, mainChar.character->name);
		char tmp[75];
		sprintf_s(tmp, TEXT_LEN, "%s does a stressing attack to %s!", currentEntity->data->name, mainChar.character->name);
		NextLine(tmp);

		if (value <= 0) value = 0;
		mainChar.character->stats.stress += value;
		//if (mainChar.character->stats.stress >= mainChar.character->stats.stressMax) mainChar.character->stats.stress -= mainChar.character->stats.stressMax;
		// already done in stress power
	}
	sprintf_s(mainChar.stress, TEXT_LEN, "ST: %d/%d", mainChar.character->stats.stress, mainChar.character->stats.stressMax);
}

void SceneCombat::ManaCost(int value)
{
	if ((currentChar->character->stats.mPoints - value) <= 0)
	{
		LOG("%s can't use this skill, not enough MP!", currentChar->character->name);
		char tmp[75];
		sprintf_s(tmp, TEXT_LEN, "%s can't use this skill, not enough MP!", currentChar->character->name);
		NextLine(tmp);
	}
	else
	{
		currentChar->character->stats.mPoints -= value;
	}
	sprintf_s(mainChar.mp, TEXT_LEN, "MP: %d/%d", mainChar.character->stats.mPoints, mainChar.character->stats.mPointsMax);
	sprintf_s(grandpa.mp, TEXT_LEN, "MP: %d/%d", grandpa.character->stats.mPoints, grandpa.character->stats.mPointsMax);
	//Should add the rest of the characters
}

void SceneCombat::VictoryCondition()
{
	ListItem<CombatEntity*>* e = turnOrder.start;
	int counter = 0;
	while (e != nullptr)
	{
		if (!IsCharacter(e->data)) counter++;
		e = e->next;
	}

	if (counter == 0)
	{
		combatState = CombatStateType::COMBAT_END;
		LOG("Victory!\n");
		NextLine("Victory!");
		wait = true;
	}
}

void SceneCombat::DefeatCondition()
{
	ListItem<CombatEntity*>* e = turnOrder.start;
	int counter = 0;
	while (e != nullptr)
	{
		if (IsCharacter(e->data)) counter++;
		e = e->next;
	}

	if (counter == 0)
	{
		heDed = true;
		combatState = CombatStateType::COMBAT_END;
		LOG("Defeat!\n");
		NextLine("Defeat!");
		wait = true;
	}
}

int SceneCombat::EnemyTarget()
{
	int c = 0;
	if (mainChar.character->stats.hPoints > 0) c++;
	if (grandpa.character->stats.hPoints > 0) c++;
	//Should add the rest of the characters
	return  rand() % c + 1;
}

void SceneCombat::StressPower()
{
	if (mainChar.character->stats.stress >= mainChar.character->stats.stressMax)
	{
		mainChar.character->stats.stress -= mainChar.character->stats.stressMax;
		mainChar.character->isStunned = 2;
	}
	StressStatus pastStressStatus = stressStatus;
	if (pastStress != mainChar.character->stats.stress)
	{
		if (mainChar.character->stats.stress < 50)
		{
			stressStatus = StressStatus::UNDER;
		}
		else if (mainChar.character->stats.stress == 50)
		{
			stressStatus = StressStatus::BALANCE;
		}
		else
		{
			stressStatus = StressStatus::OVER;
		}
	}
	pastStress = mainChar.character->stats.stress;

	if (pastStressStatus != stressStatus)
	{
		switch (stressStatus)
		{
		case StressStatus::UNDER:
			mainChar.character->stats.pDef += 15;
			mainChar.character->stats.mDef += 15;
			mainChar.character->stats.pAtk -= 10;
			mainChar.character->stats.mAtk -= 10;
			if (pastStressStatus == StressStatus::BALANCE)
			{
				mainChar.character->stats.pDef -= 10;
				mainChar.character->stats.mDef -= 10;
				mainChar.character->stats.pAtk -= 10;
				mainChar.character->stats.mAtk -= 10;
			}
			else if(pastStressStatus == StressStatus::OVER)
			{
				mainChar.character->stats.pDef += 10;
				mainChar.character->stats.mDef += 10;
				mainChar.character->stats.pAtk -= 15;
				mainChar.character->stats.mAtk -= 15;
			}
			break;
		case StressStatus::BALANCE:
			mainChar.character->stats.pDef += 10;
			mainChar.character->stats.mDef += 10;
			mainChar.character->stats.pAtk += 10;
			mainChar.character->stats.mAtk += 10;
			if (pastStressStatus == StressStatus::UNDER)
			{
				mainChar.character->stats.pDef -= 15;
				mainChar.character->stats.mDef -= 15;
				mainChar.character->stats.pAtk += 10;
				mainChar.character->stats.mAtk += 10;
			}
			else if (pastStressStatus == StressStatus::OVER)
			{
				mainChar.character->stats.pDef += 10;
				mainChar.character->stats.mDef += 10;
				mainChar.character->stats.pAtk -= 15;
				mainChar.character->stats.mAtk -= 15;
			}
			break;
		case StressStatus::OVER:
			mainChar.character->stats.pDef -= 10;
			mainChar.character->stats.mDef -= 10;
			mainChar.character->stats.pAtk += 15;
			mainChar.character->stats.mAtk += 15;
			if (pastStressStatus == StressStatus::UNDER)
			{
				mainChar.character->stats.pDef -= 15;
				mainChar.character->stats.mDef -= 15;
				mainChar.character->stats.pAtk += 10;
				mainChar.character->stats.mAtk += 10;
			}
			else if (pastStressStatus == StressStatus::BALANCE)
			{
				mainChar.character->stats.pDef -= 10;
				mainChar.character->stats.mDef -= 10;
				mainChar.character->stats.pAtk -= 10;
				mainChar.character->stats.mAtk -= 10;
			}
			break;
		}
	}
}

SDL_Rect SceneCombat::SkillSec(int i, EntityId id)
{
	switch (id)
	{
	case EntityId::MC:
		switch (i)
		{
		case 1:
			return { 96,416,32,32 };
			break;
		case 2:
			return { 32,160,32,32 };
			break;
		case 3:
			return { 32,96,32,32 };
			break;
		case 4:
			return { 128,416,32,32 };
			break;
		default:
            return { 0,0,32,32 };
			break;
		}
		break;
	case EntityId::VIOLENT:
		switch (i)
		{
		case 1:
			return { 0,192,32,32 };
			break;
		case 2:
			return { 0,192,32,32 };
			break;
		case 3:
			return { 0,192,32,32 };
			break;
		case 4:
			return { 0,192,32,32 };
			break;
		default:
            return { 0,192,32,32 };
			break;
		}
		break;
	default:
        return { 0,0,32,32 };
		break;
	}
}

void SceneCombat::SpawnEnemies(EntityId id1, EntityId id2, EntityId id3)
{
	if (id1 != EntityId::NOT_COMBAT || id2 != EntityId::NOT_COMBAT || id3 != EntityId::NOT_COMBAT)
	{
		int c = 0;
		if (id1 != EntityId::NOT_COMBAT) c++;
		if (id2 != EntityId::NOT_COMBAT) c++;
		if (id3 != EntityId::NOT_COMBAT) c++;

		for (int i = 0; i < c; i++)
		{
			iPoint pos;
			pos.y = 300;
			switch (c)
			{
			case 1:
				pos.x = 576;
				switch (id1)
				{
				case EntityId::STRESSING_SHADOW:
					enemy1 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::STRESSING_SHADOW, Stats(0, 10, 10, 10, 40, 40, 60));
					break;
				case EntityId::FURIOUS_SHADOW:
					enemy1 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::FURIOUS_SHADOW, Stats(15, 0, 20, 5, 60, 60, 90));
					break;
				case EntityId::NIGHTMARE:
					enemy1 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::NIGHTMARE, Stats(10, 0, 20, 10, 70, 70, 70));
					break;
				default:
					break;
				}
				break;
			case 2:
				if (i == 0)
				{
					pos.x = 341;
					switch (id1)
					{
					case EntityId::STRESSING_SHADOW:
						enemy1 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::STRESSING_SHADOW, Stats(0, 10, 10, 10, 40, 40, 60));
						break;
					case EntityId::FURIOUS_SHADOW:
						enemy1 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::FURIOUS_SHADOW, Stats(15, 0, 20, 5, 60, 60, 90));
						break;
					case EntityId::NIGHTMARE:
						enemy1 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::NIGHTMARE, Stats(10, 0, 20, 10, 70, 70, 70));
						break;
					default:
						break;
					}
				}
				else
				{
					pos.x = 810;
					switch (id2)
					{
					case EntityId::STRESSING_SHADOW:
						enemy2 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::STRESSING_SHADOW, Stats(0, 10, 10, 10, 40, 40, 60));
						break;
					case EntityId::FURIOUS_SHADOW:
						enemy2 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::FURIOUS_SHADOW, Stats(15, 0, 20, 5, 60, 60, 90));
						break;
					case EntityId::NIGHTMARE:
						enemy2 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::NIGHTMARE, Stats(10, 0, 20, 10, 70, 70, 70));
						break;
					default:
						break;
					}
				}
				break;
			case 3:
				if (i == 0)
				{
					pos.x = 224;
					switch (id1)
					{
					case EntityId::STRESSING_SHADOW:
						enemy1 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::STRESSING_SHADOW, Stats(0, 10, 10, 10, 40, 40, 60));
						break;
					case EntityId::FURIOUS_SHADOW:
						enemy1 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::FURIOUS_SHADOW, Stats(15, 0, 20, 5, 60, 60, 90));
						break;
					case EntityId::NIGHTMARE:
						enemy1 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::NIGHTMARE, Stats(10, 0, 20, 10, 70, 70, 70));
						break;
					default:
						break;
					}
				}
				else if (i == 1)
				{
					pos.x = 576;
					switch (id2)
					{
					case EntityId::STRESSING_SHADOW:
						enemy2 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::STRESSING_SHADOW, Stats(0, 10, 10, 10, 40, 40, 60));
						break;
					case EntityId::FURIOUS_SHADOW:
						enemy2 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::FURIOUS_SHADOW, Stats(15, 0, 20, 5, 60, 60, 90));
						break;
					case EntityId::NIGHTMARE:
						enemy2 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::NIGHTMARE, Stats(10, 0, 20, 10, 70, 70, 70));
						break;
					default:
						break;
					}
				}
				else
				{
					pos.x = 928;
					switch (id3)
					{
					case EntityId::STRESSING_SHADOW:
						enemy3 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::STRESSING_SHADOW, Stats(0, 10, 10, 10, 40, 40, 60));
						break;
					case EntityId::FURIOUS_SHADOW:
						enemy3 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::FURIOUS_SHADOW, Stats(15, 0, 20, 5, 60, 60, 90));
						break;
					case EntityId::NIGHTMARE:
						enemy3 = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::NIGHTMARE, Stats(10, 0, 20, 10, 70, 70, 70));
						break;
					default:
						break;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	else
	{
		int e = rand() % 3 + 1;
		for (int i = 0; i < e; i++)
		{
			iPoint pos;
			Entity* currentEnemy;
			pos.y = 300;
			switch (e)
			{
			case 1:
				pos.x = 576;
				currentEnemy = enemy1;
				break;
			case 2:
				if (i == 0)
				{
					pos.x = 341;
					currentEnemy = enemy1;
				}
				else
				{
					pos.x = 810;
					currentEnemy = enemy2;
				}
				break;
			case 3:
				if (i == 0)
				{
					pos.x = 224;
					currentEnemy = enemy1;
				}
				else if (i == 1)
				{
					pos.x = 576;
					currentEnemy = enemy2;
				}
				else
				{
					pos.x = 928;
					currentEnemy = enemy3;
				}
				break;
			default:
				break;
			}

			int type = rand() % 2 + 1;
			if (type == 1) { currentEnemy = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::STRESSING_SHADOW, Stats(0, 10, 10, 10, 40, 40, 60)); }
			else if (type == 2) { currentEnemy = app->entities->CreateEntity(pos.x, pos.y, EntityType::COMBAT_ENTITY, EntityId::FURIOUS_SHADOW, Stats(15, 0, 20, 5, 60, 60, 90)); }
		}
	}

	if (id1 == EntityId::NIGHTMARE || id2 == EntityId::NIGHTMARE || id3 == EntityId::NIGHTMARE)
	{
		NextLine("A nightmare emerges...");
		app->render->background = { 0,0,0,255 };
		app->audio->PlayMusic("Audio/Music/Nightmare.ogg");
		scripted = true;
	}
	else
	{
		NextLine("Some monsters want to fight!");
		app->render->background = { 125,33,129,255 };
	}
}

void SceneCombat::NextLine(const char* line)
{
	memset(firstLine, 0, TEXT_LEN);
	strcpy_s(firstLine, TEXT_LEN, secondLine);
	memset(secondLine, 0, TEXT_LEN);
	strcpy_s(secondLine, TEXT_LEN, thirdLine);
	memset(thirdLine, 0, TEXT_LEN);
	strcpy_s(thirdLine, TEXT_LEN, line);
}


//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneCombat::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 28: //ATTACK
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			strcpy_s(attackSelected, TEXT_LEN, "Smack");
			break;
		case EntityId::VIOLENT:
			strcpy_s(attackSelected, TEXT_LEN, "Smite foes");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 29: //SKILLS
		app->gui->ResetButtons();
		btnCombatSkills->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::SKILL);
		changeMenu = true;
		break;
	case 30: //ITEMS
		app->gui->ResetButtons();
		btnCombatItems->state = GuiControlState::DISABLED;
		btnLeftArrow->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::ITEMS);
		changeMenu = true;
		pageOne = true;
		break;
	case 31: //FLEE
	{
		app->gui->ResetButtons();
		if (scripted)
		{
			LOG("You can't flee!");
			NextLine("You can't flee!");
			break;
		}
		else
		{
			combatMenuFlags = 0;
			characterSelected = false;
			LOG("You fled!");
			NextLine("You fled!");
			Finish();
			once = true;
		}
		break;
	}
	case 32: //SKILL 1
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			strcpy_s(attackSelected, TEXT_LEN, "Comfort");
			break;
		case EntityId::VIOLENT:
			strcpy_s(attackSelected, TEXT_LEN, "W. Magic missile");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 33: //SKILL 2
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			strcpy_s(attackSelected, TEXT_LEN, "Slap");
			break;
		case EntityId::VIOLENT:
			strcpy_s(attackSelected, TEXT_LEN, "W. Magic Barrage");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 34: //SKILL 3
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			strcpy_s(attackSelected, TEXT_LEN, "Encourage");
			break;
		case EntityId::VIOLENT:
			strcpy_s(attackSelected, TEXT_LEN, "Magic Hand Slap");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 35: //SKILL 4
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			strcpy_s(attackSelected, TEXT_LEN, "Boring Speech");
			break;
		case EntityId::VIOLENT:
			strcpy_s(attackSelected, TEXT_LEN, "Judgemental Stare");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	//case 56: //SKILL 5
	//	app->gui->ResetButtons();
	//	combatMenuFlags = 0;
	//	characterSelected = false;
	//	switch (currentChar->character->id)
	//	{
	//	case EntityId::MC:
	//		attackSelected.Create("MC Skill 5");
	//		break;
	//	case EntityId::VIOLENT:
	//		attackSelected.Create("Grandpa Skill 5");
	//		break;
	//	default:
	//		break;
	//	}
	//	LOG("Who do you want to attack?");
	//	NextLine("Who do you want to attack?");
	//	break;
	//case 57: //SKILL 6
	//	app->gui->ResetButtons();
	//	combatMenuFlags = 0;
	//	characterSelected = false;
	//	switch (currentChar->character->id)
	//	{
	//	case EntityId::MC:
	//		attackSelected.Create("MC Skill 6");
	//		break;
	//	case EntityId::VIOLENT:
	//		attackSelected.Create("Grandpa Skill 6");
	//		break;
	//	default:
	//		break;
	//	}
	//	LOG("Who do you want to attack?");
	//	NextLine("Who do you want to attack?");
	//	break;
	case 36: //ITEM 1
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 1;
		targetItem = true;
		LOG("USED %s", btnCombatItem1->text);
		break;
	case 37: //ITEM 2
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 2;
		targetItem = true;
		LOG("USED %s", btnCombatItem2->text);
		break;
	case 38: //ITEM 3
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 3;
		targetItem = true;
		LOG("USED %s", btnCombatItem3->text);
		break;
	case 39: //ITEM 4
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 4;
		targetItem = true;
		LOG("USED %s", btnCombatItem4->text);
		break;
	case 40: //ITEM 5
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 5;
		targetItem = true;
		LOG("USED %s", btnCombatItem5->text);
		break;
	case 41: //ITEM 6
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 6;
		targetItem = true;
		LOG("USED %s", btnCombatItem6->text);
		break;
	case 42: //RIGHT ARROW
		pageOne = false;
		btnLeftArrow->state = GuiControlState::NORMAL;
		btnRightArrow->state = GuiControlState::DISABLED;
		app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatItem7));
		break;
	case 43: //LEFT ARROW
		pageOne = true;
		btnLeftArrow->state = GuiControlState::DISABLED;
		btnRightArrow->state = GuiControlState::NORMAL;
		app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatItem1));
		break;
	case 44: //ITEM 7
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 7;
		targetItem = true;
		LOG("USED %s", btnCombatItem7->text);
		break;
	case 45: //ITEM 8
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 8;
		targetItem = true;
		LOG("USED %s", btnCombatItem8->text);
		break;
	case 46: //ITEM 9
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 9;
		targetItem = true;
		LOG("USED %s", btnCombatItem9->text);
		break;
	case 47: //ITEM 10
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 10;
		targetItem = true;
		LOG("USED %s", btnCombatItem10->text);
		break;
	case 48: //ITEM 11
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 11;
		targetItem = true;
		LOG("USED %s", btnCombatItem11->text);
		break;
	case 49: //ITEM 12
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 12;
		targetItem = true;
		LOG("USED %s", btnCombatItem12->text);
		break;
	default:
		break;
	}

	return true;
}