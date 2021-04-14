#ifndef __SCENECOMBAT_H__
#define __SCENECOMBAT_H__

#include "Scene.h"
#include "Entity.h"
#include "List.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class Entity;
class CombatEntity;

struct  CombatCharacter
{
    CombatEntity* character;
    int x, y;
    SDL_Rect box;
    SDL_Rect characterTex;
    SString hp;
    SString mp;
    SString stress;
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
    void Damage(int index, CombatEntity* target, bool isMagic = false);
    void SelectTarget();
    int EnemyTarget();
    bool IsCharacter(CombatEntity* e);
    void TickDownBuffs();

    bool Draw(Font* dialogueFont);

    bool Finish();
    void VictoryCondition();
    void DefeatCondition();

    bool Unload();

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);


public:
    //STATE FLAGS
    int flags;

    SDL_Texture* combatGui;
    SDL_Rect combatTextBox;
    SDL_Rect combatMenuBox;
    int characterFlags;

    CombatCharacter mainChar;
    CombatCharacter grandpa;
    //CombatCharacter childhood;
    //CombatCharacter extroverted;

private:
    List<CombatEntity*> turnOrder;
    ListItem<CombatEntity*>* currentEntity;
    CombatEntity* target;
    bool characterSelected; // (For now, this is a temporal value for menu changing)
    bool targetAttack;
    bool finishedAction;
    bool hasTicked;
    bool scripted;
    int attackSelected;
    bool once;

    CombatCharacter* currentChar;
    Entity* enemy1;
    Entity* enemy2;
    Entity* enemy3;

    int combatMenuFlags;
    GuiButton* btnCombatAttack;
    GuiButton* btnCombatSkills;
    GuiButton* btnCombatItems;
    GuiButton* btnCombatSpecial;
    GuiButton* btnCombatFlee;
    GuiButton* btnCombatSkill1;
    GuiButton* btnCombatSkill2;
    GuiButton* btnCombatSkill3;
    GuiButton* btnCombatSkill4;
    GuiButton* btnCombatSkill5;
    GuiButton* btnCombatSkill6;
    GuiButton* btnCombatItem1;
    GuiButton* btnCombatItem2;
    GuiButton* btnCombatItem3;
    GuiButton* btnCombatItem4;
    GuiButton* btnCombatItem5;
    GuiButton* btnCombatItem6;
}

#endif // __SCENECOMBAT_H__