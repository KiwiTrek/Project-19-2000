#include "SceneCombat.h"

#include "Audio.h"
#include "EntityManager.h"
#include "CombatEntity.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"

SceneCombat::SceneCombat()
{
	float tmpValue = 0;

	//COMBAT
	combatTextBox = { 0,0,1280,248 };
	combatMenuBox = { 305,249,1001,130 };
	btnCombatAttack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 29, { 34,505,200,60 }, "ATTACK", 40, this);
	btnCombatSkills = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 30, { 234,505,200,60 }, "SKILLS", 40, this);
	btnCombatItems = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 31, { 434,505,200,60 }, "ITEMS", 40, this);
	btnCombatSpecial = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 32, { 634,505,200,60 }, "SPECIAL", 40, this);
	btnCombatFlee = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 33, { 834,505,200,60 }, "FLEE", 40, this);

	ListItem<CombatEntity*>* e = turnOrder.start;
	while (e != nullptr)
	{
		ListItem<CombatEntity*>* eNext = e->next;
		int i = turnOrder.Find(e->data);
		delete turnOrder[i];
		turnOrder.Del(turnOrder.At(i));
		e = eNext;
	}
	turnOrder.Clear();

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

	targetAttack = false;
	finishedAction = false;
	combatState = CombatStateType::COMBAT_START;
	attackSelected = -1;
	combatMenuFlags = 0;

	currentChar = &mainChar;
	mainChar.box = { 1280,0,204,190 };
	mainChar.characterTex = { 0,252,72,92 };
	mainChar.character = app->entities->CreateEntity(36, app->render->camera.h - mainChar.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::MC, Stats(20, 0, 10, 10, 100, 100, 50, 100, 100, 0, 100));
	mainChar.hp.Create("HP: %d/%d", mainChar.character->stats.hPoints, mainChar.character->stats.hPointsMax);
	mainChar.mp.Create("MP: %d/%d", mainChar.character->stats.mPoints, mainChar.character->stats.mPointsMax);
	mainChar.stress.Create("ST: %d/%d", mainChar.character->stats.stress, mainChar.character->stats.stressMax);

	grandpa.box = { 1280,0,204,190 };
	grandpa.characterTex = { 73,252,68,100 };
	grandpa.character = app->entities->CreateEntity(grandpa.box.w + 36, app->render->camera.h - grandpa.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::VIOLENT, Stats(25, 25, 0, 0, 100, 100, 25, 100, 100));
	grandpa.hp.Create("HP: %d/%d", grandpa.character->stats.hPoints, grandpa.character->stats.hPointsMax);
	grandpa.mp.Create("MP: %d/%d", grandpa.character->stats.mPoints, grandpa.character->stats.mPointsMax);

	enemy1 = app->entities->CreateEntity(300, 300, EntityType::COMBAT_ENTITY, EntityId::STRESSING_SHADOW, Stats(0, 10, 10, 10, 40, 40, 60));
	enemy2 = app->entities->CreateEntity(700, 300, EntityType::COMBAT_ENTITY, EntityId::FURIOUS_SHADOW, Stats(15, 0, 20, 5, 60, 60, 90));

	LOG("%d", characterFlags);

	return false;
}

bool SceneCombat::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) characterSelected = !characterSelected;

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
		}
		else
		{
			if (IsCharacter(currentEntity->data)) // character
			{
				//WAIT FOR PLAYER INPUT
				characterSelected = true;						//should delete eventually

				//player should decide what to do here based on the buttons (guiclickevent)
				//LOG("%s's turn!\n", currentEntity->data->name.GetString());

				switch (currentEntity->data->id)
				{
				case EntityId::MC:
					currentChar = &mainChar;

					if (attackSelected == -1)
					{
					}
					else if (currentEntity->data->AttackPool.At(attackSelected)->data->target == TargetType::ONE)
					{
						targetAttack = true;
						if (target == nullptr)
							SelectTarget();

						if (target != nullptr)
						{
							currentEntity->data->CalculatePrecision(currentEntity->data->AttackPool.At(0)->data->stat1);
							//dialogue that X character does Y attack to Z enemy
							LOG("damage attack to %s!\n", target->name.GetString());
							target->stats.hPoints -= currentEntity->data->AttackPool.At(0)->data->stat1;
							target = nullptr;
							finishedAction = true;
						}
					}
					else
					{
						switch (attackSelected)
						{
							//case 0: //attack
							//	break;
						case 1: //skill 1
							break;
						case 2: //skill 2
							break;
						default:
							break;
						}
					}
					break;
				case EntityId::VIOLENT:
					currentChar = &grandpa;

					if (attackSelected == -1)
					{
					}
					else if (currentEntity->data->AttackPool.At(attackSelected)->data->target == TargetType::ONE)
					{
						targetAttack = true;
						if (target == nullptr)
							SelectTarget();

						if (target != nullptr)
						{
							currentEntity->data->CalculatePrecision(currentEntity->data->AttackPool.At(0)->data->stat1);
							//dialogue that X character does Y attack to Z enemy
							LOG("damage attack to %s!\n", target->name.GetString());
							target->stats.hPoints -= currentEntity->data->AttackPool.At(0)->data->stat1;
							target = nullptr;
							finishedAction = true;
						}
					}
					else
					{
						switch (attackSelected)
						{
							//case 0: //attack
							//	break;
						case 1: //skill 1
							break;
						case 2: //skill 2
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
			else // enemy
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
						LOG("stress attack!\n");
						mainChar.character->stats.stress += 10;
					}
					else //Magical blow
					{
						int pTarget = rand() % 2 + 1;
						if (pTarget == 1) //MC
						{
							currentEntity->data->CalculatePrecision(currentEntity->data->AttackPool.At(0)->data->stat1);
							//dialogue that X enemy does Y attack to Z character
							LOG("damage attack to mc!\n");
							mainChar.character->stats.hPoints -= currentEntity->data->AttackPool.At(0)->data->stat1;
						}
						else if (pTarget == 2) //GRANDPA
						{
							currentEntity->data->CalculatePrecision(currentEntity->data->AttackPool.At(0)->data->stat1);
							//dialogue that X enemy does Y attack to Z character
							LOG("damage attack to grandpa!\n");
							grandpa.character->stats.hPoints -= currentEntity->data->AttackPool.At(0)->data->stat1;
						}
					}
					finishedAction = true;
					break;
				}
				case EntityId::FURIOUS_SHADOW:
				{
					LOG("NOTHING!\n");
					finishedAction = true;
					break;
				}
				default:
					break;
				}
			}

			if (finishedAction)
			{
				finishedAction = false;
				currentEntity = currentEntity->next;
			}
		}

		ListItem<CombatEntity*>* e = turnOrder.start;
		int counter = 0;
		while (e != nullptr)
		{
			if (IsCharacter(e->data) == false)
				counter++;

			e = e->next;
		}

		if (counter == 0) combatState = CombatStateType::COMBAT_END;
	}
	break;
	case COMBAT_END:
		// cool animation as a victory thing and change back to gameplay
		combat = false;
		break;
	default:
		break;
	}

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

bool SceneCombat::Draw(Font* dialogueFont)
{
	//switch (ZONE)
	app->render->background = { 125,33,129,255 };
	app->render->DrawTexture(combatGui, -app->render->camera.x, -app->render->camera.y, false, &combatTextBox);
	app->render->DrawTexture(combatGui, -app->render->camera.x, -app->render->camera.y + app->render->camera.h - combatTextBox.h, false, &combatTextBox);
	//app->render->DrawRectangle({ 1280 / 2 - 64,720 / 2 - 64,128,128 }, 0, 255, 255, 255);


	if (!characterSelected)
	{
		if (characterFlags >= 1)
		{
			app->render->DrawTexture(combatGui, -app->render->camera.x + mainChar.character->entityRect.x, -app->render->camera.y + mainChar.character->entityRect.y, false, &mainChar.box);
			app->render->DrawTexture(combatGui, -app->render->camera.x + mainChar.character->entityRect.x + 10, -app->render->camera.y + mainChar.character->entityRect.y + (mainChar.box.h / 2 - mainChar.characterTex.h / 2), false, &mainChar.characterTex);
			app->render->DrawText(dialogueFont, mainChar.hp.GetString(), -app->render->camera.x + mainChar.character->entityRect.x + mainChar.characterTex.w + 15, -app->render->camera.y + mainChar.character->entityRect.y + 45, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, mainChar.mp.GetString(), -app->render->camera.x + mainChar.character->entityRect.x + mainChar.characterTex.w + 15, -app->render->camera.y + mainChar.character->entityRect.y + 45 + 30, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, mainChar.stress.GetString(), -app->render->camera.x + mainChar.character->entityRect.x + mainChar.characterTex.w + 15, -app->render->camera.y + mainChar.character->entityRect.y + 45 + 60, 28, 1, { 255,255,255,255 });
		}
		if (characterFlags >= 3)
		{
			app->render->DrawTexture(combatGui, -app->render->camera.x + grandpa.character->entityRect.x, -app->render->camera.y + grandpa.character->entityRect.y, false, &grandpa.box);
			app->render->DrawTexture(combatGui, -app->render->camera.x + grandpa.character->entityRect.x + 10, -app->render->camera.y + grandpa.character->entityRect.y + (grandpa.box.h / 2 - grandpa.characterTex.h / 2), false, &grandpa.characterTex);
			app->render->DrawText(dialogueFont, grandpa.hp.GetString(), -app->render->camera.x + grandpa.character->entityRect.x + grandpa.characterTex.w + 15, -app->render->camera.y + grandpa.character->entityRect.y + 50, 28, 1, { 255,255,255,255 });
			app->render->DrawText(dialogueFont, grandpa.mp.GetString(), -app->render->camera.x + grandpa.character->entityRect.x + grandpa.characterTex.w + 15, -app->render->camera.y + grandpa.character->entityRect.y + 50 + 30, 28, 1, { 255,255,255,255 });
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
		app->render->DrawTexture(combatGui, -app->render->camera.x + app->render->camera.w - currentChar->box.w - 34, -app->render->camera.y + app->render->camera.h - currentChar->box.h - 25, false, &currentChar->box);
		app->render->DrawTexture(combatGui, -app->render->camera.x + app->render->camera.w - currentChar->box.w - 24, -app->render->camera.y + app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4), false, &currentChar->characterTex);
		app->render->DrawText(dialogueFont, currentChar->hp.GetString(), -app->render->camera.x + app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w, -app->render->camera.y + app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4), 28, 1, { 255,255,255,255 });
		app->render->DrawText(dialogueFont, currentChar->mp.GetString(), -app->render->camera.x + app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w, -app->render->camera.y + app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4) + 30, 28, 1, { 255,255,255,255 });
		if (currentChar->character->id == EntityId::MC) app->render->DrawText(dialogueFont, currentChar->stress.GetString(), -app->render->camera.x + app->render->camera.w - currentChar->box.w - 24 + currentChar->characterTex.w, -app->render->camera.y + app->render->camera.h - currentChar->box.h + (currentChar->characterTex.h / 4) + 60, 28, 1, { 255,255,255,255 });

		btnCombatAttack->Draw(-app->render->camera.x, -app->render->camera.y);
		btnCombatSkills->Draw(-app->render->camera.x, -app->render->camera.y);
		btnCombatItems->Draw(-app->render->camera.x, -app->render->camera.y);
		btnCombatSpecial->Draw(-app->render->camera.x, -app->render->camera.y);
		btnCombatFlee->Draw(-app->render->camera.x, -app->render->camera.y);

		if ((combatMenuFlags & 1 << Flags::SKILL) != 0)
		{
			app->render->DrawTexture(combatGui, -app->render->camera.x + 36, -app->render->camera.y + app->render->camera.h - combatMenuBox.h - 25, false, &combatMenuBox);
		}
		else if ((combatMenuFlags & 1 << Flags::ITEMS) != 0)
		{
			app->render->DrawTexture(combatGui, -app->render->camera.x + 36, -app->render->camera.y + app->render->camera.h - combatMenuBox.h - 25, false, &combatMenuBox);
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
			{
				app->render->DrawRectangle(e->data->collider->rect, 255, 255, 255, 150, false);
			}
			e = e->next;
		}
	}

	return true;
}

bool SceneCombat::Unload()
{
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

void SceneCombat::ResetButtons()
{
	btnCombatAttack->state = GuiControlState::NORMAL;
	btnCombatSkills->state = GuiControlState::NORMAL;
	btnCombatItems->state = GuiControlState::NORMAL;
	btnCombatSpecial->state = GuiControlState::NORMAL;
	btnCombatFlee->state = GuiControlState::NORMAL;
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
		ResetButtons();
		combatMenuFlags = 0;
		characterSelected = false;
		attackSelected = 0;
		LOG("Who do you want to attack?");
		break;
	case 30: //SKILLS
		ResetButtons();
		btnCombatSkills->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::SKILL);
		break;
	case 31: //ITEMS
		ResetButtons();
		btnCombatItems->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::ITEMS);
		break;
	case 32: //SPECIAL
		ResetButtons();
		btnCombatSpecial->state = GuiControlState::DISABLED;
		combatMenuFlags = 0;
		combatMenuFlags = SetBit(combatMenuFlags, Flags::SPECIAL);
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