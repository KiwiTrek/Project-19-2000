#include "SceneTitleScreen.h"

#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "GuiManager.h"

#include "EntityManager.h"

#include "SDL/include/SDL.h"


SceneTitleScreen::SceneTitleScreen()
{
    // GUI: Initialize required controls for the screen
    //MAIN MENU
    app->gui->Enable();
    btnStart = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 1, { 90, 300, 300, 60 }, "START", this);
    btnContinue = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 2, { 90, 400, 300, 60 }, "CONTINUE", this);
    btnOptions = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 3, { 90, 500, 300, 60 }, "OPTIONS", this);
    btnExit = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 4, { 90, 600, 300, 60 }, "EXIT", this);

    //OPTIONS
    sldrVolume = (GuiSlider*)app->gui->CreateGuiControl(GuiControlType::SLIDER, 5, { 180, 200, 60, 60 }, "VOLUME", this, 6);
    sldrFx = (GuiSlider*)app->gui->CreateGuiControl(GuiControlType::SLIDER, 6, { 800, 200, 60, 60 }, "FX", this, 6);
    boxFullScreen = (GuiCheckBox*)app->gui->CreateGuiControl(GuiControlType::CHECKBOX, 7, { 180, 400, 60, 60 }, "FULLSCREEN", this);
    boxVSync = (GuiCheckBox*)app->gui->CreateGuiControl(GuiControlType::CHECKBOX, 8, { 800, 400, 60, 60 }, "VSync", this);
    btnControls = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 9, { 180, 600, 200, 60 }, "CONTROLS", this);
    btnBack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 10, { 800, 600, 200, 60 }, "BACK", this);

    //CONTROLS
    btnKeySelect = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 11, { 180, 120, 300, 60 }, "SELECT", this);
    btnKeyCancel = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 12, { 180, 200, 300, 60 }, "CANCEL", this);
    btnKeyMenu = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 13, { 180, 280, 300, 60 }, "MENU", this);
    btnKeyUp = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 14, { 180, 360, 300, 60 }, "UP", this);
    btnKeyDown = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 15, { 180, 440, 300, 60 }, "DOWN", this);
    btnKeyLeft = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 16, { 180, 520, 300, 60 }, "LEFT", this);
    btnKeyRight = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 17, { 180, 600, 300, 60 }, "RIGHT", this);
    btnBack2 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 18, { 1280/2 - 200/2, 600, 200, 60 }, "BACK", this);
    btnPadSelect = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 19, { 800, 120, 300, 60 }, "SELECT", this);
    btnPadCancel = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 20, { 800, 200, 300, 60 }, "CANCEL", this);
    btnPadMenu = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 21, { 800, 280, 300, 60 }, "MENU", this);
    btnPadUp = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 22, { 800, 360, 300, 60 }, "UP", this);
    btnPadDown = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 23, { 800, 440, 300, 60 }, "DOWN", this);
    btnPadLeft = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 24, { 800, 520, 300, 60 }, "LEFT", this);
    btnPadRight = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 25, { 800, 600, 300, 60 }, "RIGHT", this);

    for (int i = 0; i != 8; ++i) noose.PushBack({ i * 301,0,301,670 });

    flags = 0;
}

SceneTitleScreen::~SceneTitleScreen()
{
}

bool SceneTitleScreen::Load()
{
    nooseBG = app->tex->Load("Assets/Textures/NooseBG.png");
    titleCard = app->tex->Load("Assets/Textures/TitleCard.png");
    dialogueFont = new Font("Assets/Fonts/DialogueFont.xml");

    app->audio->PlayMusic("Assets/Audio/Music/Title.ogg");
    noose.Reset();

    return true;
}

bool SceneTitleScreen::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) TransitionToScene(SceneType::DEV_ROOM);
    noose.Update(dt);

    if ((flags & 1<<Flags::OPTIONS) == 0 && (flags & 1<<Flags::CONTROLS) == 0)
    {
        btnStart->Update(dt);
        btnContinue->Update(dt);
        btnOptions->Update(dt);
        btnExit->Update(dt);
    }
    else if ((flags & 1 << Flags::CONTROLS) == 0)
    {
        sldrVolume->Update(dt);
        sldrFx->Update(dt);
        boxFullScreen->Update(dt);
        boxVSync->Update(dt);
        btnControls->Update(dt);
        btnBack->Update(dt);
    }
    else
    {
        btnKeySelect->Update(dt);
        btnKeyCancel->Update(dt);
        btnKeyMenu->Update(dt);
        btnKeyUp->Update(dt);
        btnKeyDown->Update(dt);
        btnKeyLeft->Update(dt);
        btnKeyRight->Update(dt);
        btnBack2->Update(dt);
        btnPadSelect->Update(dt);
        btnPadCancel->Update(dt);
        btnPadMenu->Update(dt);
        btnPadUp->Update(dt);
        btnPadDown->Update(dt);
        btnPadLeft->Update(dt);
        btnPadRight->Update(dt);
    }

    LOG("%d", flags);
    return true;
}

bool SceneTitleScreen::Draw()
{
    app->render->background = { 0, 0, 0, 255 };
    app->render->DrawTexture(nooseBG, 810, 0,false,&noose.GetCurrentFrame());
    app->render->DrawTexture(titleCard, 90, 64);

    if ((flags & 1<<Flags::OPTIONS) == 0 && (flags & 1<<Flags::CONTROLS) == 0)
    {
        btnStart->Draw();
        btnContinue->Draw();
        btnOptions->Draw();
        btnExit->Draw();
    }
    else if ((flags & 1 << Flags::CONTROLS) == 0)
    {
        app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);
        sldrVolume->Draw();
        sldrFx->Draw();
        boxFullScreen->Draw();
        boxVSync->Draw();
        btnControls->Draw();
        btnBack->Draw();
    }
    else
    {
        app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);
        btnKeySelect->Draw();
        btnKeyCancel->Draw();
        btnKeyMenu->Draw();
        btnKeyUp->Draw();
        btnKeyDown->Draw();
        btnKeyLeft->Draw();
        btnKeyRight->Draw();
        btnBack2->Draw();
        btnPadSelect->Draw();
        btnPadCancel->Draw();
        btnPadMenu->Draw();
        btnPadUp->Draw();
        btnPadDown->Draw();
        btnPadLeft->Draw();
        btnPadRight->Draw();
    }

    char test[64] = { 0 };
    sprintf_s(test, 64, "SCORE: %03i", 56);

    app->render->DrawText(dialogueFont, test, 700, 640, 64, 2, { 0, 255, 255, 255 });

    return true;
}

bool SceneTitleScreen::Unload()
{
    app->tex->UnLoad(nooseBG);
    app->tex->UnLoad(titleCard);

    return true;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneTitleScreen::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->id)
    {
    case 1: //START
        TransitionToScene(SceneType::GAMEPLAY);
        break;
    case 2: //CONTINUE
        app->LoadRequest();
        TransitionToScene(SceneType::GAMEPLAY);
        break;
    case 3: //OPTIONS
        flags = SetBit(flags,Flags::OPTIONS);
        break;
    case 4: //EXIT
        app->exitRequest = true;
        break;
    case 5: //VOLUME
        break;
    case 6: //FX
        break;
    case 7: //FULLSCREEN
        break;
    case 8: //VSYNC
        break;
    case 9: //CONTROLS
        flags = SetBit(flags,Flags::CONTROLS);
        break;
    case 10: //BACK
        flags = ClearBit(flags,Flags::OPTIONS);
        break;
    case 11: //KEY SELECT
        break;
    case 12: //KEY CANCEL
        break;
    case 13: //KEY MENU
        break;
    case 14: //KEY UP
        break;
    case 15: //KEY DOWN
        break;
    case 16: //KEY LEFT
        break;
    case 17: //KEY RIGHT
        break;
    case 18: //BACK 2
        flags = ClearBit(flags,Flags::CONTROLS);
        break;
    case 19: //PAD SELECT
        break;
    case 20: //PAD CANCEL
        break;
    case 21: //PAD MENU
        break;
    case 22: //PAD UP
        break;
    case 23: //PAD DOWN
        break;
    case 24: //PAD LEFT
        break;
    case 25: //PAD RIGHT
        break;
    default:
        break;
    }

    return true;
}