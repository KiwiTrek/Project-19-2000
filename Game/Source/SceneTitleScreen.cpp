#include "SceneTitleScreen.h"

#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Window.h"
#include "List.h"
#include "SceneManager.h"

#include "EntityManager.h"

#include "SDL/include/SDL.h"


SceneTitleScreen::SceneTitleScreen()
{    
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
    buttonFont = new Font("Assets/Fonts/ButtonFont.xml");
    titleFx = app->audio->LoadFx("Assets/Audio/Fx/TitleFX.wav");

    app->audio->SetMusicVolume(app->audio->auxVolume);
    app->audio->PlayMusic("Assets/Audio/Music/Title.ogg", 0.0f);
    app->render->camera.x = 0;
    app->render->camera.y = 0;
    noose.Reset();

    app->audio->PlayFx(titleFx, 0);

    // GUI: Initialize required controls for the screen
    float tmpValue = 0;

    //MAIN MENU
    app->gui->Enable();
    btnStart = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 1, { 90, 300, 300, 60 }, "START", 40, this);
    btnContinue = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 2, { 90, 400, 300, 60 }, "CONTINUE", 40, this);
    btnOptions = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 3, { 90, 500, 300, 60 }, "OPTIONS", 40, this);
    btnExit = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 4, { 90, 600, 300, 60 }, "EXIT", 40, this);
    /*if (app->input->GetControllerName() != "unplugged") btnStart->state = GuiControlState::FOCUSED;*/

    //OPTIONS
    sldrVolume = (GuiSlider*)app->gui->CreateGuiControl(GuiControlType::SLIDER, 5, { 180, 200, 69, 42 }, "VOLUME", 40, this, 6);
    sldrVolume->value = app->audio->GetMusicVolume();
    sldrVolume->maxValue = 128;
    tmpValue = (float)(sldrVolume->limits.w - sldrVolume->bounds.w) / (float)sldrVolume->maxValue;
    sldrVolume->bounds.x = sldrVolume->limits.x + (tmpValue * sldrVolume->value);

    sldrFx = (GuiSlider*)app->gui->CreateGuiControl(GuiControlType::SLIDER, 6, { 800, 200, 69, 42 }, "FX", 40, this, 6);
    sldrFx->value = app->audio->GetFxVolume();
    sldrFx->maxValue = 128;
    tmpValue = (float)(sldrFx->limits.w - sldrFx->bounds.w) / (float)sldrFx->maxValue;
    sldrFx->bounds.x = sldrFx->limits.x + (tmpValue * sldrFx->value);

    boxFullScreen = (GuiCheckBox*)app->gui->CreateGuiControl(GuiControlType::CHECKBOX, 7, { 180, 400, 60, 60 }, "FULLSCREEN", 40, this);
    boxVSync = (GuiCheckBox*)app->gui->CreateGuiControl(GuiControlType::CHECKBOX, 8, { 800, 400, 60, 60 }, "VSync", 40, this);
    btnControls = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 9, { 180, 600, 200, 60 }, "CONTROLS", 35, this);
    btnBack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 10, { 800, 600, 200, 60 }, "BACK", 40, this);
    
    //CONTROLS
    btnKeySelect = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 11, { 180, 120, 300, 60 }, "SELECT", 40, this);
    btnKeyCancel = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 12, { 180, 200, 300, 60 }, "CANCEL", 40, this);
    btnKeyMenu = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 13, { 180, 280, 300, 60 }, "MENU", 40, this);
    btnKeyUp = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 14, { 180, 360, 300, 60 }, "UP", 40, this);
    btnKeyDown = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 15, { 180, 440, 300, 60 }, "DOWN", 40, this);
    btnKeyLeft = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 16, { 180, 520, 300, 60 }, "LEFT", 40, this);
    btnKeyRight = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 17, { 180, 600, 300, 60 }, "RIGHT", 40, this);
    btnPadSelect = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 18, { 800, 120, 300, 60 }, "SELECT", 40, this);
    btnPadCancel = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 19, { 800, 200, 300, 60 }, "CANCEL", 40, this);
    btnPadMenu = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 20, { 800, 280, 300, 60 }, "MENU", 40, this);
    btnPadUp = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 21, { 800, 360, 300, 60 }, "UP", 40, this);
    btnPadDown = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 22, { 800, 440, 300, 60 }, "DOWN", 40, this);
    btnPadLeft = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 23, { 800, 520, 300, 60 }, "LEFT", 40, this);
    btnPadRight = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 24, { 800, 600, 300, 60 }, "RIGHT", 40, this);
    btnBack2 = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 25, { 1280 / 2 - 200 / 2, 600, 200, 60 }, "BACK", 40, this);

    // Used for the Gamepad GUI control
    app->scene->currentButton = app->gui->controls.start;
    changeMenu = false;
    if (app->input->GetControllerName() != "unplugged") usingGamepad = false;
    app->input->mouseMotionX = 0;
    app->input->mouseMotionY = 0;
    app->scene->continueLoadRequest = false;

    return false;
}


bool SceneTitleScreen::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) TransitionToScene(SceneType::DEV_ROOM);
    noose.Update(dt);
    
    // Logic for using Gamepad or mouse (GUI)
    ListItem<InputButton*>* gamepadControls = app->input->controlConfig.start;
    while (gamepadControls->next != nullptr)
    {
        if (app->input->GetPadKey(gamepadControls->data->gamePadId) == KEY_DOWN)
        {
            usingGamepad = true;
            break;
        }
        gamepadControls = gamepadControls->next;
    }
    int tmpX = 0, tmpY = 0;
    app->input->GetMouseMotion(tmpX, tmpY);
    if (((tmpX > 3 || tmpX < -3) || (tmpY > 3 || tmpY < -3)) || (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)) usingGamepad = false;

    // Calls update with gamepad parameters (GUI)
    if (usingGamepad)
    {
        if ((flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
        {
            if (changeMenu)
            {
                app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnStart));
                changeMenu = false;
            }
            app->scene->currentButton->data->Update(dt, 1, 4);
        }
        else if ((flags & 1 << Flags::CONTROLS) == 0)
        {
            if (changeMenu)
            {
                app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(sldrVolume));
                changeMenu = false;
            }
            app->scene->currentButton->data->Update(dt, 5, 10);
        }
        else
        {
            if (changeMenu)
            {
                app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnKeySelect));
                changeMenu = false;
            }
            app->scene->currentButton->data->Update(dt, 11, 25);
        }
    }
    // Calls update for mouse parameters (GUI)
    else
    {
        if ((flags & 1 << Flags::OPTIONS) == 0 && (flags & 1 << Flags::CONTROLS) == 0)
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
    }

    //LOG("%d", flags);
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
        SString titleOptions = "Options";
        app->render->DrawText(buttonFont, titleOptions.GetString(), /*app->render->camera.x + */((app->render->camera.w - (titleOptions.Length() * 24))/2), 100, 64, 2, { 255, 255, 255, 255 });

        sldrVolume->Draw();
        sldrFx->Draw();
        if (app->scene->fullscreenChecked) boxFullScreen->checked = true;
        boxFullScreen->Draw();
        boxVSync->Draw();
        btnControls->Draw();
        btnBack->Draw();
    }
    else
    {
        app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);
        SString titleControls = "Controls";
        app->render->DrawText(buttonFont, titleControls.GetString(), /*app->render->camera.x +*/ ((app->render->camera.w - (titleControls.Length() * 24)) / 2), 100, 64, 2, { 255, 255, 255, 255 });

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
    if (usingGamepad) app->scene->currentButton->data->Draw();
    //char test[64] = { 0 };
    //sprintf_s(test, 64, "SCORE: %03i", 56);
    //app->render->DrawText(dialogueFont, test, 700, 640, 64, 2, { 0, 255, 255, 255 });

    return true;
}

bool SceneTitleScreen::Unload()
{
    if (nooseBG != nullptr) app->tex->UnLoad(nooseBG);
    if (titleCard != nullptr) app->tex->UnLoad(titleCard);
    if (titleFx != -1) app->audio->UnloadFx(titleFx);
    app->gui->Disable();
    if (buttonFont != nullptr) delete buttonFont;
    if (app->scene->currentButton != nullptr) app->scene->currentButton = nullptr;

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
        TransitionToScene(SceneType::GAMEPLAY);
        app->scene->continueLoadRequest = true;
        break;
    case 3: //OPTIONS
        flags = SetBit(flags, Flags::OPTIONS);
        changeMenu = true;
        app->gui->ResetButtons();
        usingGamepad = true;
        break;
    case 4: //EXIT
        app->exitRequest = true;
        break;
    case 5: //VOLUME
        app->audio->SetMusicVolume(sldrVolume->value / 2);
        app->audio->auxVolume = sldrVolume->value;
        break;
    case 6: //FX
        app->audio->SetFxVolumeValue(sldrFx->value);
        break;
    case 7: //FULLSCREEN
        app->win->ToggleFullscreen(boxFullScreen->checked);
        break;
    case 8: //VSYNC
        //app->win->ToggleFullscreen(false);
        //app->render->ToggleVsync(boxVSync->checked, (Module*)this);
        break;
    case 9: //CONTROLS
        flags = SetBit(flags, Flags::CONTROLS);
        changeMenu = true;
        app->gui->ResetButtons();
        usingGamepad = true;
        break;
    case 10: //BACK
        flags = ClearBit(flags, Flags::OPTIONS);
        changeMenu = true;
        app->gui->ResetButtons();
        usingGamepad = true;
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
    case 18: //PAD SELECT
        break;
    case 19: //PAD CANCEL
        break;
    case 20: //PAD MENU
        break;
    case 21: //PAD UP
        break;
    case 22: //PAD DOWN
        break;
    case 23: //PAD LEFT
        break;
    case 24: //PAD RIGHT
        break;
    case 25: //BACK 2
        flags = ClearBit(flags, Flags::CONTROLS);
        changeMenu = true;
        app->gui->ResetButtons();
        usingGamepad = true;
        break;
    default:
        break;
    }

    return true;
}