#ifndef __SCENECOMBAT_H__
#define __SCENECOMBAT_H__

#include "Scene.h"
#include "Animation.h"
#include "Entity.h"
#include "List.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "GuiManager.h"
#include "GuiImageButton.h"

class Entity;
class CombatEntity;
class Item;

struct  CombatCharacter
{
	CombatEntity* character;
	int x, y;
	SDL_Rect box;
	SDL_Rect characterTex;
	SString hp;
	SString mp;
	SString stress;

	SString lvl;
	SString xp;
	SString nextLvl;
	SString pAtk, mAtk, pDef, mDef, speed;
};

enum class StressStatus
{
	UNDER,
	BALANCE,
	OVER
};

enum class TransitionStatus
{
	NONE,
	SCENE,
	BATTLE,
	END
};

class SceneCombat : public Scene
{
public:
	enum Flags
	{
		ATTACK,
		SKILL,
		ITEMS,
		SPECIAL,
		FLEE
	};

	SceneCombat();
	virtual ~SceneCombat();

	bool Load();

	bool Start(EntityId id1 = EntityId::NOT_COMBAT, EntityId id2 = EntityId::NOT_COMBAT, EntityId id3 = EntityId::NOT_COMBAT);
	void SpawnEnemies(EntityId id1, EntityId id2, EntityId id3);
	bool SortSpeed(bool isSorted);

	bool Update(float dt);
	void Damage(CombatEntity* target, int damage, bool isMagic);
	void Heal(CombatEntity* target, float p);
	void Stress(int value);
	void ManaCost(int value);
	void SelectTarget();
	int EnemyTarget();
	bool IsCharacter(CombatEntity* e);
	void TickDownBuffs();
	void StressPower();
	SDL_Rect SkillSec(int i, EntityId id);
	void NextLine(const char* line);

	bool Draw(Font* dialogueFont);
	bool UpdateTransition(float dt, TransitionStatus transitionTo);

	bool Finish();
	void VictoryCondition();
	void DefeatCondition();

	bool Unload();

	// Declare on mouse click event
	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	//STATE FLAGS
	int flags;
	bool heDed;

	SDL_Texture* combatGui;
	SDL_Rect combatTextBox;
	SDL_Rect combatMenuBox;
	int characterFlags;

	CombatCharacter mainChar;
	CombatCharacter grandpa;
	//CombatCharacter childhood;
	//CombatCharacter extroverted;

	List<Item*> items;

	TransitionStatus waitForTransition;
	bool backToGameplay;
	float alpha;

private:
	List<CombatEntity*> turnOrder;
	ListItem<CombatEntity*>* currentEntity;
	CombatEntity* target;
	bool characterSelected; // (For now, this is a temporal value for menu changing)
	bool targetAttack;
	bool finishedAction;
	bool hasTicked;
	bool scripted;
	SString attackSelected;
	bool once;
	bool wait;

	SDL_Texture* transitionTx;
	Animation transition;

	StressStatus stressStatus;
	int pastStress;

	CombatCharacter* currentChar;
	Entity* enemy1;
	Entity* enemy2;
	Entity* enemy3;

	SDL_Color white;
	int combatMenuFlags;
	GuiButton* btnCombatAttack;
	GuiButton* btnCombatSkills;
	GuiButton* btnCombatItems;
	GuiButton* btnCombatSpecial;
	GuiButton* btnCombatFlee;

	GuiImageButton* btnCombatSkill1;
	GuiImageButton* btnCombatSkill2;
	GuiImageButton* btnCombatSkill3;
	GuiImageButton* btnCombatSkill4;
	//GuiImageButton* btnCombatSkill5;
	//GuiImageButton* btnCombatSkill6;

	GuiImageButton* btnCombatItem1;
	GuiImageButton* btnCombatItem2;
	GuiImageButton* btnCombatItem3;
	GuiImageButton* btnCombatItem4;
	GuiImageButton* btnCombatItem5;
	GuiImageButton* btnCombatItem6;
	GuiImageButton* btnCombatItem7;
	GuiImageButton* btnCombatItem8;
	GuiImageButton* btnCombatItem9;
	GuiImageButton* btnCombatItem10;
	GuiImageButton* btnCombatItem11;
	GuiImageButton* btnCombatItem12;
	GuiButton* btnLeftArrow;
	GuiButton* btnRightArrow;
	int itemSelected;
	bool pageOne;
	bool targetItem;

	SString firstLine;
	SString secondLine;
	SString thirdLine;
	SDL_Rect arrowCombat;
	bool blink;
	float blinkTimer;

	int smackFx;
	int slapFx;
	int confortFx;
	int speechFx;

	int smiteFx;
	int magicBlowFx;
	int magicBarrageFx;
	int magicSlapFx;
	int stareFx;

	int strongerFx;
	int bladesFx;
	int stressFx;
	int badDreamFx;
	int closeEyesFx;
	int deadFx;
	int graspFx;

	int enemiesCount;
};

#endif // __SCENECOMBAT_H__