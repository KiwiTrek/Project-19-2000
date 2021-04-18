#include "SceneCombat.h"

#include "Audio.h"
#include "EntityManager.h"
#include "CombatEntity.h"
#include "Player.h"
#include "Textures.h"
#include "GuiManager.h"
#include "SceneManager.h"
#include "SceneGameplay.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"

SceneCombat::SceneCombat()
{
	flags = 0;

	characterFlags = 0;
	characterFlags = SetBit(characterFlags, (uint)EntityId::MC);
	characterFlags = SetBit(characterFlags, (uint)EntityId::VIOLENT);
}

SceneCombat::~SceneCombat()
{}

bool SceneCombat::Load()
{
	// COMBAT
	combatGui = app->tex->Load("Assets/Textures/GUI/combatGui.png");
	scripted = false;

	currentChar = nullptr;
	target = nullptr;
	currentEntity = nullptr;
	mainChar.box = { 1280,0,204,190 };
	mainChar.characterTex = { 0,252,72,92 };
	mainChar.character = (CombatEntity*)app->entities->CreateEntity(36, app->render->camera.h - mainChar.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::MC, Stats(20, 0, 10, 10, 100, 100, 50, 100, 100, 0, 100));
	mainChar.hp.Create("HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
	mainChar.mp.Create("MP: %d/%d", mainChar.character->stats.mPoints, mainChar.character->stats.mPointsMax);
	mainChar.stress.Create("ST: %d/%d", mainChar.character->stats.stress, mainChar.character->stats.stressMax);

	grandpa.box = { 1280,0,204,190 };
	grandpa.characterTex = { 73,252,68,100 };
	grandpa.character = (CombatEntity*)app->entities->CreateEntity(grandpa.box.w + 36, app->render->camera.h - grandpa.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::VIOLENT, Stats(25, 25, 0, 0, 100, 100, 25, 100, 100));
	grandpa.hp.Create("HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
	grandpa.mp.Create("MP: %d/%d", grandpa.character->stats.mPoints, grandpa.character->stats.mPointsMax);

	//enemy1 = app->entities->CreateEntity(300, 300, EntityType::COMBAT_ENTITY, EntityId::STRESSING_SHADOW, Stats(0, 10, 10, 10, 40, 40, 60));
	//enemy2 = app->entities->CreateEntity(700, 300, EntityType::COMBAT_ENTITY, EntityId::FURIOUS_SHADOW, Stats(15, 0, 20, 5, 60, 60, 90));

		//COMBAT
	combatTextBox = { 0,0,1280,248 };
	combatMenuBox = { 305,249,1001,130 };
	btnCombatAttack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 29, { 34,505,200,60 }, "ATTACK", 40, this);
	btnCombatSkills = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 30, { 234,505,200,60 }, "SKILLS", 40, this);
	btnCombatItems = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 31, { 434,505,200,60 }, "ITEMS", 40, this);
	btnCombatSpecial = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 32, { 634,505,200,60 }, "SPECIAL", 40, this);
	btnCombatSpecial->state = GuiControlState::DISABLED;
	btnCombatFlee = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 33, { 834,505,200,60 }, "FLEE", 40, this);

	btnCombatSkill1 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 34, { 36, app->render->camera.h - combatMenuBox.h - 25,200,60 }, "skill 1", 40, this);
	btnCombatSkill2 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 35, { 256, app->render->camera.h - combatMenuBox.h - 25,200,60 }, "skill 2", 40, this);
	btnCombatSkill3 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 36, { 476, app->render->camera.h - combatMenuBox.h - 25,200,60 }, "skill 3", 40, this);
	btnCombatSkill4 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 37, { 36, app->render->camera.h - combatMenuBox.h + 35,200,60 }, "skill 4", 40, this);
	btnCombatSkill5 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 38, { 256, app->render->camera.h - combatMenuBox.h + 35,200,60 }, "skill 5", 40, this);
	btnCombatSkill6 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 39, { 476, app->render->camera.h - combatMenuBox.h + 35,200,60 }, "skill 6", 40, this);

	btnCombatItem1 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 40, { 36, app->render->camera.h - combatMenuBox.h - 25,200,60 }, "item 1", 40, this);
	btnCombatItem2 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 41, { 256, app->render->camera.h - combatMenuBox.h - 25,200,60 }, "item 2", 40, this);
	btnCombatItem3 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 42, { 476, app->render->camera.h - combatMenuBox.h - 25,200,60 }, "item 3", 40, this);
	btnCombatItem4 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 43, { 36, app->render->camera.h - combatMenuBox.h + 35,200,60 }, "item 4", 40, this);
	btnCombatItem5 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 44, { 256, app->render->camera.h - combatMenuBox.h + 35,200,60 }, "item 5", 40, this);
	btnCombatItem6 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 45, { 476, app->render->camera.h - combatMenuBox.h + 35,200,60 }, "item 6", 40, this);

	LOG("%d", characterFlags);

	heDed = false;

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
	attackSelected = -1;
	combatMenuFlags = 0;
	once = true;
	scripted = false;

	firstLine.Clear();
	secondLine.Clear();
	thirdLine.Clear();

	ListItem<CombatEntity*>* e = turnOrder.start;
	while (e != nullptr)
	{
		ListItem<CombatEntity*>* eNext = e->next;
		turnOrder.Del(e);
		e = eNext;
	}
	turnOrder.Clear();

	SpawnEnemies(id1, id2, id3);

	app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnCombatAttack));
	if (app->input->GetControllerName() != "unplugged") usingGamepad = true;
	app->input->mouseMotionX = 0;
	app->input->mouseMotionY = 0;

	heDed = false;

	combatState = CombatStateType::COMBAT_START;

	return true;
}

bool SceneCombat::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) characterSelected = !characterSelected;
	//LOG("%d", app->entities->entities.Count());
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
				turnOrder.Add((CombatEntity*)e->data);
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
			if (!hasTicked)
			{
				TickDownBuffs();
				hasTicked = true;
			}
			if (currentEntity->data->stats.hPoints == 0)
			{
				//ListItem<CombatEntity*>* e = currentEntity;
				//currentEntity = currentEntity->next;
				LOG("%s is dead!", currentEntity->data->name.GetString());
				char tmp[50];
				sprintf(tmp, "%s is dead!", currentEntity->data->name.GetString());
				NextLine(tmp);
				//turnOrder.Del(e);

				ListItem<CombatEntity*>* eNext = currentEntity->next;
				if (!IsCharacter(currentEntity->data)) app->entities->DestroyEntity(currentEntity->data);
				turnOrder.Del(currentEntity);
				currentEntity = eNext;
				currentChar = nullptr;
				characterSelected = false;
				if (currentEntity == nullptr)
				{
					SortSpeed(false);
					currentEntity = turnOrder.start;
				}

				VictoryCondition();
				DefeatCondition();
			}

			if (IsCharacter(currentEntity->data))													// CHARACTER
			{
				//WAIT FOR PLAYER INPUT
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

				switch (currentEntity->data->id)
				{
				case EntityId::MC:
					currentChar = &mainChar;

					if (currentEntity->data->isStunned == false)
					{
						if (currentEntity->data->isTaunted == true)
						{
							currentEntity->data->isTaunted = false;
							target = currentEntity->data->tauntedBy;
							attackSelected = 0;
						}

						if (attackSelected == -1)
						{
						}
						else if (currentEntity->data->attackPool.At(attackSelected)->data->target == TargetType::ONE)
						{
							targetAttack = true;
							if (target == nullptr)
								SelectTarget();

							if (target != nullptr)
							{
								switch (attackSelected)
								{
								case 0: //attack
									Damage(0, target);
									break;
									//case 1: //skill 1
									//	LOG("skill 1");
									//	break;
									//case 2: //skill 2
									//	LOG("skill 2");
									//	break;
									//case 3: //skill 3
									//	LOG("skill 3");
									//	break;
									//case 4: //skill 4
									//	LOG("skill 4");
									//	break;
									//case 5: //skill 5
									//	LOG("skill 5");
									//	break;
									//case 6: //skill 6
									//	LOG("skill 6");
									//	break;
								default:
									break;
								}
								attackSelected = -1;
								target = nullptr;
								targetAttack = false;
								finishedAction = true;
							}
						}
						else
						{
							switch (attackSelected)
							{
							case 1: //skill 1
								LOG("skill 1");
								break;
							case 2: //skill 2
								LOG("skill 2");
								break;
							case 3: //skill 3
								LOG("skill 3");
								break;
							case 4: //skill 4
								LOG("skill 4");
								break;
							case 5: //skill 5
								LOG("skill 5");
								break;
							case 6: //skill 6
								LOG("skill 6");
								break;
							default:
								break;
							}
							attackSelected = -1;
							finishedAction = true;
						}
					}
					else
						finishedAction = true;

					break;
				case EntityId::VIOLENT:
					currentChar = &grandpa;

					if (currentEntity->data->isStunned == false)
					{
						if (currentEntity->data->isTaunted == true)
						{
							currentEntity->data->isTaunted = false;
							target = currentEntity->data->tauntedBy;
							attackSelected = 0;
						}

						if (attackSelected == -1)
						{
						}
						else if (currentEntity->data->attackPool.At(attackSelected)->data->target == TargetType::ONE)
						{
							targetAttack = true;
							if (target == nullptr)
								SelectTarget();

							if (target != nullptr)
							{
								switch (attackSelected)
								{
								case 0: //attack
									Damage(0, target);
									break;
								//case 1: //skill 1
								//	LOG("skill 1");
								//	break;
								//case 2: //skill 2
								//	LOG("skill 2");
								//	break;
								//case 3: //skill 3
								//	LOG("skill 3");
								//	break;
								//case 4: //skill 4
								//	LOG("skill 4");
								//	break;
								//case 5: //skill 5
								//	LOG("skill 5");
								//	break;
								//case 6: //skill 6
								//	LOG("skill 6");
								//	break;
								default:
									break;
								}
								attackSelected = -1;
								target = nullptr;
								targetAttack = false;
								finishedAction = true;
							}
						}
						else
						{
							switch (attackSelected)
							{
							case 1: //skill 1
								LOG("skill 1");
								break;
							case 2: //skill 2
								LOG("skill 2");
								break;
							case 3: //skill 3
								LOG("skill 3");
								break;
							case 4: //skill 4
								LOG("skill 4");
								break;
							case 5: //skill 5
								LOG("skill 5");
								break;
							case 6: //skill 6
								LOG("skill 6");
								break;
							default:
								break;
							}
							attackSelected = -1;
							finishedAction = true;
						}
					}
					else
						finishedAction = true;

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
					srand(time(NULL));
					int p = rand() % 10 + 1;
					if (p >= 6) //Stressing attack
					{
						//dialogue that X enemy does Y attack to MC
						LOG("%s does stress attack!", currentEntity->data->name.GetString());
						char tmp[75];
						sprintf(tmp, "%s does stress attack!", currentEntity->data->name.GetString());
						NextLine(tmp);
						mainChar.character->stats.stress += 10;
						mainChar.stress.Create("ST: %d/%d", mainChar.character->stats.stress, mainChar.character->stats.stressMax);
					}
					else //Magical blow
					{
						int t = EnemyTarget();
						switch (t)
						{
						case 1: //MC
							Damage(0, mainChar.character, true);
							mainChar.hp.Create("HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
							break;
						case 2: //GRANDPA
							Damage(0, grandpa.character, true);
							grandpa.hp.Create("HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
							break;
						case 3:
							break;
						case 4:
							break;
						default:
							break;
						}
					}
					finishedAction = true;
					break;
				}
				case EntityId::FURIOUS_SHADOW:
				{
					srand(time(NULL));
					int p = rand() % 10 + 1;
					if (p >= 5) //Getting stronger
					{
						//dialogue that X enemy does Y attack
						LOG("Furious Shadow got stronger!");
						NextLine("Furious Shadow got stronger!");
						currentEntity->data->stats.pDef += (currentEntity->data->stats.pDef * 5) / 100;
						currentEntity->data->stats.mDef += (currentEntity->data->stats.mDef * 5) / 100;
						currentEntity->data->attackPool.At(0)->data->turns = 2;
					}
					else //Fury of blades
					{
						Damage(1, mainChar.character);
						mainChar.hp.Create("HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
						Damage(1, grandpa.character);
						grandpa.hp.Create("HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
						//dialogue that X enemy does Y attack to all characters
						LOG("Furious Shadow attack to all!\n");
						NextLine("Furious Shadow attack to all!");
					}
					finishedAction = true;
					break;
				}
				case EntityId::NIGHTMARE:
				{
					srand(time(NULL));
					int p = rand() % 100 + 1;
					if (p >= 60) //Bad dream
					{
						int t = EnemyTarget();
						switch (t)
						{
						case 1: //MC
							Damage(0, mainChar.character);
							mainChar.hp.Create("HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
							break;
						case 2: //GRANDPA
							Damage(0, grandpa.character);
							grandpa.hp.Create("HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
							break;
						case 3:
							break;
						case 4:
							break;
						default:
							break;
						}
					}
					else if(p >= 30) //Nightmarish
					{
						//dialogue that X enemy does Y attack
						LOG("Nightmarish got stronger!");
						NextLine("Nightmarish got stronger!");
						currentEntity->data->stats.pDef += ((currentEntity->data->stats.pDef * 10) / 100);
						currentEntity->data->stats.mDef += ((currentEntity->data->stats.mDef * 10) / 100);
						currentEntity->data->attackPool.At(0)->data->turns = 2;
					}
					else if (p >= 5) //Close your eyes
					{
						int t = EnemyTarget();
						switch (t)
						{
						case 1: //MC
							//dialogue that X enemy does Y attack to Z character
							LOG("mc has been taunted!");
							NextLine("You have been taunted!");
							mainChar.character->isTaunted = true;
							mainChar.character->tauntedBy = currentEntity->data;
							break;
						case 2: //GRANDPA
							//dialogue that X enemy does Y attack to Z character
							LOG("grandpa has been taunted!");
							NextLine("Grandpa has been taunted!");
							grandpa.character->isTaunted = true;
							grandpa.character->tauntedBy = currentEntity->data;
							break;
						case 3:
							break;
						case 4:
							break;
						default:
							break;
						}
					}
					else //Grasp of depression
					{
						int t = EnemyTarget();
						switch (t)
						{
						case 1: //MC
						{
							//dialogue that X enemy does Y attack to Z character
							LOG("mc has been stunned and debuffed!");
							NextLine("You have been stunned and debuffed!");
							mainChar.character->isStunned = true;
							SString s = "10% debuff";
							Attack* a = new Attack(s, AttackType::BUFF, TargetType::SELF, mainChar.character->stats.pDef, mainChar.character->stats.mDef);
							a->turns = 1;
							mainChar.character->attackPool.Add(a);
							mainChar.character->stats.pDef -= a->stat1 / 10;
							mainChar.character->stats.mDef -= a->stat2 / 10;
							break;
						}
						case 2: //GRANDPA
						{
							//dialogue that X enemy does Y attack to Z character
							LOG("grandpa has been stunned and debuffed!");
							NextLine("Grandpa have been stunned and debuffed!");
							grandpa.character->isStunned = true;
							SString s = "10% debuff";
							Attack* a = new Attack(s, AttackType::BUFF, TargetType::SELF, grandpa.character->stats.pDef, grandpa.character->stats.mDef);
							a->turns = 1;
							grandpa.character->attackPool.Add(a);
							grandpa.character->stats.pDef -= a->stat1 / 10;
							grandpa.character->stats.mDef -= a->stat2 / 10;
							break;
						}
						case 3:
							break;
						case 4:
							break;
						default:
							break;
						}
					}

					finishedAction = true;
					break;
				}
				default:
					break;
				}
			}

			if (finishedAction)
			{
				LOG("HP: %d/%d", currentEntity->data->stats.hPoints, currentEntity->data->stats.hPointsMax);
				finishedAction = false;
				hasTicked = false;
				once = true;
				currentEntity = currentEntity->next;
			}
		}
	}
	break;
	case COMBAT_END:
	{
		Finish();
		break;
	}
	default:
		break;
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
				btnCombatItem1->Update(dt);
				btnCombatItem2->Update(dt);
				btnCombatItem3->Update(dt);
				btnCombatItem4->Update(dt);
				btnCombatItem5->Update(dt);
				btnCombatItem6->Update(dt);
			}
		}
	}
	

	return true;
}

bool SceneCombat::Draw(Font* dialogueFont)
{
	//switch (ZONE)
	app->render->DrawTexture(combatGui, -app->render->camera.x, -app->render->camera.y, false, &combatTextBox);
	app->render->DrawTexture(combatGui, -app->render->camera.x, -app->render->camera.y + app->render->camera.h - combatTextBox.h, false, &combatTextBox);

	app->render->DrawText(dialogueFont, firstLine.GetString(), /*-app->render->camera.x*/ + 44, /*-app->render->camera.y*/ + 52, 48, 2, { 255,255,255,255 });
	app->render->DrawText(dialogueFont, secondLine.GetString(), /*-app->render->camera.x*/ + 44, /*-app->render->camera.y*/ + 52 + 48, 48, 2, { 255,255,255,255 });
	app->render->DrawText(dialogueFont, thirdLine.GetString(), /*-app->render->camera.x*/ + 44, /*-app->render->camera.y*/ + 52 + 96, 48, 2, { 255,255,255,255 });

	//app->render->DrawRectangle({ 1280 / 2 - 64,720 / 2 - 64,128,128 }, 0, 255, 255, 255);

	if (!characterSelected)
	{
		if (characterFlags >= 1)
		{
			app->render->DrawTexture(combatGui, -app->render->camera.x + mainChar.character->entityRect.x, -app->render->camera.y + mainChar.character->entityRect.y, false, &mainChar.box);
			app->render->DrawTexture(combatGui, -app->render->camera.x + mainChar.character->entityRect.x + 10, -app->render->camera.y + mainChar.character->entityRect.y + (mainChar.box.h / 2 - mainChar.characterTex.h / 2), false, &mainChar.characterTex);
			app->render->DrawText(dialogueFont, mainChar.hp.GetString(), /*-app->render->camera.x +*/ mainChar.character->entityRect.x + mainChar.characterTex.w + 15,/* -app->render->camera.y +*/ mainChar.character->entityRect.y + 45, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, mainChar.mp.GetString(), /*-app->render->camera.x +*/ mainChar.character->entityRect.x + mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ mainChar.character->entityRect.y + 45 + 30, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, mainChar.stress.GetString(), /*-app->render->camera.x +*/ mainChar.character->entityRect.x + mainChar.characterTex.w + 15, /*-app->render->camera.y +*/ mainChar.character->entityRect.y + 45 + 60, 28, 1, { 255,255,255,255 });
		}
		if (characterFlags >= 3)
		{
			app->render->DrawTexture(combatGui, -app->render->camera.x + grandpa.character->entityRect.x, -app->render->camera.y + grandpa.character->entityRect.y, false, &grandpa.box);
			app->render->DrawTexture(combatGui, -app->render->camera.x + grandpa.character->entityRect.x + 10, -app->render->camera.y + grandpa.character->entityRect.y + (grandpa.box.h / 2 - grandpa.characterTex.h / 2), false, &grandpa.characterTex);
			app->render->DrawText(dialogueFont, grandpa.hp.GetString(), /*-app->render->camera.x +*/ grandpa.character->entityRect.x + grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ grandpa.character->entityRect.y + 50, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, grandpa.mp.GetString(), /*-app->render->camera.x +*/ grandpa.character->entityRect.x + grandpa.characterTex.w + 15, /*-app->render->camera.y +*/ grandpa.character->entityRect.y + 50 + 30, 28, 1, { 255,255,255,255 });
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
			app->render->DrawText(dialogueFont, currentChar->hp.GetString(), /*-app->render->camera.x +*/ app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w, /*-app->render->camera.y +*/ app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4), 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, currentChar->mp.GetString(), /*-app->render->camera.x +*/ app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w, /*-app->render->camera.y +*/ app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4) + 30, 28, 1, { 255,255,255,255 });
			if (currentChar->character->id == EntityId::MC) app->render->DrawText(dialogueFont, currentChar->stress.GetString(),/* -app->render->camera.x + */app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w,/* -app->render->camera.y +*/ app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4) + 60, 28, 1, { 255,255,255,255 });
		}

		btnCombatAttack->Draw(-app->render->camera.x, -app->render->camera.y);
		btnCombatSkills->Draw(-app->render->camera.x, -app->render->camera.y);
		btnCombatItems->Draw(-app->render->camera.x, -app->render->camera.y);
		btnCombatSpecial->Draw(-app->render->camera.x, -app->render->camera.y);
		btnCombatFlee->Draw(-app->render->camera.x, -app->render->camera.y);

		if ((combatMenuFlags & 1 << Flags::SKILL) != 0)
		{
			app->render->DrawTexture(combatGui, -app->render->camera.x + 36, -app->render->camera.y + app->render->camera.h - combatMenuBox.h - 25, false, &combatMenuBox);
			btnCombatSkill1->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatSkill2->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatSkill3->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatSkill4->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatSkill5->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatSkill6->Draw(-app->render->camera.x, -app->render->camera.y);
		}
		else if ((combatMenuFlags & 1 << Flags::ITEMS) != 0)
		{
			app->render->DrawTexture(combatGui, -app->render->camera.x + 36, -app->render->camera.y + app->render->camera.h - combatMenuBox.h - 25, false, &combatMenuBox);
			btnCombatItem1->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatItem2->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatItem3->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatItem4->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatItem5->Draw(-app->render->camera.x, -app->render->camera.y);
			btnCombatItem6->Draw(-app->render->camera.x, -app->render->camera.y);

		}
		else if ((combatMenuFlags & 1 << Flags::SPECIAL) != 0)
		{
			app->render->DrawTexture(combatGui, -app->render->camera.x + 36, -app->render->camera.y + app->render->camera.h - combatMenuBox.h - 25, false, &combatMenuBox);
		}
	}

	if (targetAttack) //add more as we go
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
	attackSelected = -1;
	combatMenuFlags = 0;
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
			if (e->data->collider->Intersects({ x, y, 1, 1 }) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
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
		if (a->data->turns > 0)
		{
			a->data->turns--;
			if (a->data->turns == 0)
			{
				if (a->data->attackName == "10% debuff")
				{
					currentChar->character->stats.pDef += a->data->stat1 / 10;
					currentChar->character->stats.mDef += a->data->stat2 / 10;
				}
				else if (a->data->attackName == "Getting stronger")
				{
					currentChar->character->stats.pDef -= (a->data->stat1 * 5) / 100;
					currentChar->character->stats.mDef -= (a->data->stat2 * 5) / 100;
				}
				else if (a->data->attackName == "Nightmarish")
				{
					currentChar->character->stats.pDef -= a->data->stat1 / 10;
					currentChar->character->stats.mDef -= a->data->stat2 / 10;
				}
			}
		}
		if (a->next != nullptr) a = a->next;
		else break;
	}
}

void SceneCombat::Damage(int index, CombatEntity* target, bool isMagic)
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
		currentEntity->data->CalculatePrecision(currentEntity->data->attackPool.At(index)->data->stat1);
		//dialogue that X character does Y attack to Z enemy
		LOG("%s does damage attack to %s!", currentEntity->data->name.GetString(), target->name.GetString());
		char tmp[75];
		sprintf(tmp, "%s does damage attack to %s!", currentEntity->data->name.GetString(), target->name.GetString());
		NextLine(tmp);
		int attack = 0;
		if (!isMagic)
			attack = (currentEntity->data->attackPool.At(index)->data->stat1 - target->stats.pDef);
		else
			attack = (currentEntity->data->attackPool.At(index)->data->stat1 - target->stats.mDef);

		if (attack <= 0)
			attack = 0;
		target->stats.hPoints -= attack;
		if (target->stats.hPoints <= 0)
			target->stats.hPoints = 0;
	}
	else
	{
		if (!IsCharacter(target))
		{
			currentEntity->data->CalculatePrecision(currentEntity->data->attackPool.At(index)->data->stat1);
			//dialogue that X character does Y attack to Z enemy
			LOG("%s does damage attack to %s!", currentEntity->data->name.GetString(), target->name.GetString());
			LOG("%s does damage attack to %s!", currentEntity->data->name.GetString(), target->name.GetString());
			char tmp[75];
			sprintf(tmp, "%s does damage attack to %s!", currentEntity->data->name.GetString(), target->name.GetString());
			int attack = 0;
			if (!isMagic)
				attack = (currentEntity->data->attackPool.At(index)->data->stat1 - target->stats.pDef);
			else
				attack = (currentEntity->data->attackPool.At(index)->data->stat1 - target->stats.mDef);

			if (attack <= 0)
				attack = 0;
			target->stats.hPoints -= attack;
			if (target->stats.hPoints <= 0)
				target->stats.hPoints = 0;
		}
		else
		{
			LOG("You're god, no damage taken!");
			NextLine("God Mode active. No damage recieved.");
		}
	}
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
	}
}

int SceneCombat::EnemyTarget()
{
	int c = 0;
	if (mainChar.character != nullptr) c++;
	if (grandpa.character != nullptr) c++;
	return  rand() % c + 1;
}


void SceneCombat::SpawnEnemies(EntityId id1, EntityId id2, EntityId id3)
{
	if (id1 != EntityId::NOT_COMBAT || id2 != EntityId::NOT_COMBAT || id3 != EntityId::NOT_COMBAT)
	{
		int c = 0;
		if (id1 != EntityId::NOT_COMBAT) c++;
		if (id2 != EntityId::NOT_COMBAT) c++;
		if (id3 != EntityId::NOT_COMBAT) c++;


		srand(time(NULL));
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
		srand(time(NULL));
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
	case 29: //ATTACK
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		attackSelected = 0;
		LOG("Who do you want to attack?");
		NextLine("Who do you want to attack?");
		break;
	case 30: //SKILLS
		app->gui->ResetButtons();
		btnCombatSkills->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::SKILL);
		changeMenu = true;
		break;
	case 31: //ITEMS
		app->gui->ResetButtons();
		btnCombatItems->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::ITEMS);
		changeMenu = true;
		break;
	case 32: //SPECIAL
		app->gui->ResetButtons();
		btnCombatSpecial->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::SPECIAL);
		changeMenu = true;
		break;
	case 33: //FLEE
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
	case 34: //SKILL 1
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		//attackSelected = 1;
		finishedAction = true; //Reminder de quitarlo cuando tengamos skills
		break;
	case 35: //SKILL 2
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		//attackSelected = 2;
		finishedAction = true; //Reminder de quitarlo cuando tengamos skills
		break;
	case 36: //SKILL 3
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		//attackSelected = 3;
		finishedAction = true; //Reminder de quitarlo cuando tengamos skills
		break;
	case 37: //SKILL 4
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		//attackSelected = 4;
		finishedAction = true; //Reminder de quitarlo cuando tengamos skills
		break;
	case 38: //SKILL 5
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		//attackSelected = 5;
		finishedAction = true; //Reminder de quitarlo cuando tengamos skills
		break;
	case 39: //SKILL 6
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		//attackSelected = 6;
		finishedAction = true; //Reminder de quitarlo cuando tengamos skills
		break;
	case 40: //ITEM 1
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		finishedAction = true;
		LOG("Item 1");
		break;
	case 41: //ITEM 2
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		finishedAction = true;
		LOG("Item 2");
		break;
	case 42: //ITEM 3
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		finishedAction = true;
		LOG("Item 3");
		break;
	case 43: //ITEM 4
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		finishedAction = true;
		LOG("Item 4");
		break;
	case 44: //ITEM 5
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		finishedAction = true;
		LOG("Item 5");
		break;
	case 45: //ITEM 6
		app->gui->ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		finishedAction = true;
		LOG("Item 6");
		break;
	default:
		break;
	}

	return true;
}