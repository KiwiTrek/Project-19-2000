#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class SceneTitleScreen : public Scene
{
public:
    enum Flags
    {
        OPTIONS,
        CONTROLS
    };

    SceneTitleScreen();
    virtual ~SceneTitleScreen();

    bool Load();

    bool Update(float dt);

    bool Draw();

    bool Unload();

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);


private:

    SDL_Texture* nooseBG = nullptr;
    SDL_Texture* titleCard = nullptr;
    SDL_Rect titleCardPos;
    SDL_Color white;
    float titleAlpha = 0.0f;
    bool onceFx = false;

    Animation noose;

    //MAIN MENU
    GuiButton* btnStart = nullptr;
    GuiButton* btnContinue = nullptr;
    GuiButton* btnOptions = nullptr;
    GuiButton* btnExit = nullptr;

    //OPTIONS                      
    GuiSlider* sldrVolume = nullptr;
    GuiSlider* sldrFx = nullptr;
    GuiCheckBox* boxFullScreen = nullptr;
    GuiCheckBox* boxVSync = nullptr;
    GuiButton* btnControls = nullptr;
    GuiButton* btnBack = nullptr;

    //CONTROLS                      
    GuiButton* btnKeySelect = nullptr;
    GuiButton* btnKeyCancel = nullptr;
    GuiButton* btnKeyMenu = nullptr;
    GuiButton* btnKeyQuest = nullptr;
    GuiButton* btnKeyUp = nullptr;
    GuiButton* btnKeyDown = nullptr;
    GuiButton* btnKeyLeft = nullptr;
    GuiButton* btnKeyRight = nullptr;
    GuiButton* btnBack2 = nullptr;
    GuiButton* btnPadSelect = nullptr;
    GuiButton* btnPadCancel = nullptr;
    GuiButton* btnPadMenu = nullptr;
    GuiButton* btnPadQuest = nullptr;
    GuiButton* btnPadUp = nullptr;
    GuiButton* btnPadDown = nullptr;
    GuiButton* btnPadLeft = nullptr;
    GuiButton* btnPadRight = nullptr;

    //FONTS
    Font* buttonFont = nullptr;

    //SOUND
    int titleFx = -1;

    unsigned char flags = 0;
};

#endif // __SCENETITLE_H__
