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
    CombatEntity* character = nullptr;
    int x = 0, y = 0;
    SDL_Rect box = { 0,0,0,0 };
    SDL_Rect characterTex = { 0,0,0,0 };
    char hp[TEXT_LEN] = { 0 };
    char mp[TEXT_LEN] = { 0 };
    char stress[TEXT_LEN] = { 0 };

    char lvl[TEXT_LEN] = { 0 };
    char xp[TEXT_LEN] = { 0 };
    char nextLvl[TEXT_LEN] = { 0 };
    char pAtk[TEXT_LEN] = { 0 };
    char mAtk[TEXT_LEN] = { 0 };
    char pDef[TEXT_LEN] = { 0 };
    char mDef[TEXT_LEN] = { 0 };
    char speed[TEXT_LEN] = { 0 };
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
    void UpdateSelection(const SDL_Rect r);
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
    int flags = 0;
    bool heDed = false;

    SDL_Texture* combatGui = nullptr;
    SDL_Rect combatTextBox;
    SDL_Rect combatMenuBox;
    int characterFlags = 0;

    CombatCharacter mainChar;
    CombatCharacter grandpa;
    //CombatCharacter childhood;
    //CombatCharacter extroverted;

    List<Item*> items;

    TransitionStatus waitForTransition = TransitionStatus::NONE;
    bool backToGameplay = false;
    float alpha = 0.0f;

private:
    List<CombatEntity*> turnOrder;
    ListItem<CombatEntity*>* currentEntity = nullptr;
    CombatEntity* target = nullptr;
    float targetFrameTime;
    bool increaseSelect;
    bool characterSelected = false; // (For now, this is a temporal value for menu changing)
    bool targetAttack = false;
    bool finishedAction = false;
    bool hasTicked = false;
    bool scripted = false;
    char attackSelected[TEXT_LEN] = { 0 };
    bool once = false;
    bool wait = false;

    SDL_Texture* transitionTx = nullptr;
    Animation transition;

    StressStatus stressStatus = StressStatus::UNDER;
    int pastStress = 0;

    CombatCharacter* currentChar = nullptr;
    Entity* enemy1 = nullptr;
    Entity* enemy2 = nullptr;
    Entity* enemy3 = nullptr;

    SDL_Color white;
    int combatMenuFlags = 0;
    GuiButton* btnCombatAttack = nullptr;
    GuiButton* btnCombatSkills = nullptr;
    GuiButton* btnCombatItems = nullptr;
    GuiButton* btnCombatSpecial = nullptr;
    GuiButton* btnCombatFlee = nullptr;

    GuiImageButton* btnCombatSkill1 = nullptr;
    GuiImageButton* btnCombatSkill2 = nullptr;
    GuiImageButton* btnCombatSkill3 = nullptr;
    GuiImageButton* btnCombatSkill4 = nullptr;
    //GuiImageButton* btnCombatSkill5;
    //GuiImageButton* btnCombatSkill6;

    GuiImageButton* btnCombatItem1 = nullptr;
    GuiImageButton* btnCombatItem2 = nullptr;
    GuiImageButton* btnCombatItem3 = nullptr;
    GuiImageButton* btnCombatItem4 = nullptr;
    GuiImageButton* btnCombatItem5 = nullptr;
    GuiImageButton* btnCombatItem6 = nullptr;
    GuiImageButton* btnCombatItem7 = nullptr;
    GuiImageButton* btnCombatItem8 = nullptr;
    GuiImageButton* btnCombatItem9 = nullptr;
    GuiImageButton* btnCombatItem10 = nullptr;
    GuiImageButton* btnCombatItem11 = nullptr;
    GuiImageButton* btnCombatItem12 = nullptr;
    GuiButton* btnLeftArrow = nullptr;
    GuiButton* btnRightArrow = nullptr;
    int itemSelected = 0;
    bool pageOne = true;
    bool targetItem = false;

    char firstLine[TEXT_LEN] = { 0 };
    char secondLine[TEXT_LEN] = { 0 };
    char thirdLine[TEXT_LEN] = { 0 };
    SDL_Rect arrowCombat;
    bool blink = false;
    float blinkTimer = 0.0f;

    int smackFx = -1;
    int slapFx = -1;
    int confortFx = -1;
    int speechFx = -1;

    int smiteFx = -1;
    int magicBlowFx = -1;
    int magicBarrageFx = -1;
    int magicSlapFx = -1;
    int stareFx = -1;

    int strongerFx = -1;
    int bladesFx = -1;
    int stressFx = -1;
    int badDreamFx = -1;
    int closeEyesFx = -1;
    int deadFx = -1;
    int graspFx = -1;

    int enemiesCount = 0;
};

#endif // __SCENECOMBAT_H__