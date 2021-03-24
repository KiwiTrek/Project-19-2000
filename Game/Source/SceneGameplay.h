#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "GuiButton.h"

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


    //CONTROLS


    //FONTS
    Font* dialogueFont = nullptr;

    uint flags;
};

#endif // __SCENEGAMEPLAY_H__