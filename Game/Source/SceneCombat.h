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
    Entity* character;
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

    bool Update(float dt);

    bool Draw(Font* dialogueFont);

    bool Unload();

    bool SortSpeed(bool isSorted);

    bool IsCharacter(CombatEntity* e);

    void SelectTarget();

    void ResetButtons();

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
    int attackSelected;

    CombatCharacter* currentChar;
    Entity* enemy1;
    Entity* enemy2;

    int combatMenuFlags;
    GuiButton* btnCombatAttack;
    GuiButton* btnCombatSkills;
    GuiButton* btnCombatItems;
    GuiButton* btnCombatSpecial;
    GuiButton* btnCombatFlee;
};

#endif // __SCENECOMBAT_H__