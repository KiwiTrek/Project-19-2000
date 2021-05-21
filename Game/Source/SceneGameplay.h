#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "GuiImageButton.h"
#include "SString.h"

class Entity;
class SceneCombat;
class DialogSystem;
class PuzzlePieces;
class Blockers;

class SceneGameplay : public Scene
{
public:
	enum Flags
	{
		MENU,
		INVENTORY,
		SKILLS,
		SKILL_TREE,
		EQUIPMENT,
		STATS,
		OPTIONS,
		CONTROLS
	};

	SceneGameplay();
	virtual ~SceneGameplay();

	bool Load();

	bool Update(float dt);
	bool UpdateDialogue(float dt);
	bool UpdatePauseMenu(float dt);

	bool Draw();
	bool DrawPauseMenu();

	bool Unload();

	// Declare on mouse click event
	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	//COMBAT
	SceneCombat* combatScene;

	//NPCs
	Entity* hero = nullptr;
	Entity* cat = nullptr;
	Entity* shopDude = nullptr;
	Entity* grandpa = nullptr;

	Entity* player = nullptr;
	GuiButton* btnInventory;

	//Puzzle buttons, spikes & locked doors
	PuzzlePieces* buttonOne;
	PuzzlePieces* buttonTwo;
	PuzzlePieces* buttonThree;
	PuzzlePieces* buttonFour;
	PuzzlePieces* buttonFive;
	PuzzlePieces* buttonSix;

	Blockers* spikeRowOne;
	Blockers* spikeRowTwo;
	Blockers* spikeRowThree;
	Blockers* lockedDoor;

private:
	SDL_Texture* textBox = nullptr;
	SDL_Color white;

	//MENU
	SDL_Rect menuBox;
	SDL_Rect menuCharacterBox;
	GuiButton* btnSkills;
	GuiButton* btnSkillTree;
	GuiButton* btnEquipment;
	GuiButton* btnStats;
	GuiButton* btnOptions;
	GuiButton* btnTitleScreen;

	//OPTIONS
	GuiSlider* sldrVolume;
	GuiSlider* sldrFx;
	GuiCheckBox* boxFullScreen;
	GuiCheckBox* boxVSync;
	GuiButton* btnControls;
	GuiButton* btnBack;

	//CONTROLS
	GuiButton* btnKeySelect;
	GuiButton* btnKeyCancel;
	GuiButton* btnKeyMenu;
	GuiButton* btnKeyQuest;
	GuiButton* btnKeyUp;
	GuiButton* btnKeyDown;
	GuiButton* btnKeyLeft;
	GuiButton* btnKeyRight;
	GuiButton* btnBack2;
	GuiButton* btnPadSelect;
	GuiButton* btnPadCancel;
	GuiButton* btnPadMenu;
	GuiButton* btnPadQuest;
	GuiButton* btnPadUp;
	GuiButton* btnPadDown;
	GuiButton* btnPadLeft;
	GuiButton* btnPadRight;

	//ITEMS
	GuiImageButton* btnItem1;
	GuiImageButton* btnItem2;
	GuiImageButton* btnItem3;
	GuiImageButton* btnItem4;
	GuiImageButton* btnItem5;
	GuiImageButton* btnItem6;
	GuiImageButton* btnItem7;
	GuiImageButton* btnItem8;
	GuiImageButton* btnItem9;
	GuiImageButton* btnItem10;
	GuiImageButton* btnItem11;
	GuiImageButton* btnItem12;
	int itemSelected;

	//SKILLS
	GuiImageButton* btnSkill1;
	GuiImageButton* btnSkill2;
	GuiImageButton* btnSkill3;
	GuiImageButton* btnSkill4;

	// This line is added for the sake of commiting & testing

	//FONTS
	Font* dialogueFont = nullptr;
	Font* buttonFont = nullptr;

	bool onPause = false;

	//STATE FLAGS
	int flags;
	int statFlags;
    bool onceStatsFx;

	//DIALOG
	DialogSystem* dialogSystem = nullptr;
	float dtSave;
	float dtItem;
};

#endif // __SCENEGAMEPLAY_H__