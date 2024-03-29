#include "SceneLogo.h"

#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"
#include "SceneManager.h"

#include "EntityManager.h"

#define TIMER_SPEED 0.8f

SceneLogo::SceneLogo()
{
	state = 0;
	timeCounter = 0.0f;
	timeVelocity = 0.0f;
	onceFx = true;
	onceOut = true;
}

SceneLogo::~SceneLogo()
{
}

bool SceneLogo::Load()
{
	char tmp[TEXT_LEN] = { 0 };
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderTexture, "Logo.png");
	logo = app->tex->Load(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "LogoFX.wav");
	logoFx = app->audio->LoadFx(tmp);

	pos.x = -500;
	pos.y = app->win->screenSurface->h / 2 - 250;

	return false;
}

bool SceneLogo::Update(float dt)
{
	if (state == 0)
	{
		state = 1;
		app->render->CreateSpline(&pos.x, app->win->screenSurface->w / 2 - 250, 1350, SplineType::BACK);
	}
	else if (state == 1)
	{
		timeVelocity += (TIMER_SPEED * dt);

		if (timeVelocity > 1.0f)
		{
			timeVelocity = 1.0f;
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
		if (onceOut)
		{
			onceOut = false;
			app->render->CreateSpline(&pos.x, app->win->screenSurface->w, 1350, SplineType::BACK);
		}

		timeVelocity -= (TIMER_SPEED * dt);

		if (timeVelocity < 0.0f)
		{
			timeVelocity = 0.0f;
			TransitionToScene(SceneType::TITLE_SCREEN);
		}
	}

	return false;
}

bool SceneLogo::Draw()
{
	app->render->background = { 255,255,255,255 };
	app->render->DrawTexture(logo, pos.x, pos.y);
	//app->render->DrawRectangle(app->render->camera, 255, 255, 255, (uchar)(255 - (255 * logoAlpha)));

	return false;
}

bool SceneLogo::Unload()
{
	app->tex->UnLoad(logo);
	app->audio->UnloadFx(logoFx);

	return false;
}
