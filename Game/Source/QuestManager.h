#ifndef __QUEST_MANAGER_H__
#define __QUEST_MANAGER_H__

#include "Module.h"
#include "List.h"
#include "SString.h"
#include "App.h"
#include "Log.h"
#include "Input.h"
#include "Render.h"
#include "Font.h"
#include "Textures.h"
#include "Defs.h"
#include "Log.h"
#include "EntityManager.h"

#include "External/PugiXml/src/pugixml.hpp"
#include "SDL/include/SDL_scancode.h"

#include <string>
#include <iostream>
using namespace std;

class Font;

class Quest
{
public:
	Quest() {};
	~Quest() {};
public:
	int id; // Quest ID (unique for each quest)
	//int type; // Quest type: 0 --> gather 1 --> kill 2 --> delivery 3 --> dialogue
	SString title; // Quest title
	SString description; // Quest description
	//SString objective; // Object gathered or monster killer or NPC talked
	//int quantity; // Amount of objective required for the quest
	//SString demandingNPC; // Name of NPC quest giver
	//SString rewardingNPC; // Name of rewarding NPC
	int rewardXP; // Quantity of XP rewarded
	int rewardGold; // Quantity of Gold rewarded
	string requiredIdString; // For a quest chain if it requires another quest ID completed before
	int requiredId[4] = { 0 };
	bool isCompleted; // true --> Quest completed // false --> Quest not completed
	int status; // 0 inactive, 1 active, 2 completed
};

class QuestManager : public Module
{
public:
	QuestManager();

	~QuestManager();

	// Called before the first frame
	void Init();
	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool FinishedQuestLogic();
	bool ChainQuestsLogic();
	bool CheckQuestsLogic();
	bool DebugQuests();

	bool CompleteQuest(int id);
	bool AppearQuest(int id);
	bool CheckQuestCompleted(int id);

	bool DrawActiveQuests();

	void StringToIntArray(Quest* quest, string requiredIdString);
	int CountRequiredIds(int requiredIds[]);

	// Load/Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

public:
	enum TpFlags
	{
		GRANDPA = 1,
		SMALL_PUZZLE = 2,
		FIGHT = 4,
		BIG_PUZZLE = 8,
		LABYRINTH = 16,
		BOSS_FIGHT = 32,
		BEDROOM = 64,
		SHOP = 128,
		MAIN_LOBBY = 256,
		BATHROOM = 512,
		KITCHEN = 1024
	};
	enum BoolFlags
	{
		FIRSTCOMBAT = 1,
		ESCAPEDLABYRINTH = 2,
		FIRSTBOSS = 4,
		DIARY = 8
	};
	bool drawQuests = false;

	unsigned int tpFlags = 0;
	unsigned int boolFlags = 0;

private:
	int totalQuests = 0;

	List<Quest*> questsList;
	List<Quest*> questsInactive;
	List<Quest*> questsActive;
	List<Quest*> questsFinished;

	Font* font = nullptr;
	SDL_Texture *bookTex = nullptr;
    SDL_Rect bookBox = { 0,0,0,0 };

};


#endif