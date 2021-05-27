#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class SceneTitleScreen : public Scene
{
public:

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
    GuiButton* btnBack = nullptr;

    //FONTS
    Font* buttonFont = nullptr;

    //SOUND
    int titleFx = -1;

    bool options = false;
};

#endif // __SCENETITLE_H__
