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

    SDL_Texture* nooseBG;
    SDL_Texture* titleCard;
    SDL_Rect titleCardPos;
    float titleAlpha;
    bool onceFx;

    Animation noose;

    //MAIN MENU
    GuiButton* btnStart;
    GuiButton* btnContinue;
    GuiButton* btnOptions;
    GuiButton* btnExit;

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
    Font* buttonFont = nullptr;

    //SOUND
    int titleFx;

    unsigned char flags;
};

#endif // __SCENETITLE_H__
