#include "QuestManager.h"
#include "Log.h"
#include "App.h"
#include "Module.h"
#include "List.h"
#include "Input.h"
#include "Render.h"
#include "Font.h"

#include "External/PugiXml/src/pugixml.hpp"
#include "SDL/include/SDL_scancode.h"



#include <string>
#include <iostream>
using namespace std;

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

	// font = new Font("Fonts/dungeon_font3.xml", app->tex);

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
		quest->type = questNode.attribute("type").as_int();
		quest->title = questNode.attribute("title").as_string();
		quest->description = questNode.attribute("description").as_string();
		quest->objective = questNode.attribute("objective").as_string();
		quest->quantity = questNode.attribute("quantity").as_int();
		quest->demandingNPC = questNode.attribute("demandingNPC").as_string();
		quest->rewardingNPC = questNode.attribute("rewardingNPC").as_string();
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
	return true;
}

bool QuestManager::Update(float dt)
{
	CheckQuestsLogic();
	CheckChainQuestsLogic();
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
bool QuestManager::CheckQuestsLogic()
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
bool QuestManager::CheckChainQuestsLogic()
{
	ListItem<Quest*>* inactiveQuestsList = questsInactive.start;
	while (inactiveQuestsList != NULL)
	{
		int requiredIdCount = CountRequiredIds(inactiveQuestsList->data->requiredId);
		int count = 0;
		for (int i = 0; (/*inactiveQuestsList->data->requiredId[i] != 0 ||*/ i < 4); ++i)
		{
			ListItem<Quest*>* finishedList = questsFinished.start;
			while (finishedList != NULL)
			{
				if ((inactiveQuestsList->data->requiredId[i] == finishedList->data->id) && (inactiveQuestsList->data->requiredId[i] != 0))
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

		inactiveQuestsList = inactiveQuestsList->next;
	}

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

// This is the drawing quest on UI function. Looks at the active quests list and hardcodes the position of the text
// Montu: I am sure theres an improvement on drawing quests. Something like drawing a list not -->
// --> hardcoding every single quest and position of the text blitted
bool QuestManager::DrawActiveQuests()
{
	string numToStr;
	const char* strToConstChar;
	ListItem<Quest*>* L = questsActive.start;
	while (L != NULL)
	{
		switch (L->data->id)
		{
		case 1: // new quest chain 1
			// Title Drawing
			// app->render->DrawText(font, L->data->title.GetString(), 0, 60, 60, 0, { 255,255,255,255 });

			// Amount of mushrooms taken
			//numToStr = to_string(app->player->mushroomCount); // to_string converts an int to string
			strToConstChar = numToStr.c_str(); // c_str converts a string to const char*
			// app->render->DrawText(font, strToConstChar, 280, 63, 60, 0, { 255,255,255,200 });

			// Description Drawing if pressed L
			if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				// app->render->DrawText(font, L->data->description.GetString(), 300, 70, 45, 0, { 200,200,200,155 });

				break;
		default:
			break;
		}

		L = L->next;
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
		}
		L = L->next;
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

// This function counts how many numeros different to 0 are in an array. Serves for counting the total ids required for a quest
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