#include "SceneTitleScreen.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
//#include "Font.h"

#include "EntityManager.h"

#include "SDL/include/SDL.h"


SceneTitleScreen::SceneTitleScreen()
{
    // GUI: Initialize required controls for the screen
    //btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "START");
    //btnStart->SetObserver(this);

    //btnExit = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "EXIT");
    //btnExit->SetObserver(this);
}

SceneTitleScreen::~SceneTitleScreen()
{
}

bool SceneTitleScreen::Load()
{
    nooseBG = app->tex->Load("Assets/Textures/NooseBG.png");
    titleCard = app->tex->Load("Assets/Textures/TitleCard.png");
    //font = new Font("Assets/Fonts/londrina.xml", tex);

    return false;
}

bool SceneTitleScreen::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);

    //btnStart->Update(dt);
    //btnExit->Update(dt);

    return false;
}

bool SceneTitleScreen::Draw()
{
    app->render->background = { 0, 0, 0, 255 };
    app->render->DrawTexture(nooseBG, 810, 0);
    app->render->DrawTexture(titleCard, 90, 64);

    //btnStart->Draw();
    //btnExit->Draw();

    //char score[64] = { 0 };
    //sprintf_s(score, 64, "SCORE: %03i", 56);

    //app->render->DrawText(font, score, 10, 10, 200, 0, { 255, 0, 255, 255 });

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