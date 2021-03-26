#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class Entity;

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

    bool Draw();

    bool Unload();

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

    void ResetButtons();

private:

    Entity* player = nullptr;

    //MENU
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


    //FONTS
    Font* dialogueFont = nullptr;

    int flags;
};

#endif // __SCENEGAMEPLAY_H__