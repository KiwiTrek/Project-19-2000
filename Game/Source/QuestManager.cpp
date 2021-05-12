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
	bookTex = app->tex->Load("Textures/scrollQuest.png");
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
	CheckObjectivesCompletion();
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
	questsList.Clear();
	questsInactive.Clear();
	questsActive.Clear();
	questsFinished.Clear();
	
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
	/*

	Quest 7:
		Appear: Tp to bedroom
		Complete: Opened diary bool
	Quest 8: (interact with residents in the living room)
		Appear: Complete quest 7
		Complete: interaction count == 3
	Quest 9: (visit the shop)
		Appear: Complete quest 7
		Complete: Talk to the shopkeeper
	Quest 10: (visit the bathroom)
		Appear: Complete quest 7
		Complete: Tp to bathroom
	Quest 11: (visit the kitchen)
		Appear: Complete quest 7
		Complete: Tp to kitchen
	*/

	// Quest 1:
	if ((tpFlags & 1 << TpFlags::GRANDPA) != 0)
		AppearQuest(1);
	if (((app->entities->flagsShopkeeper & 1 << (int)DialogueFlags::FINISHED_TALK_REQUEST) != 0) && ((tpFlags & 1 << TpFlags::SMALL_PUZZLE) != 0))
		CompleteQuest(1);

	// Quest 2:
	// It appears when completing quest 1
	if (/* Finsh puzzle condition && */((tpFlags & 1 << TpFlags::FIGHT) != 0))
		CompleteQuest(2);

	// Quest 3:
	// It appears when completing quest 2
	if (/* Finish first fight condition && */((tpFlags & 1 << TpFlags::BIG_PUZZLE) != 0))
		CompleteQuest(3);

	// Quest 4:
	// It appears when completing quest 3
	if (/* Finish big puzzle condition && */((tpFlags & 1 << TpFlags::LABYRINTH) != 0))
		CompleteQuest(4);

	// Quest 5:
	// It appears when completing quest 4
	if (/* Labyrinth sensor achieved */ 1)
		CompleteQuest(5);

	// Quest 6:
	if ((tpFlags & 1 << TpFlags::BOSS_FIGHT) != 0)
		AppearQuest(6);
	if (/* Finish boss fight condition */ 1)
		CompleteQuest(6);

	// Quest 7:
	if ((tpFlags & 1 << TpFlags::BEDROOM) != 0)
		AppearQuest(7);
	if (/* Opened diary condition */ 1)
		CompleteQuest(7);


	return true;
}

bool QuestManager::CheckObjectivesCompletion()
{
	// This is the Objectives Completion Conditions. For each quest, an if with the conditions with a call -->
	// --> to the function CompleteQuest(id)

	//if (completion condition of quest with id == is completed (app->player->mushroomCount == 8))
	//	CompleteQuest(id of the quest completed);

	return true;
}

// This debug function complete the first active quest in the list by pressing F7
bool QuestManager::DebugQuests()
{
	ListItem<Quest*>* activeList = questsActive.start;
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		CompleteQuest(activeList->data->id);

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