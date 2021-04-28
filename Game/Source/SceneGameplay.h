#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "SString.h"

class Entity;
class SceneCombat;
class DialogSystem;

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

    //FONTS
    Font* dialogueFont = nullptr;
    Font* buttonFont = nullptr;

    bool onPause = false;

    //STATE FLAGS
    int flags;
    int statFlags;

    //DIALOG
    DialogSystem* dialogSystem = nullptr;
    SDL_Texture* dialogGui;
    SDL_Rect dialogTextBox;
    SDL_Rect portraitBox;
    SDL_Rect shopKeeperPortrait;
    SDL_Rect catPortrait;
    SDL_Rect grandpaPortrait;
    SDL_Rect superheroPortrait;
    SDL_Rect hatsunePortrait;

    SString savingText;
    int savingCounter;
    bool savingBool;
};

#endif // __SCENEGAMEPLAY_H__