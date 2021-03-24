#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

#include "GuiButton.h"

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
    //bool OnGuiMouseClickEvent(GuiControl* control);

private:

    SDL_Texture* nooseBG;
    SDL_Texture* titleCard;

    Animation noose;

    GuiButton* btnStart;
    GuiButton* btnContinue;
    GuiButton* btnOptions;
    GuiButton* btnExit;

    Font* testFont = nullptr;
    int titleFx;
};

#endif // __SCENETITLE_H__
