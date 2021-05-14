#include "QuestManager.h"

QuestManager::QuestManager() : Module()
{
	name.Create("quests");
	active = false;
}

QuestManager::~QuestManager()
{
}

bool QuestManager::Start()
{

	font = new Font("Fonts/DialogueFont.xml");
	bookTex = app->tex->Load("Textures/ScrollQuest.png");
	bookBox = { 0,0,1280,720 };

	pugi::xml_node questNode;
	pugi::xml_document questData;
	pugi::xml_parse_result parseResult = questData.load_file("quests.xml");
	if (parseResult == NULL)
		LOG("Could not load xml file <quests.xml> pugi error: %s", parseResult.description());
	else
		questNode = questData.child("quests");
	if (questNode == NULL)
		LOG("Could not load <questsData> xml_document");

	questNode = questNode.child("quest");
	while (questNode != NULL)
	{
		Quest* quest = new Quest();

		quest->id = questNode.attribute("id").as_int();
		//quest->type = questNode.attribute("type").as_int();
		quest->title = questNode.attribute("title").as_string();
		quest->description = questNode.attribute("description").as_string();
		//quest->objective = questNode.attribute("objective").as_string();
		//quest->quantity = questNode.attribute("quantity").as_int();
		//quest->demandingNPC = questNode.attribute("demandingNPC").as_string();
		//quest->rewardingNPC = questNode.attribute("rewardingNPC").as_string();
		quest->rewardXP = questNode.attribute("rewardXP").as_int();
		quest->rewardGold = questNode.attribute("rewardGold").as_int();
		quest->requiredIdString = questNode.attribute("requiredId").as_string();
		quest->isCompleted = questNode.attribute("isCompleted").as_bool();
		quest->status = questNode.attribute("status").as_int();

		// This serves to sort out the list with the quests status
		if (quest->status == 0) // status 0 goes to inactive list
		{
			questsInactive.Add(quest);
		}
		else if (quest->status == 1) // status 1 goes to active list 
		{
			questsActive.Add(quest);
		}
		else if (quest->status == 2) // status 2 goes to finished list
		{
			questsFinished.Add(quest);
		}

		// List with all quests
		questsList.Add(quest);

		totalQuests += 1;

		StringToIntArray(quest, quest->requiredIdString);

		questNode = questNode.next_sibling("quest");
	}

	tpFlags = 0;

	return true;
}

bool QuestManager::Update(float dt)
{
	FinishedQuestLogic();
	ChainQuestsLogic();
	CheckQuestsLogic();
	DebugQuests();

	return true;
}

bool QuestManager::PostUpdate()
{
	DrawActiveQuests();
	return true;
}

bool QuestManager::CleanUp()
{
	// Destroy && Clean quests
	ListItem<Quest*>* questL = questsList.start;
	while (questL != nullptr)
	{
		questsList.Del(questL);
		questL = questL->next;
	}
	questsList.Clear();

	ListItem<Quest*>* inactiveL = questsInactive.start;
	while (inactiveL != nullptr)
	{
		questsInactive.Del(inactiveL);
		inactiveL = inactiveL->next;
	}
	questsInactive.Clear();

	ListItem<Quest*>* activeL = questsActive.start;
	while (activeL != nullptr)
	{
		questsActive.Del(activeL);
		activeL = activeL->next;
	}
	questsActive.Clear();

	ListItem<Quest*>* finishL = questsFinished.start;
	while (finishL != nullptr)
	{
		questsFinished.Del(finishL);
		finishL = finishL->next;
	}
	questsFinished.Clear();

	// Destroy && Release Textures and Fonts
	RELEASE(font);
	app->tex->UnLoad(bookTex);

	return true;
}

// Passes a completed quest from the active list to the finished list and gives rewards
bool QuestManager::FinishedQuestLogic()
{
	ListItem<Quest*>* activeQuestsList = questsActive.start;
	while (activeQuestsList != nullptr)
	{
		if (activeQuestsList->data->isCompleted == true)
		{
			activeQuestsList->data->status = 2;
			questsActive.Del(activeQuestsList);
			questsFinished.Add(activeQuestsList->data);
			//app->player->RewardXP(activeQuestsList->data->rewardXP);
			//app->player->RewardGold(activeQuestsList->data->rewardGold);
		}

		activeQuestsList = activeQuestsList->next;
	}

	return true;
}

// Chain quests logic (now works for multiple quest requirements)
bool QuestManager::ChainQuestsLogic()
{
	ListItem<Quest*>* inactiveQuestsList = questsInactive.start;
	while (inactiveQuestsList != NULL)
	{
		int requiredIdCount = CountRequiredIds(inactiveQuestsList->data->requiredId);
		int count = 0;
		if (requiredIdCount != 0)
		{

			for (int i = 0; (/*inactiveQuestsList->data->requiredId[i] != 0 ||*/ i < 4); ++i)
			{
				ListItem<Quest*>* finishedList = questsFinished.start;
				while (finishedList != NULL)
				{
					if ((inactiveQuestsList->data->requiredId[i] == finishedList->data->id) /*&& (inactiveQuestsList->data->requiredId[i] != 0)*/)
					{
						count++;
					}
					finishedList = finishedList->next;
				}
			}

			if (count == requiredIdCount)
			{
				questsActive.Add(inactiveQuestsList->data);
				if (questsInactive.At(questsInactive.Find(inactiveQuestsList->data)) != nullptr) questsInactive.Del(inactiveQuestsList);
				inactiveQuestsList->data->status = 1;
			}
		}
		inactiveQuestsList = inactiveQuestsList->next;
	}

	return true;
}

// This function manages the logic for every quest. For appearing and completion
bool QuestManager::CheckQuestsLogic()
{
	// Quest 1: Grandpa talk
	if ((tpFlags & TpFlags::GRANDPA) != 0)
		AppearQuest(1);
	if (/*(app->entities->flagsGrandpa & 1 << (int)DialogueFlags::FINISHED_TALK) == 0 && ((tpFlags & TpFlags::GRANDPA) != 0)*/((tpFlags & TpFlags::SMALL_PUZZLE) != 0)) // Delete tpFlags of SMALL_PUZZLE when grandpa implemented
		CompleteQuest(1);

	// Quest 2: Small puzzle
	if (((tpFlags & TpFlags::SMALL_PUZZLE) != 0))
		AppearQuest(2);
	if (/* Finsh small puzzle condition && */((tpFlags & TpFlags::FIGHT) != 0))
		CompleteQuest(2);

	// Quest 3: Fight
	// It appears when completing quest 2
	if (firstCombatWon)
		CompleteQuest(3);

	// Quest 4: Big puzzle
	if (((tpFlags & TpFlags::BIG_PUZZLE) != 0))
		AppearQuest(4);
	if (/* Finish big puzzle condition && */((tpFlags & TpFlags::LABYRINTH) != 0))
		CompleteQuest(4);

	// Quest 5: Labyrinth
	// It appears when completing quest 4
	if (escapedLabyrinth)
		CompleteQuest(5);

	// Quest 6: Boss fight
	if (escapedLabyrinth && ((tpFlags & TpFlags::BOSS_FIGHT) != 0))
		AppearQuest(6);
	if (firstBossDefeated)
		CompleteQuest(6);

	// Quest 7: Diary
	if (((tpFlags & TpFlags::BEDROOM) != 0))
		AppearQuest(7);
	if (/* Opened diary condition */ 0)
		CompleteQuest(7);

	// Quest 8: Shop visit
	// It appears when completing quest 7
	if (((tpFlags & TpFlags::SHOP) != 0))
		CompleteQuest(8);

	// Quest 9: Save game
	// It appears when completing quest 7
	if (app->saveRequest == true)
		CompleteQuest(9);

	// Quest 10: Talk to residents
	// It appears when completing quest 7
	if (/* has talked to cat && has talked to strange man*/ 0)
		CompleteQuest(10);

	// Quest 11: Talk to grandpa
	// It appears when completing quest 8, 9, 10
	if (/* has talked to grandpa && ((tpFlags & TpFlags::MAIN_LOBBY) != 0)*/ 0) // Differentiate both has talked to grandpa. Maybe with flags
		CompleteQuest(11);

	return true;
}

// This debug function complete the first active quest in the list by pressing F7
bool QuestManager::DebugQuests()
{
	ListItem<Quest*>* activeList = questsActive.start;
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		if (activeList != nullptr) CompleteQuest(activeList->data->id);
	}


	return true;
}

// This is the drawing quest on UI function. Looks at the active quests list and draws them with the respective offsets
bool QuestManager::DrawActiveQuests()
{
	if (drawQuests)
	{
		// Draw back square
		app->render->DrawTexture(bookTex, -app->render->camera.x, -app->render->camera.y, false, &bookBox);

		int offsetY = 70;
		int offsetX = 45;
		SString auxDescription;
		const char* cutText = "...";
		ListItem<Quest*>* activeQuestList = questsActive.start;
		while (activeQuestList != NULL)
		{
			// Cutting description text
			auxDescription = activeQuestList->data->description.GetString();
			if (activeQuestList->data->description.Length() >= 45)
			{
				auxDescription.Cut(45);
				auxDescription += cutText;
			}

			// Draw title
			app->render->DrawText(font, activeQuestList->data->title.GetString(), offsetX, offsetY, 35, 2, { 0,0,0,255 });
			offsetY += 37; // Offset the description from the title
			offsetX += 15; // Offset the description from the title
			app->render->DrawText(font, auxDescription.GetString(), offsetX, offsetY, 22, 2, { 80,80,80,255 });
			offsetY += 40; // Offset the quest from the last one
			offsetX -= 15; // Offset the descripton from the title

			activeQuestList = activeQuestList->next;
		}
	}
	return true;
}

// This function looks at the complete quest list and looks if the id passed as an argument matches -->
// --> with the quest id. If true puts that quest boolean isCompleted = true
bool QuestManager::CompleteQuest(int id)
{
	ListItem<Quest*>* L = questsList.start;
	while (L != nullptr)
	{
		if (id == L->data->id)
		{
			L->data->isCompleted = true;
			return true;
		}
		L = L->next;
	}
	return true;
}

// This function looks at the complete quest list and looks if the id passed as an argument matches -->
// --> with the quest id. If true puts that quest boolean isCompleted = true
bool QuestManager::AppearQuest(int id)
{
	ListItem<Quest*>* inactiveL = questsInactive.start;
	while (inactiveL != nullptr)
	{
		if (id == inactiveL->data->id)
		{
			inactiveL->data->status = 1;
			questsActive.Add(inactiveL->data);
			if (questsInactive.At(questsInactive.Find(inactiveL->data)) != nullptr) questsInactive.Del(inactiveL);
		}

		inactiveL = inactiveL->next;
	}
	return true;
}

// This function checks with the id passed if the quest is completed or not
bool QuestManager::CheckQuestCompleted(int id)
{
	ListItem<Quest*>* finishedL = questsFinished.start;
	while (finishedL != nullptr)
	{
		if (finishedL->data->id == id)
			return true;

		finishedL = finishedL->next;
	}
	return false;
}

// This function converts the xml string data "requiredIdString", and modifies the Quest data "required Id", that it's an int array
void QuestManager::StringToIntArray(Quest* quest, string requiredIdString)
{
	int stringLength = quest->requiredIdString.length();
	int j = 0;
	for (int i = 0; quest->requiredIdString[i] != '\0'; ++i)
	{
		if (quest->requiredIdString[i] == ',')
			continue;
		if (quest->requiredIdString[i] == ' ')
			j++;
		else
		{
			quest->requiredId[j] = quest->requiredId[j] * 10 + (quest->requiredIdString[i] - 48);
			if (j != 0)
			{
				if (quest->requiredIdString[i + 1] != ',' && quest->requiredIdString[i + 1] != ' ');
				else
					j++;
			}
			else
				j++;
			
		}
	}
}

// This function counts how many numbers different to 0 are in an integer array. Serves for counting the total ids required for a quest
int QuestManager::CountRequiredIds(int requiredIds[])
{
	int count = 0;
	for (int i = 0; requiredIds[i] != NULL; ++i)
	{
		if (requiredIds[i] != 0)
			count++;
	}
	return count;
}

bool QuestManager::Save(pugi::xml_node& savegame)
{
	LOG("Saving quests data");
	bool ret = true;

	savegame.append_attribute("tpFlags").set_value(tpFlags);
	savegame.append_attribute("drawQuests").set_value(drawQuests);
	savegame.append_attribute("firstCombatWon").set_value(firstCombatWon);
	savegame.append_attribute("escapedLabyrinth").set_value(escapedLabyrinth);
	savegame.append_attribute("firstBossDefeated").set_value(firstBossDefeated);

	ListItem<Quest*>* totalQuestsL = questsList.start;
	while (totalQuestsL != nullptr)
	{
		pugi::xml_node quest = savegame.append_child("quest");
		quest.append_attribute("id").set_value(totalQuestsL->data->id);
		quest.append_attribute("title").set_value(totalQuestsL->data->title.GetString());
		quest.append_attribute("description").set_value(totalQuestsL->data->description.GetString());
		quest.append_attribute("rewardXP").set_value(totalQuestsL->data->rewardXP);
		quest.append_attribute("rewardGold").set_value(totalQuestsL->data->rewardGold);
		quest.append_attribute("requiredId").set_value(totalQuestsL->data->requiredIdString.c_str());
		quest.append_attribute("isCompleted").set_value(totalQuestsL->data->isCompleted);
		quest.append_attribute("status").set_value(totalQuestsL->data->status);

		totalQuestsL = totalQuestsL->next;
	}


	return ret;
}

bool QuestManager::Load(pugi::xml_node& savegame)
{
	LOG("Loading quests data");
	bool ret = true;

	// Destroy && Clean quests
	ListItem<Quest*>* questL = questsList.start;
	while (questL != nullptr)
	{
		questsList.Del(questL);
		questL = questL->next;
	}
	questsList.Clear();
	
	ListItem<Quest*>* inactiveL = questsInactive.start;
	while (inactiveL != nullptr)
	{
		questsInactive.Del(inactiveL);
		inactiveL = inactiveL->next;
	}
	questsInactive.Clear();

	ListItem<Quest*>* activeL = questsActive.start;
	while (activeL != nullptr)
	{
		questsActive.Del(activeL);
		activeL = activeL->next;
	}
	questsActive.Clear();

	ListItem<Quest*>* finishL = questsFinished.start;
	while (finishL != nullptr)
	{
		questsFinished.Del(finishL);
		finishL = finishL->next;
	}
	questsFinished.Clear();

	tpFlags = savegame.attribute("tpFlags").as_uint();
	drawQuests = savegame.attribute("drawQuests").as_bool();
	firstCombatWon = savegame.attribute("firstCombatWon").as_bool();
	escapedLabyrinth = savegame.attribute("escapedLabyrinth").as_bool();
	firstBossDefeated = savegame.attribute("firstBossDefeated").as_bool();

	pugi::xml_node questNode = savegame.child("quest");
	while (questNode != NULL)
	{
		Quest* quest = new Quest();
		
		quest->id = questNode.attribute("id").as_int();
		//quest->type = questNode.attribute("type").as_int();
		quest->title = questNode.attribute("title").as_string();
		quest->description = questNode.attribute("description").as_string();
		//quest->objective = questNode.attribute("objective").as_string();
		//quest->quantity = questNode.attribute("quantity").as_int();
		//quest->demandingNPC = questNode.attribute("demandingNPC").as_string();
		//quest->rewardingNPC = questNode.attribute("rewardingNPC").as_string();
		quest->rewardXP = questNode.attribute("rewardXP").as_int();
		quest->rewardGold = questNode.attribute("rewardGold").as_int();
		quest->requiredIdString = questNode.attribute("requiredId").as_string();
		quest->isCompleted = questNode.attribute("isCompleted").as_bool();
		quest->status = questNode.attribute("status").as_int();

		// This serves to sort out the list with the quests status
		if (quest->status == 0) // status 0 goes to inactive list
		{
			questsInactive.Add(quest);
		}
		else if (quest->status == 1) // status 1 goes to active list 
		{
			questsActive.Add(quest);
		}
		else if (quest->status == 2) // status 2 goes to finished list
		{
			questsFinished.Add(quest);
		}

		// List with all quests
		questsList.Add(quest);

		StringToIntArray(quest, quest->requiredIdString);

		questNode = questNode.next_sibling("quest");
	}


	return ret;
}