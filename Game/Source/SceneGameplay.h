#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class Entity;

struct  CombatCharacter
{
    Entity* character;
    int x, y;
    SDL_Rect box;
    SDL_Rect characterTex;
    SString hp;
    SString mp;
};

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

    enum CombatFlags
    {
        ATTACK,
        SKILL,
        ITEMS,
        SPECIAL,
        FLEE
    };

    SceneGameplay();
    virtual ~SceneGameplay();

    bool Load();

    bool Update(float dt);
    bool UpdatePauseMenu(float dt);
    bool UpdateCombat(float dt);

    bool Draw();
    bool DrawPauseMenu();
    bool DrawCharacterBox(CombatCharacter character);
    bool DrawCombat();

    bool Unload();

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

    void ResetButtons();

private:

    Entity* player = nullptr;
    SDL_Texture* textBox = nullptr;

    //MENU
    SDL_Rect menuBox;
    SDL_Rect menuCharacterBox;
    GuiButton* btnInventory;
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
    GuiButton* btnKeyUp;
    GuiButton* btnKeyDown;
    GuiButton* btnKeyLeft;
    GuiButton* btnKeyRight;
    GuiButton* btnBack2;
    GuiButton* btnPadSelect;
    GuiButton* btnPadCancel;
    GuiButton* btnPadMenu;
    GuiButton* btnPadUp;
    GuiButton* btnPadDown;
    GuiButton* btnPadLeft;
    GuiButton* btnPadRight;

    //COMBAT
    bool combat;
    bool characterSelected; // (For now, this is a temporal value for menu changing)
    SDL_Texture* combatGui;
    SDL_Rect combatTextBox;
    SDL_Rect combatMenuBox;
    int characterFlags;
    int combatMenuFlags;
    CombatCharacter* currentChar;
    CombatCharacter mainChar;
    CombatCharacter grandpa;
    //CombatCharacter childhood;
    //CombatCharacter extroverted;
    GuiButton* btnCombatAttack;
    GuiButton* btnCombatSkills;
    GuiButton* btnCombatItems;
    GuiButton* btnCombatSpecial;
    GuiButton* btnCombatFlee;

    //FONTS
    Font* dialogueFont = nullptr;
    Font* buttonFont = nullptr;

    //STATE FLAGS
    int flags;
};

#endif // __SCENEGAMEPLAY_H__