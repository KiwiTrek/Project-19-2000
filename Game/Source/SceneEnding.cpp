#include "SceneEnding.h"

#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "SceneManager.h"
#include "GuiManager.h"
#include "Window.h"


#include "EntityManager.h"

#define TIMER_SPEED 0.8f

SceneEnding::SceneEnding()
{
	state = 0;
	timeCounter = 0.0f;
}

SceneEnding::~SceneEnding()
{
}

bool SceneEnding::Load()
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	char tmp[TEXT_LEN] = { 0 };
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderTexture, "Ending.png");
	bg = app->tex->Load(tmp);

	app->gui->Enable();
	btnContinue = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 1, { 0, 1020, 300, 60 }, "RETRY", 40, this);
	btnContinue->bounds.x = app->render->camera.w / 2 - btnContinue->bounds.w / 2;
	btnTitle = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 2, { 0, 1020, 300, 60 }, "TITLE", 40, this);
	btnTitle->bounds.x = app->render->camera.w / 2 - btnTitle->bounds.w / 2;
	btnExit = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 3, { 0, 1020, 300, 60 }, "EXIT", 40, this);
	btnExit->bounds.x = app->render->camera.w / 2 - btnExit->bounds.w / 2;

	app->scene->currentButton = app->gui->controls.start;

	pos.x = 0;
	pos.y = -app->win->screenSurface->h / 2;

	app->audio->PlayMusic("Audio/Music/GameOver.ogg", 2.0f);

	return false;
}

bool SceneEnding::Update(float dt)
{
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
	if (((tmpX > 3 || tmpX < -3) || (tmpY > 3 || tmpY < -3)) || (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN))
		usingGamepad = false;

	if (state == 0)
	{
		state = 1;
		app->render->CreateSpline(&pos.y, app->render->camera.h / 2 - 360, 3000, SplineType::QUART);
	}
	else if (state == 1)
	{
		timeCounter += (TIMER_SPEED * dt);

		if (timeCounter > 2.5f)
		{
			timeCounter = 5.0f;
			app->render->CreateSpline(&btnContinue->bounds.y, 400, 2000, SplineType::BACK);
			app->render->CreateSpline(&btnTitle->bounds.y, 500, 2000, SplineType::BACK);
			app->render->CreateSpline(&btnExit->bounds.y, 600, 2000, SplineType::BACK);
			state = 2;
		}
	}
	else if (state == 2)
	{
		if (usingGamepad)
		{
			app->scene->currentButton->data->Update(dt, 1, 3);
		}
		else
		{
			btnContinue->Update(dt);
			btnExit->Update(dt);
			btnTitle->Update(dt);
		}
	}

	return false;
}

bool SceneEnding::Draw()
{
	app->render->background = { 0,0,0,255 };
	app->render->DrawTexture(bg, pos.x, pos.y, true);

	btnContinue->Draw();
	btnTitle->Draw();
	btnExit->Draw();
	if (usingGamepad) app->scene->currentButton->data->Draw();

	return false;
}

bool SceneEnding::Unload()
{
	app->tex->UnLoad(bg);

	app->gui->Disable();
	app->scene->currentButton = nullptr;
	app->render->DestroySplines();

	return false;
}

bool SceneEnding::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 1: //CONTINUE
		TransitionToScene(SceneType::GAMEPLAY);
		app->scene->continueLoadRequest = true;
		break;
	case 2: //TITLE
		TransitionToScene(SceneType::TITLE_SCREEN);
		break;
	case 3: //OPTIONS
		app->exitRequest = true;
		break;
	default:
		break;
	}

	return true;
}