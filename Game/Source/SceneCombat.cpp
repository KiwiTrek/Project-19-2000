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

SceneCombat::SceneCombat()
{
	flags = 0;
	white = { 255,255,255,255 };

	for (int i = 0; i != 27; ++i)
	{
		transition.PushBack({ 600 * i, 0, 600, 338 });
	}
	transition.loop = false;
	transition.speed = 10.0f;

	characterFlags = 0;
	characterFlags = SetBit(characterFlags, (uint)EntityId::MC);
	characterFlags = SetBit(characterFlags, (uint)EntityId::VIOLENT);
}

SceneCombat::~SceneCombat()
{}

bool SceneCombat::Load()
{
	// COMBAT
	srand(time(NULL));
	combatGui = app->tex->Load("Textures/GUI/combatGui.png");
	transitionTx = app->tex->Load("Textures/BattleTransition.png");
	waitForTransition = TransitionStatus::NONE;
	scripted = false;
    backToGameplay = false;

	currentChar = nullptr;
	target = nullptr;
	currentEntity = nullptr;
	mainChar.box = { 1280,0,204,190 };
	mainChar.characterTex = { 0,252,72,92 };
	mainChar.character = (CombatEntity*)app->entities->CreateEntity(36, app->render->camera.h - mainChar.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::MC, Stats(15, 0, 10, 10, 100, 100, 50, 100, 100, 0, 100));
	mainChar.hp.Create("HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
	mainChar.mp.Create("MP: %d/%d", mainChar.character->stats.mPoints, mainChar.character->stats.mPointsMax);
	mainChar.stress.Create("ST: %d/%d", mainChar.character->stats.stress, mainChar.character->stats.stressMax);
	
	mainChar.lvl.Create("LVL: %d", 1);
	mainChar.xp.Create("XP: %d", 0);
	mainChar.nextLvl.Create("Next Level: %d", 15);
	mainChar.pAtk.Create("Physical Attack: %d", mainChar.character->stats.pAtk);
	mainChar.mAtk.Create("Magic Attack: %d", mainChar.character->stats.mAtk);
	mainChar.pDef.Create("Physical Defense: %d", mainChar.character->stats.pDef);
	mainChar.mDef.Create("Magic Defense: %d", mainChar.character->stats.mDef);
	mainChar.speed.Create("Speed: %d", mainChar.character->stats.speed);

	grandpa.box = { 1280,0,204,190 };
	grandpa.characterTex = { 73,252,68,100 };
	grandpa.character = (CombatEntity*)app->entities->CreateEntity(grandpa.box.w + 36, app->render->camera.h - grandpa.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::VIOLENT, Stats(25, 25, 0, 0, 100, 100, 25, 100, 100));
	grandpa.hp.Create("HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
	grandpa.mp.Create("MP: %d/%d", grandpa.character->stats.mPoints, grandpa.character->stats.mPointsMax);

	grandpa.lvl.Create("LVL: %d", 1);
	grandpa.xp.Create("XP: %d", 0);
	grandpa.nextLvl.Create("Next Level: %d", 15);
	grandpa.pAtk.Create("Physical Attack: %d", grandpa.character->stats.pAtk);
	grandpa.mAtk.Create("Magic Attack: %d", grandpa.character->stats.mAtk);
	grandpa.pDef.Create("Physical Defense: %d", grandpa.character->stats.pDef);
	grandpa.mDef.Create("Magic Defense: %d", grandpa.character->stats.mDef);
	grandpa.speed.Create("Speed: %d", grandpa.character->stats.speed);


	//enemy1 = app->entities->CreateEntity(300, 300, EntityType::COMBAT_ENTITY, EntityId::STRESSING_SHADOW, Stats(0, 10, 10, 10, 40, 40, 60));
	//enemy2 = app->entities->CreateEntity(700, 300, EntityType::COMBAT_ENTITY, EntityId::FURIOUS_SHADOW, Stats(15, 0, 20, 5, 60, 60, 90));

		//COMBAT
	combatTextBox = { 0,0,1280,248 };
	combatMenuBox = { 305,249,1001,130 };
	btnCombatAttack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 41, { 34,505,200,60 }, "ATTACK", 40, this);
	btnCombatSkills = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 42, { 234,505,200,60 }, "SKILLS", 40, this);
	btnCombatItems = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 43, { 434,505,200,60 }, "ITEMS", 40, this);
	btnCombatSpecial = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 44, { 634,505,200,60 }, "SPECIAL", 40, this);
	btnCombatSpecial->state = GuiControlState::DISABLED;
	btnCombatFlee = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 45, { 834,505,200,60 }, "FLEE", 40, this);

	btnCombatSkill1 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 46, { 36, app->render->camera.h - combatMenuBox.h - 25,200,60 }, "skill 1", 40, this);
	btnCombatSkill2 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 47, { 256, app->render->camera.h - combatMenuBox.h - 25,200,60 }, "skill 2", 40, this);
	btnCombatSkill3 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 48, { 476, app->render->camera.h - combatMenuBox.h - 25,200,60 }, "skill 3", 40, this);
	btnCombatSkill4 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 49, { 36, app->render->camera.h - combatMenuBox.h + 35,200,60 }, "skill 4", 40, this);
	btnCombatSkill5 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 50, { 256, app->render->camera.h - combatMenuBox.h + 35,200,60 }, "skill 5", 40, this);
	btnCombatSkill6 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 51, { 476, app->render->camera.h - combatMenuBox.h + 35,200,60 }, "skill 6", 40, this);

	btnCombatItem1 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 52, { 84,570,300,60 }, "item one", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem2 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 53, { 384,570,300,60 }, "item two", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem3 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 54, { 684,570,300,60 }, "item three", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem4 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 55, { 84,630,300,60 }, "item four", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem5 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 56, { 384,630,300,60 }, "item five", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem6 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 57, { 684,630,300,60 }, "item six", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem7 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 58, { 84,570,300,60 }, "item seven", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem8 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 59, { 384,570,300,60 }, "item eight", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem9 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 60, { 684,570,300,60 }, "item nine", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem10 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 61, { 84,630,300,60 }, "item ten", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem11 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 62, { 384,630,300,60 }, "item eleven", 40, this, 0, app->entities->itemAtlas);
	btnCombatItem12 = (GuiImageButton*)app->gui->CreateGuiControl(GuiControlType::IMAGEBUTTON, 63, { 684,630,300,60 }, "item twelve", 40, this, 0, app->entities->itemAtlas);
	btnLeftArrow = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 64, { 34,605,50,50 }, "", 40, this);
	btnRightArrow = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 65, { 984,605,50,50 }, "", 40, this);

	LOG("%d", characterFlags);

    smackFx = app->audio->LoadFx("Audio/Fx/Smack.wav");
    slapFx = app->audio->LoadFx("Audio/Fx/Slap.wav");
    confortFx = app->audio->LoadFx("Audio/Fx/Confort.wav");
    smiteFx = app->audio->LoadFx("Audio/Fx/Smite.wav");
    magicBlowFx = app->audio->LoadFx("Audio/Fx/MagicBlow.wav");
    strongerFx = app->audio->LoadFx("Audio/Fx/Stronger.wav");
    bladesFx = app->audio->LoadFx("Audio/Fx/Blades.wav");
    stressFx = app->audio->LoadFx("Audio/Fx/Stress.wav");
    badDreamFx = app->audio->LoadFx("Audio/Fx/BadDream.wav");
    closeEyesFx = app->audio->LoadFx("Audio/Fx/CloseEyes.wav");
    deadFx = app->audio->LoadFx("Audio/Fx/Dead.wav");
    graspFx = app->audio->LoadFx("Audio/Fx/Grasp.wav");

	heDed = false;
	pastStress = -1;
	stressStatus = StressStatus::UNDER;

	return false;
}

bool SceneCombat::Start(EntityId id1, EntityId id2, EntityId id3)
{
	currentChar = nullptr;
	target = nullptr;
	currentEntity = nullptr;
	characterSelected = false;
	targetAttack = false;
	finishedAction = false;
	hasTicked = false;
	attackSelected.Clear();
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

	firstLine.Clear();
	secondLine.Clear();
	thirdLine.Clear();
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

	app->audio->PlayMusic("Audio/Music/Battle.ogg");
	SpawnEnemies(id1, id2, id3);

	app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatAttack));
    usingGamepad = false;
	if (app->input->GetControllerName() != "unplugged") usingGamepad = true;
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
			if (app->input->CheckButton("select", KEY_DOWN))
			{
				wait = false;
			}
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
				attackSelected.Clear();
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
			case COMBAT_START:
			{
				//CHECK WHO IS FIRST
				ListItem<Entity*>* e = app->entities->entities.start;

				while (e != nullptr)
				{
					//ADDS THEM TO THE LIST
					if (e->data->type == EntityType::COMBAT_ENTITY)
					{
						CombatEntity* cEntity = (CombatEntity*)e->data;
						if (cEntity->stats.hPoints > 0)
						{
							turnOrder.Add(cEntity);
						}
					}
					e = e->next;
				}

				SortSpeed(false);
				combatState = CombatStateType::COMBAT_MIDGAME;
			}
			break;
			case COMBAT_MIDGAME:
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
							LOG("%s is dead!", e->data->name.GetString());
							char tmp[50];
							sprintf(tmp, "%s is dead!", e->data->name.GetString());
							NextLine(tmp);
							app->audio->PlayFx(deadFx);
							wait = true;
							currentChar = nullptr;
							characterSelected = false;

							combatState = CombatStateType::COMBAT_END;
							currentEntity = e;
							TickDownBuffs();
							combatState = CombatStateType::COMBAT_MIDGAME;

							if (!IsCharacter(e->data)) app->entities->DestroyEntity(e->data);
							turnOrder.Del(turnOrder.At(turnOrder.Find(e->data)));
							SortSpeed(false);
							if (currentEntity == nullptr)
							{
								currentEntity = eNext;
							}
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
						}

						if (IsCharacter(currentEntity->data))													// CHARACTER
						{
							if (once) //i think we should use the flags menu to turn this true only when there are no "main buttons" selected
							{
								once = false;
								characterSelected = true;						//should delete eventually
								changeMenu = true;

								if (currentEntity->data->id == EntityId::MC) NextLine("It's your turn!");
								else
								{
									char tmp[50];
									sprintf(tmp, "It's %s's turn!", currentEntity->data->name.GetString());
									NextLine(tmp);
								}
							}

							//player should decide what to do here based on the buttons (guiclickevent)
							//LOG("%s's turn!\n", currentEntity->data->name.GetString());

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
									sprintf(tmp, "%s used %s item.", currentEntity->data->name.GetString(), items.At(itemSelected - 1)->data->effect.attackName.GetString());
									NextLine(tmp);
									app->audio->PlayFx(app->entities->itemFx);
									if (items.At(itemSelected - 1)->data->count == 0)
									{
										items.Del(items.At(itemSelected - 1));
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
									sprintf(tmp, "%s is stunned and can't move!", currentEntity->data->name.GetString());
									NextLine(tmp);
								}
								else
								{
									if (currentEntity->data->isTaunted != 0)
									{
										if (currentEntity->data->isTaunted == 1) currentEntity->data->isTaunted = 0;
										target = currentEntity->data->tauntedBy;
										attackSelected.Clear();
										char tmp[75];
										sprintf(tmp, "%s is taunted by %s!", currentEntity->data->name.GetString(), target->name.GetString());
										NextLine(tmp);
									}

									if (target == nullptr)
									{
										if (attackSelected != "")
										{
											targetAttack = true;
											SelectTarget();
										}
									}
									else
									{
										if (attackSelected == "Smack")
										{
											LOG("%d", currentEntity->data->stats.pAtk);
											app->audio->PlayFx(smackFx);
											Damage(target, currentEntity->data->stats.pAtk, false);
										}
										else if (attackSelected == "Comfort")
										{
											int p = rand() % 10 + 11;
											Heal(target, p);
											app->audio->PlayFx(confortFx);
										}
										else if (attackSelected == "Slap")
										{
											app->audio->PlayFx(slapFx);
											Damage(target, currentEntity->data->stats.pAtk, false);
										}
										else if (attackSelected == "Encourage")
										{
											LOG("%s buffs %s!", currentEntity->data->name.GetString(), target->name.GetString());
											char tmp[75];
											sprintf(tmp, "%s buffs %s!", currentEntity->data->name.GetString(), target->name.GetString());
											NextLine(tmp);
											Attack* a = new Attack("25 buff");
											a->turns = 3;
											a->beforeBuff1 = target->stats.pAtk;
											a->beforeBuff2 = target->stats.mAtk;
											target->attackPool.Add(a);
											target->stats.pAtk += (target->stats.pAtk * 25) / 100;
											target->stats.mAtk += (target->stats.mAtk * 25) / 100;
											app->audio->PlayFx(confortFx);
										}
										else if (attackSelected == "Boring Speech")
										{
											LOG("%s debuffs %s!", currentEntity->data->name.GetString(), target->name.GetString());
											char tmp[75];
											sprintf(tmp, "%s debuffs %s!", currentEntity->data->name.GetString(), target->name.GetString());
											NextLine(tmp);
											Attack* a = new Attack("10 debuff");
											a->turns = 3;
											a->beforeBuff1 = target->stats.pAtk;
											a->beforeBuff2 = target->stats.mAtk;
											target->attackPool.Add(a);
											target->stats.pDef -= (target->stats.pDef * 10) / 100;
											target->stats.mDef -= (target->stats.mDef * 10) / 100;
										}
										finishedAction = true;
										wait = true;
										if (target != nullptr)
										{
											target = nullptr;
											targetAttack = false;
											attackSelected.Clear();
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
									sprintf(tmp, "%s is stunned and can't move!", currentEntity->data->name.GetString());
									NextLine(tmp);
								}
								else
								{
									if (currentEntity->data->isTaunted != 0)
									{
										if (currentEntity->data->isTaunted == 1) currentEntity->data->isTaunted = 0;
										target = currentEntity->data->tauntedBy;
										attackSelected.Clear();
										char tmp[75];
										sprintf(tmp, "%s is taunted by %s!", currentEntity->data->name.GetString(), target->name.GetString());
										NextLine(tmp);
									}

									if (attackSelected == "Weak Magic Barrage")
									{
										ListItem<CombatEntity*>* e = turnOrder.start;
										while (e != nullptr)
										{
											if (!IsCharacter(e->data)) Damage(target, currentEntity->data->stats.mAtk / 1.4f, true);
											e = e->next;
										}
										finishedAction = true;
										wait = true;
										if (target != nullptr)
										{
											target = nullptr;
											targetAttack = false;
											attackSelected.Clear();
										}
									}
									else if (target == nullptr)
									{
										if (attackSelected != "")
										{
											targetAttack = true;
											SelectTarget();
										}
									}
									else
									{
										if (attackSelected == "Smite foes")
										{
											LOG("%d", currentEntity->data->stats.mAtk);
											app->audio->PlayFx(smackFx);
											Damage(target, currentEntity->data->stats.mAtk, true);
										}
										else if (attackSelected == "Weak Magic missile")
										{
											app->audio->PlayFx(smiteFx);
											Damage(target, currentEntity->data->stats.mAtk / 1.3f, true);
											Damage(target, currentEntity->data->stats.mAtk / 1.3f, true);
											Damage(target, currentEntity->data->stats.mAtk / 1.3f, true);
										}
										else if (attackSelected == "Magic Hand Slap")
										{
											Damage(target, currentEntity->data->stats.mAtk / 1.1f, true);
											Damage(target, currentEntity->data->stats.mAtk / 1.1f, true);
										}
										else if (attackSelected == "Judgemental Stare")
										{
											Damage(target, currentEntity->data->stats.mAtk * 1.5f, true);
										}
										finishedAction = true;
										wait = true;
										if (target != nullptr)
										{
											target = nullptr;
											targetAttack = false;
											attackSelected.Clear();
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
									Stress(10);
									app->audio->PlayFx(stressFx);
								}
								else //Magical blow
								{
									int t = EnemyTarget();
									switch (t)
									{
									case 1: //MC
										Damage(mainChar.character, currentEntity->data->stats.mAtk, true);
										break;
									case 2: //GRANDPA
										Damage(grandpa.character, currentEntity->data->stats.mAtk, true);
										break;
									//case 3:
									//	break;
									//case 4:
									//	break;
									default:
										break;
									}
									app->audio->PlayFx(magicBlowFx);
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
									LOG("Furious Shadow got stronger!");
									NextLine("Furious Shadow got stronger!");
									Attack* a = new Attack("5 buff defenses");
									a->turns = 2;
									currentEntity->data->attackPool.Add(a);
									currentEntity->data->stats.pDef += (currentEntity->data->stats.pDef * 5) / 100;
									currentEntity->data->stats.mDef += (currentEntity->data->stats.mDef * 5) / 100;
									app->audio->PlayFx(strongerFx);
								}
								else //Fury of blades
								{
									Damage(mainChar.character, currentEntity->data->stats.pAtk, false);
									Damage(grandpa.character, currentEntity->data->stats.pAtk, false);
									LOG("Furious Shadow attack to all!\n");
									NextLine("Furious Shadow attack to all!");
									app->audio->PlayFx(bladesFx);
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
									int t = EnemyTarget();
									switch (t)
									{
									case 1: //MC
										Damage(mainChar.character, currentEntity->data->stats.pAtk, false);
										break;
									case 2: //GRANDPA
										Damage(grandpa.character, currentEntity->data->stats.pAtk, false);
										break;
									//case 3:
									//	break;
									//case 4:
									//	break;
									default:
										break;
									}
									app->audio->PlayFx(badDreamFx);
								}
								else if (p >= 30) //Nightmarish
								{
									LOG("Nightmarish got stronger!");
									NextLine("Nightmarish got stronger!");
									Attack* a = new Attack("10 buff defenses");
									a->turns = 2;
									currentEntity->data->attackPool.Add(a);
									currentEntity->data->stats.pDef += currentEntity->data->stats.pDef / 10;
									currentEntity->data->stats.mDef += currentEntity->data->stats.mDef / 10;
									app->audio->PlayFx(strongerFx);
								}
								else if (p >= 5) //Close your eyes
								{
									int t = EnemyTarget();
									switch (t)
									{
									case 1: //MC
										LOG("Nightmarish used Close Your Eyes on You!");
										NextLine("Nightmarish used Close Your Eyes on You!");
										mainChar.character->isTaunted = 1;
										mainChar.character->tauntedBy = currentEntity->data;
										break;
									case 2: //GRANDPA
										LOG("Nightmarish used Close Your Eyes on Grandpa!");
										NextLine("Nightmarish used Close Your Eyes on Grandpa!");
										grandpa.character->isTaunted = 1;
										grandpa.character->tauntedBy = currentEntity->data;
										break;
									//case 3:
									//	break;
									//case 4:
									//	break;
									default:
										break;
									}
									app->audio->PlayFx(closeEyesFx);
								}
								else //Grasp of depression
								{
									int t = EnemyTarget();
									switch (t)
									{
									case 1: //MC
									{
										LOG("Nightmarish used Grasp of Depression on You!");
										NextLine("Nightmarish used Grasp of Depression on You!");
										mainChar.character->isStunned = 1;
										Attack* a = new Attack("10 debuff");
										a->turns = 1;
										mainChar.character->attackPool.Add(a);
										mainChar.character->stats.pDef -= mainChar.character->stats.pDef / 10;
										mainChar.character->stats.mDef -= mainChar.character->stats.mDef / 10;
										break;
									}
									case 2: //GRANDPA
									{
										LOG("Nightmarish used Grasp of Depression on Grandpa!");
										NextLine("Nightmarish used Grasp of Depression on Grandpa!");
										grandpa.character->isStunned = 1;
										Attack* a = new Attack("10 debuff");
										a->turns = 1;
										grandpa.character->attackPool.Add(a);
										grandpa.character->stats.pDef -= grandpa.character->stats.pDef / 10;
										grandpa.character->stats.mDef -= grandpa.character->stats.mDef / 10;
										break;
									}
									//case 3:
									//	break;
									//case 4:
									//	break;
									default:
										break;
									}
									app->audio->PlayFx(graspFx);
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
							if (currentEntity != nullptr) LOG("HP: %d/%d", currentEntity->data->stats.hPoints, currentEntity->data->stats.hPointsMax);
							finishedAction = false;
							hasTicked = false;
							attackSelected.Clear();
							once = true;
							currentEntity = currentEntity->next;
						}
					}
				}
			}
			break;
			case COMBAT_END:
			{
				if (!heDed && ((app->quests->tpFlags & QuestManager::TpFlags::FIGHT) != 0))
					app->quests->firstCombatWon = true;
				if (enemy1 != nullptr) enemiesCount++;
				if (enemy2 != nullptr) enemiesCount++;
				if (enemy3 != nullptr) enemiesCount++;
				if (enemiesCount == 3 && !heDed && 
					(enemy1->id == EntityId::NIGHTMARE || enemy2->id == EntityId::NIGHTMARE || enemy3->id == EntityId::NIGHTMARE) && 
					((app->quests->tpFlags & QuestManager::TpFlags::BOSS_FIGHT) != 0))
					app->quests->firstBossDefeated = true;
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

		// Calls update with gamepad parameters (GUI)
		if (usingGamepad)
		{
			if (changeMenu)
			{
				app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatAttack));
				changeMenu = false;
			}
			if (characterSelected)
			{
				app->scene->currentButton->data->Update(dt, 29, 33);
				if ((combatMenuFlags & 1 << Flags::SKILL) != 0)
				{
					if (changeMenu)
					{
						app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatSkill1));
						changeMenu = false;
					}
					else
					{
						app->scene->currentButton->data->Update(dt, 34, 39);
						if (app->input->CheckButton("cancel", KeyState::KEY_DOWN))
						{
							app->gui->ResetButtons();
							app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatAttack));
							combatMenuFlags = 0;
							changeMenu = true;
						}
					}
				}
				else if ((combatMenuFlags & 1 << Flags::ITEMS) != 0)
				{
					if (changeMenu)
					{
						app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatItem1));
						changeMenu = false;
					}
					else
					{
						app->scene->currentButton->data->Update(dt, 40, 45);
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
		// Calls update for mouse parameters (GUI)
		else
		{
			if (characterSelected)
			{
				btnCombatAttack->Update(dt);
				btnCombatSkills->Update(dt);
				btnCombatItems->Update(dt);
				btnCombatSpecial->Update(dt);
				btnCombatFlee->Update(dt);

				if ((combatMenuFlags & 1 << Flags::SKILL) != 0)
				{
					btnCombatSkill1->Update(dt);
					btnCombatSkill2->Update(dt);
					btnCombatSkill3->Update(dt);
					btnCombatSkill4->Update(dt);
					btnCombatSkill5->Update(dt);
					btnCombatSkill6->Update(dt);
				}
				else if ((combatMenuFlags & 1 << Flags::ITEMS) != 0)
				{
					int i = 0;
					int maxItems = -1;
					if (pageOne)
					{
						i = 0;
						if (items.Count() > 6)
							maxItems = 6;
						else
							maxItems = items.Count();
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
							btnCombatItem1->text = items.At(i)->data->effect.attackName;
							btnCombatItem1->count = items.At(i)->data->countText;
							btnCombatItem1->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem1;
							btnCombatItem1->Update(dt);
							break;
						case 1:
							btnCombatItem2->itemId = items.At(i)->data->id;
							btnCombatItem2->text = items.At(i)->data->effect.attackName;
							btnCombatItem2->count = items.At(i)->data->countText;
							btnCombatItem2->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem2;
							btnCombatItem2->Update(dt);
							break;
						case 2:
							btnCombatItem3->itemId = items.At(i)->data->id;
							btnCombatItem3->text = items.At(i)->data->effect.attackName;
							btnCombatItem3->count = items.At(i)->data->countText;
							btnCombatItem3->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem3;
							btnCombatItem3->Update(dt);
							break;
						case 3:
							btnCombatItem4->itemId = items.At(i)->data->id;
							btnCombatItem4->text = items.At(i)->data->effect.attackName;
							btnCombatItem4->count = items.At(i)->data->countText;
							btnCombatItem4->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem4;
							btnCombatItem4->Update(dt);
							break;
						case 4:
							btnCombatItem5->itemId = items.At(i)->data->id;
							btnCombatItem5->text = items.At(i)->data->effect.attackName;
							btnCombatItem5->count = items.At(i)->data->countText;
							btnCombatItem5->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem5;
							btnCombatItem5->Update(dt);
							break;
						case 5:
							btnCombatItem6->itemId = items.At(i)->data->id;
							btnCombatItem6->text = items.At(i)->data->effect.attackName;
							btnCombatItem6->count = items.At(i)->data->countText;
							btnCombatItem6->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem6;
							btnCombatItem6->Update(dt);
							break;
						case 6:
							btnCombatItem7->itemId = items.At(i)->data->id;
							btnCombatItem7->text = items.At(i)->data->effect.attackName;
							btnCombatItem7->count = items.At(i)->data->countText;
							btnCombatItem7->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem7;
							btnCombatItem7->Update(dt);
							break;
						case 7:
							btnCombatItem8->itemId = items.At(i)->data->id;
							btnCombatItem8->text = items.At(i)->data->effect.attackName;
							btnCombatItem8->count = items.At(i)->data->countText;
							btnCombatItem8->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem8;
							btnCombatItem8->Update(dt);
							break;
						case 8:
							btnCombatItem9->itemId = items.At(i)->data->id;
							btnCombatItem9->text = items.At(i)->data->effect.attackName;
							btnCombatItem9->count = items.At(i)->data->countText;
							btnCombatItem9->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem9;
							btnCombatItem9->Update(dt);
							break;
						case 9:
							btnCombatItem10->itemId = items.At(i)->data->id;
							btnCombatItem10->text = items.At(i)->data->effect.attackName;
							btnCombatItem10->count = items.At(i)->data->countText;
							btnCombatItem10->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem10;
							btnCombatItem10->Update(dt);
							break;
						case 10:
							btnCombatItem11->itemId = items.At(i)->data->id;
							btnCombatItem11->text = items.At(i)->data->effect.attackName;
							btnCombatItem11->count = items.At(i)->data->countText;
							btnCombatItem11->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem11;
							btnCombatItem11->Update(dt);
							break;
						case 11:
							btnCombatItem12->itemId = items.At(i)->data->id;
							btnCombatItem12->text = items.At(i)->data->effect.attackName;
							btnCombatItem12->count = items.At(i)->data->countText;
							btnCombatItem12->sec = items.At(i)->data->texSec;
							items.At(i)->data->button = btnCombatItem12;
							btnCombatItem12->Update(dt);
							break;
						default:
							break;
						}
					}
					if (items.Count() <= 6)
					{
						btnLeftArrow->Update(dt);
						btnRightArrow->Update(dt);
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
            if (app->map->data.name == "tutorial.tmx") app->audio->PlayMusic("Audio/Music/Tutorial.ogg");
            else if (app->map->data.name == "home.tmx") app->audio->PlayMusic("Audio/Music/Home.ogg");
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
				app->render->DrawText(dialogueFont, mainChar.hp.GetString(), /*-app->render->camera.x +*/ mainChar.character->entityRect.x + mainChar.characterTex.w + 15,/* -app->render->camera.y +*/ mainChar.character->entityRect.y + 45, 28, 1, white);
				app->render->DrawText(dialogueFont, mainChar.mp.GetString(), /*-app->render->camera.x +*/ mainChar.character->entityRect.x + mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ mainChar.character->entityRect.y + 45 + 30, 28, 1, white);
				app->render->DrawText(dialogueFont, mainChar.stress.GetString(), /*-app->render->camera.x +*/ mainChar.character->entityRect.x + mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ mainChar.character->entityRect.y + 45 + 60, 28, 1, white);
			}
			if (characterFlags >= 3)
			{
				app->render->DrawTexture(combatGui, -app->render->camera.x + grandpa.character->entityRect.x, -app->render->camera.y + grandpa.character->entityRect.y, false, &grandpa.box);
				app->render->DrawTexture(combatGui, -app->render->camera.x + grandpa.character->entityRect.x + 10, -app->render->camera.y + grandpa.character->entityRect.y + (grandpa.box.h / 2 - grandpa.characterTex.h / 2), false, &grandpa.characterTex);
				app->render->DrawText(dialogueFont, grandpa.hp.GetString(), /*-app->render->camera.x +*/ grandpa.character->entityRect.x + grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ grandpa.character->entityRect.y + 50, 28, 1, white);
				app->render->DrawText(dialogueFont, grandpa.mp.GetString(), /*-app->render->camera.x +*/ grandpa.character->entityRect.x + grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ grandpa.character->entityRect.y + 50 + 30, 28, 1, white);
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
				app->render->DrawText(dialogueFont, currentChar->hp.GetString(), /*-app->render->camera.x +*/ app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w, /*-app->render->camera.y +*/ app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4), 28, 1, white);
				app->render->DrawText(dialogueFont, currentChar->mp.GetString(), /*-app->render->camera.x +*/ app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w, /*-app->render->camera.y +*/ app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4) + 30, 28, 1, white);
				if (currentChar->character->id == EntityId::MC) app->render->DrawText(dialogueFont, currentChar->stress.GetString(),/* -app->render->camera.x + */app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w,/* -app->render->camera.y +*/ app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4) + 60, 28, 1, white);
			}
		}
	}

	if (waitForTransition == TransitionStatus::END)
	{
		app->render->DrawText(dialogueFont, firstLine.GetString(), /*-app->render->camera.x*/ +44, /*-app->render->camera.y*/ +52, 48, 2, white);
		app->render->DrawText(dialogueFont, secondLine.GetString(), /*-app->render->camera.x*/ +44, /*-app->render->camera.y*/ +52 + 48, 48, 2, white);
		app->render->DrawText(dialogueFont, thirdLine.GetString(), /*-app->render->camera.x*/ +44, /*-app->render->camera.y*/ +52 + 96, 48, 2, white);
		if (wait && !blink)
		{
			app->render->DrawTexture(combatGui, -app->render->camera.x + combatTextBox.w - (arrowCombat.w / 2) - arrowCombat.w - 20, -app->render->camera.y + combatTextBox.h - arrowCombat.h - (arrowCombat.h / 2) - 10, false, &arrowCombat);
		}

		if (characterSelected)
		{
			btnCombatAttack->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatSkills->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatItems->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatSpecial->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatFlee->Draw(-app->render->camera.x, -app->render->camera.y);

			if ((combatMenuFlags & 1 << Flags::SKILL) != 0)
			{
				btnCombatSkill1->Draw(-app->render->camera.x, -app->render->camera.y);
				btnCombatSkill2->Draw(-app->render->camera.x, -app->render->camera.y);
				btnCombatSkill3->Draw(-app->render->camera.x, -app->render->camera.y);
				btnCombatSkill4->Draw(-app->render->camera.x, -app->render->camera.y);
				btnCombatSkill5->Draw(-app->render->camera.x, -app->render->camera.y);
				btnCombatSkill6->Draw(-app->render->camera.x, -app->render->camera.y);
			}
			else if ((combatMenuFlags & 1 << Flags::ITEMS) != 0)
			{
				//if (pageOne)
				//{
				//	btnCombatItem1->Draw(-app->render->camera.x, -app->render->camera.y);
				//	btnCombatItem2->Draw(-app->render->camera.x, -app->render->camera.y);
				//	btnCombatItem3->Draw(-app->render->camera.x, -app->render->camera.y);
				//	btnCombatItem4->Draw(-app->render->camera.x, -app->render->camera.y);
				//	btnCombatItem5->Draw(-app->render->camera.x, -app->render->camera.y);
				//	btnCombatItem6->Draw(-app->render->camera.x, -app->render->camera.y);
				//}
				//else
				//{
				//	btnCombatItem7->Draw(-app->render->camera.x, -app->render->camera.y);
				//	btnCombatItem8->Draw(-app->render->camera.x, -app->render->camera.y);
				//	btnCombatItem9->Draw(-app->render->camera.x, -app->render->camera.y);
				//	btnCombatItem10->Draw(-app->render->camera.x, -app->render->camera.y);
				//	btnCombatItem11->Draw(-app->render->camera.x, -app->render->camera.y);
				//	btnCombatItem12->Draw(-app->render->camera.x, -app->render->camera.y);
				//}

				int i = 0;
				int maxItems = -1;
				if (pageOne)
				{
					i = 0;
					if (items.Count() > 6)
						maxItems = 6;
					else
						maxItems = items.Count();
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
				if (items.Count() <= 6)
				{
					btnLeftArrow->Draw(-app->render->camera.x, -app->render->camera.y);
					btnRightArrow->Draw(-app->render->camera.x, -app->render->camera.y);
				}
			}
			else if ((combatMenuFlags & 1 << Flags::SPECIAL) != 0)
			{

			}

		}

		if (targetAttack || targetItem) //add more as we go
		{
			int x, y;
			app->input->GetMousePosition(x, y);
			ListItem<CombatEntity*>* e = turnOrder.start;
			while (e != nullptr)
			{
				if (e->data->collider->Intersects({ x, y, 1, 1 }))
					app->render->DrawRectangle({ -app->render->camera.x + e->data->collider->rect.x, -app->render->camera.y + e->data->collider->rect.y,e->data->collider->rect.w,e->data->collider->rect.h }, 255, 255, 255, 150, false);

				e = e->next;
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
	// cool animation as a victory thing and change back to gameplay
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

	firstLine.Clear();
	secondLine.Clear();
	thirdLine.Clear();

	transition.Reset();
    if (!heDed) backToGameplay = true;

	enemy1 = nullptr;
	enemy2 = nullptr;
	enemy3 = nullptr;
	currentChar = nullptr;
	target = nullptr;
	currentEntity = nullptr;
	characterSelected = false;
	targetAttack = false;
	finishedAction = false;
	hasTicked = false;
	attackSelected.Clear();
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
	mainChar.hp.Clear();
	mainChar.mp.Clear();
	mainChar.stress.Clear();

	grandpa.hp.Clear();
	grandpa.mp.Clear();
	grandpa.stress.Clear();

	if (combatGui != nullptr) app->tex->UnLoad(combatGui);
	if (transitionTx != nullptr) app->tex->UnLoad(transitionTx);
    
    app->audio->UnloadFx(smackFx);
    app->audio->UnloadFx(slapFx);
    app->audio->UnloadFx(confortFx);
    app->audio->UnloadFx(smiteFx);
    app->audio->UnloadFx(magicBlowFx);
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

	if (count != 0)
	{
		SortSpeed(false);
	}
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
		// Select target with gamepad
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

void SceneCombat::TickDownBuffs()
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
				if (a->data->attackName == "10 debuff")
				{
					currentEntity->data->stats.pDef += a->data->beforeBuff1 / 10;
					currentEntity->data->stats.mDef += a->data->beforeBuff2 / 10;
				}
				else if (a->data->attackName == "5 buff defenses")
				{
					currentEntity->data->stats.pDef -= (a->data->beforeBuff1 * 5) / 100;
					currentEntity->data->stats.mDef -= (a->data->beforeBuff2 * 5) / 100;
				}
				else if (a->data->attackName == "10 buff defenses")
				{
					currentEntity->data->stats.pDef -= a->data->beforeBuff1 / 10;
					currentEntity->data->stats.mDef -= a->data->beforeBuff2 / 10;
				}
				else if (a->data->attackName == "25 buff")
				{
					currentEntity->data->stats.pAtk -= (a->data->beforeBuff1 * 25) / 100;
					currentEntity->data->stats.mAtk -= (a->data->beforeBuff2 * 25) / 100;
				}
				else if (a->data->attackName == "5 buff")
				{
					currentEntity->data->stats.pAtk -= (a->data->beforeBuff1 * 5) / 100;
					currentEntity->data->stats.mAtk -= (a->data->beforeBuff2 * 5) / 100;
				}
				currentEntity->data->attackPool.Del(a);
				if(a != nullptr) a = nullptr;
			}
		}
		if (aNext != nullptr) a = aNext;
		else break;
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
		LOG("%s does damage attack to %s!", currentEntity->data->name.GetString(), target->name.GetString());
		char tmp[75];
		sprintf(tmp, "%s does damage attack to %s!", currentEntity->data->name.GetString(), target->name.GetString());
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
			LOG("%s does damage attack to %s!", currentEntity->data->name.GetString(), target->name.GetString());
			char tmp[75];
			sprintf(tmp, "%s does damage attack to %s!", currentEntity->data->name.GetString(), target->name.GetString());
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
	mainChar.hp.Create("HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
	grandpa.hp.Create("HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
	//Should add the rest of the characters
}

void SceneCombat::Heal(CombatEntity* target, int p)
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
		LOG("%s heals to %s!", currentEntity->data->name.GetString(), target->name.GetString());
		char tmp[75];
		sprintf(tmp, "%s heals %s!", currentEntity->data->name.GetString(), target->name.GetString());
		NextLine(tmp);

		int heal = p * target->stats.hPointsMax;

		if (heal <= 0) heal = 0;
		target->stats.hPoints += heal;
		if (target->stats.hPoints >= target->stats.hPointsMax) target->stats.hPoints = target->stats.hPointsMax;
	}
	mainChar.hp.Create("HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
	grandpa.hp.Create("HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
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
		LOG("%s does a stressing attack to %s!", currentEntity->data->name.GetString(), mainChar.character->name.GetString());
		char tmp[75];
		sprintf(tmp, "%s does a stressing attack to %s!", currentEntity->data->name.GetString(), mainChar.character->name.GetString());
		NextLine(tmp);

		int heal = value * mainChar.character->stats.hPointsMax;

		if (heal <= 0) heal = 0;
		mainChar.character->stats.hPoints += heal;
		if (mainChar.character->stats.hPoints >= mainChar.character->stats.hPointsMax) mainChar.character->stats.hPoints = mainChar.character->stats.hPointsMax;
	}
	mainChar.stress.Create("ST: %d/%d", mainChar.character->stats.stress, mainChar.character->stats.stressMax);
}

void SceneCombat::VictoryCondition()
{
	ListItem<CombatEntity*>* e = turnOrder.start;
	int counter = 0;
	while (e != nullptr)
	{
		if (IsCharacter(e->data) == false)
			counter++;

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
		if (IsCharacter(e->data) == true)
			counter++;
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
	firstLine.Clear();
	firstLine.Create(secondLine.GetString());
	secondLine.Clear();
	secondLine.Create(thirdLine.GetString());
	thirdLine.Clear();
	thirdLine.Create(line);
}


//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneCombat::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
		//COMBAT
	case 41: //ATTACK
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			attackSelected.Create("Smack");
			break;
		case EntityId::VIOLENT:
			attackSelected.Create("Smite foes");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 42: //SKILLS
		app->gui->ResetButtons();
		btnCombatSkills->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::SKILL);
		changeMenu = true;
		break;
	case 43: //ITEMS
		app->gui->ResetButtons();
		btnCombatItems->state = GuiControlState::DISABLED;
		btnLeftArrow->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::ITEMS);
		changeMenu = true;
		break;
	case 44: //SPECIAL
		app->gui->ResetButtons();
		btnCombatSpecial->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::SPECIAL);
		changeMenu = true;
		break;
	case 45: //FLEE
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
	case 46: //SKILL 1
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			attackSelected.Create("Comfort");
			break;
		case EntityId::VIOLENT:
			attackSelected.Create("Weak Magic missile");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 47: //SKILL 2
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			attackSelected.Create("Slap");
			break;
		case EntityId::VIOLENT:
			attackSelected.Create("Weak Magic Barrage");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 48: //SKILL 3
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			attackSelected.Create("Encourage");
			break;
		case EntityId::VIOLENT:
			attackSelected.Create("Magic Hand Slap");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 49: //SKILL 4
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			attackSelected.Create("Boring Speech");
			break;
		case EntityId::VIOLENT:
			attackSelected.Create("Judgemental Stare");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 50: //SKILL 5
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			attackSelected.Create("MC Skill 5");
			break;
		case EntityId::VIOLENT:
			attackSelected.Create("Grandpa Skill 5");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 51: //SKILL 6
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		switch (currentChar->character->id)
		{
		case EntityId::MC:
			attackSelected.Create("MC Skill 6");
			break;
		case EntityId::VIOLENT:
			attackSelected.Create("Grandpa Skill 6");
			break;
		default:
			break;
		}
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 52: //ITEM 1
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 1;
		targetItem = true;
		LOG("USED %s", btnCombatItem1->text.GetString());
		break;
	case 53: //ITEM 2
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 2;
		targetItem = true;
		LOG("USED %s", btnCombatItem2->text.GetString());
		break;
	case 54: //ITEM 3
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 3;
		targetItem = true;
		LOG("USED %s", btnCombatItem3->text.GetString());
		break;
	case 55: //ITEM 4
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 4;
		targetItem = true;
		LOG("USED %s", btnCombatItem4->text.GetString());
		break;
	case 56: //ITEM 5
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 5;
		targetItem = true;
		LOG("USED %s", btnCombatItem5->text.GetString());
		break;
	case 57: //ITEM 6
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 6;
		targetItem = true;
		LOG("USED %s", btnCombatItem6->text.GetString());
		break;
	case 58: //ITEM 7
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 7;
		targetItem = true;
		LOG("USED %s", btnCombatItem7->text.GetString());
		break;
	case 59: //ITEM 8
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 8;
		targetItem = true;
		LOG("USED %s", btnCombatItem8->text.GetString());
		break;
	case 60: //ITEM 9
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 9;
		targetItem = true;
		LOG("USED %s", btnCombatItem9->text.GetString());
		break;
	case 61: //ITEM 10
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 10;
		targetItem = true;
		LOG("USED %s", btnCombatItem10->text.GetString());
		break;
	case 62: //ITEM 11
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 11;
		targetItem = true;
		LOG("USED %s", btnCombatItem11->text.GetString());
		break;
	case 63: //ITEM 12
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		itemSelected = 12;
		targetItem = true;
		LOG("USED %s", btnCombatItem12->text.GetString());
		break;
	case 64: //LEFT ARROW
		pageOne = true;
		btnLeftArrow->state = GuiControlState::DISABLED;
		btnRightArrow->state = GuiControlState::NORMAL;
		break;
	case 65: //RIGHT ARROW
		pageOne = false;
		btnLeftArrow->state = GuiControlState::NORMAL;
		btnRightArrow->state = GuiControlState::DISABLED;
		break;
	default:
		break;
	}

	return true;
}