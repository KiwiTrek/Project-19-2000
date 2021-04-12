#include "SceneLogo.h"

#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "EntityManager.h"

#define LOGO_FADE_SPEED 1.0f

SceneLogo::SceneLogo()
{
    state = 0;
    timeCounter = 0.0f;
    logoAlpha = 0.0f;
    onceFx = true;
}

SceneLogo::~SceneLogo()
{
}

bool SceneLogo::Load()
{
    logo = app->tex->Load("Assets/Textures/Logo.png");
    logoFx = app->audio->LoadFx("Assets/Audio/Fx/LogoFX.wav");
    return false;
}

bool SceneLogo::Update(float dt)
{
    if (state == 0)
    {
        state = 1;
    }
    else if (state == 1)
    {
        logoAlpha += (LOGO_FADE_SPEED * dt);

        if (logoAlpha > 1.0f)
        {
            logoAlpha = 1.0f;
            state = 2;
        }
    }
    else if (state == 2)
    {
        // Waiting for 3 seconds
        timeCounter += dt;
        if (timeCounter >= 1.0f && onceFx)
        {
            onceFx = false;
            app->audio->PlayFx(logoFx, 0);
        }
        if (timeCounter >= 3.0f) state = 3;
    }
    else if (state == 3)
    {
        logoAlpha -= (LOGO_FADE_SPEED * dt);

        if (logoAlpha < 0.0f)
        {
            logoAlpha = 0.0f;
            TransitionToScene(SceneType::TITLE_SCREEN);
        }
    }

    return false;
}

bool SceneLogo::Draw()
{
    app->render->background = { 255,255,255,255 };
    app->render->DrawTexture(logo, app->win->screenSurface->w/2 - 250, app->win->screenSurface->h/2 - 250);
    app->render->DrawRectangle(app->render->camera, 255, 255, 255, (uchar)(255 - (255 * logoAlpha)));

    return false;
}

bool SceneLogo::Unload()
{
    if (logo != nullptr) app->tex->UnLoad(logo);
    if (logoFx != -1) app->audio->UnloadFx(logoFx);

    return false;
}
