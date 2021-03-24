#include "SceneTitleScreen.h"

#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "EntityManager.h"

#include "SDL/include/SDL.h"


SceneTitleScreen::SceneTitleScreen()
{
    // GUI: Initialize required controls for the screen
    btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "START");
    btnStart->SetObserver(this);

    btnContinue = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "CONTINUE");
    btnContinue->SetObserver(this);

    btnOptions = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "OPTIONS");
    btnOptions->SetObserver(this);

    btnExit = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "EXIT");
    btnExit->SetObserver(this);

    for (int i = 0; i != 8; ++i) noose.PushBack({ i * 301,0,301,670 });
}

SceneTitleScreen::~SceneTitleScreen()
{
}

bool SceneTitleScreen::Load()
{
    nooseBG = app->tex->Load("Assets/Textures/NooseBG.png");
    titleCard = app->tex->Load("Assets/Textures/TitleCard.png");
    testFont = new Font("Assets/Fonts/Test.xml");

    app->audio->PlayMusic("Assets/Audio/Music/Title.ogg");
    noose.Reset();

    return false;
}

bool SceneTitleScreen::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);
    if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) TransitionToScene(SceneType::DEV_ROOM);
    noose.Update(dt);

    btnStart->Update(dt);
    btnContinue->Update(dt);
    btnOptions->Update(dt);
    btnExit->Update(dt);

    return false;
}

bool SceneTitleScreen::Draw()
{
    app->render->background = { 0, 0, 0, 255 };
    app->render->DrawTexture(nooseBG, 810, 0,false,&noose.GetCurrentFrame());
    app->render->DrawTexture(titleCard, 90, 64);

    btnStart->Draw();
    btnContinue->Draw();
    btnOptions->Draw();
    btnExit->Draw();

    //char score[64] = { 0 };
    //sprintf_s(score, 64, "SCORE: %03i", 56);
    char test[64] = { 0 };
    sprintf_s(test, 64, "Press Enter to Play...");
    
    app->render->DrawText(testFont, test, 90, 640, 64, 0, { 0, 255, 255, 255 });

    return false;
}

bool SceneTitleScreen::Unload()
{
    app->tex->UnLoad(nooseBG);
    app->tex->UnLoad(titleCard);

    return false;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
//bool SceneTitleScreen::OnGuiMouseClickEvent(GuiControl* control)
//{
//    switch (control->type)
//    {
//    case GuiControlType::BUTTON:
//    {
//        if (control->id == 1) TransitionToScene(SceneType::GAMEPLAY);
//        else if (control->id == 2) return false; // TODO: Exit request
//    }
//    default: break;
//    }
//
//    return true;
//}