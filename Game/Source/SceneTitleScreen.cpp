#include "SceneTitleScreen.h"

#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Window.h"
#include "List.h"
#include "SceneManager.h"
#include "ParticleSystem.h"

#include "EntityManager.h"

#include "SDL/include/SDL.h"

#define TITLE_FADE_SPEED 1.5f


SceneTitleScreen::SceneTitleScreen()
{    
	for (int i = 0; i != 8; ++i) noose.PushBack({ i * 301,0,301,670 });
	white = { 255,255,255,255 };
    options = false;
    nooseBG = nullptr;
    titleCard = nullptr;
    titleCardPos = {0,0,0,0};
    titleAlpha = 0.0f;
    onceFx = false;

    btnStart = nullptr;
    btnContinue = nullptr;
    btnOptions = nullptr;
    btnExit = nullptr;

    //OPTIONS                      
    sldrVolume = nullptr;
    sldrFx = nullptr;
    boxFullScreen = nullptr;
    boxVSync = nullptr;
    btnBack = nullptr;

    //FONTS
    buttonFont = nullptr;

    //SOUND
    titleFx = -1;

    options = false;
}

SceneTitleScreen::~SceneTitleScreen()
{
}

bool SceneTitleScreen::Load()
{
	char tmp[TEXT_LEN] = { 0 };
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderTexture, "NooseBG.png");
	nooseBG = app->tex->Load(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderTexture, "TitleCard.png");
	titleCard = app->tex->Load(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderFonts, "ButtonFont.xml");
	buttonFont = new Font(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", app->scene->folderAudioFx, "TitleFX.wav");
	titleFx = app->audio->LoadFx(tmp);

	app->audio->SetMusicVolume(app->audio->auxVolume);
	app->audio->PlayMusic("Audio/Music/Title.ogg");

	app->render->camera.x = 0;
	app->render->camera.y = 0;
	noose.Reset();

	// GUI: Initialize required controls for the screen
	float tmpValue = 0;

	app->particles->Enable();

	//MAIN MENU
	app->gui->Enable();
	btnStart = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 1, { -300, 300, 300, 60 }, "START", 40, this);
	app->render->CreateSpline(&btnStart->bounds.x, 90, 2000, SplineType::QUART);
	btnContinue = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 2, { -300, 400, 300, 60 }, "CONTINUE", 40, this);
	app->render->CreateSpline(&btnContinue->bounds.x, 90, 2000, SplineType::QUART);
	btnOptions = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 3, { -300, 500, 300, 60 }, "OPTIONS", 40, this);
	app->render->CreateSpline(&btnOptions->bounds.x, 90, 2000, SplineType::QUART);
	btnExit = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 4, { -300, 600, 300, 60 }, "EXIT", 40, this);
	app->render->CreateSpline(&btnExit->bounds.x, 90, 2000, SplineType::QUART);
    options = false;
	/*if (strcmp(app->input->GetControllerName(), "unplugged") != 0) btnStart->state = GuiControlState::FOCUSED;*/

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
	btnBack = (GuiButton*)app->gui->CreateGuiControl(GuiControlType::BUTTON, 9, { 800, 600, 200, 60 }, "BACK", 40, this);
	
	// Used for the Gamepad GUI control
	app->scene->currentButton = app->gui->controls.start;
	changeMenu = false;
	usingGamepad = false;
	if (strcmp(app->input->GetControllerName(), "unplugged") != 0) usingGamepad = false;
	app->input->mouseMotionX = 0;
	app->input->mouseMotionY = 0;
	app->scene->continueLoadRequest = false;

	// Title Card easings
	onceFx = true;
	titleCardPos.x = 90;
	uint tmpW;
	uint tmpH;
	app->tex->GetSize(titleCard, tmpW, tmpH);
	titleCardPos.w = tmpW;
	titleCardPos.h = tmpH;
	titleCardPos.y = -titleCardPos.h;
	titleAlpha = 0.0f;
	app->render->CreateSpline(&titleCardPos.y,64,2000,SplineType::QUART);

	return false;
}


bool SceneTitleScreen::Update(float dt)
{
	noose.Update(dt);
	titleAlpha += (TITLE_FADE_SPEED * dt);
	if (titleAlpha >= 0.85f && onceFx)
	{
		onceFx = false;
		app->audio->PlayFx(titleFx, 0);
	}
	if (titleAlpha > 1.0f) { titleAlpha = 1.0f; }
	
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
		if (options)
		{
            if (changeMenu)
            {
                app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(sldrVolume));
                changeMenu = false;
            }
            app->scene->currentButton->data->Update(dt, 11, 27);
            if (app->input->CheckButton("cancel", KEY_DOWN))
            {
                options = false;
                changeMenu = true;
                app->gui->ResetButtons();
                usingGamepad = true;
            }
		}
		else
		{
            if (changeMenu)
            {
                app->scene->currentButton = app->gui->controls.At(app->gui->controls.Find(btnStart));
                changeMenu = false;
            }
            app->scene->currentButton->data->Update(dt, 1, 4);
		}
	}
	// Calls update for mouse parameters (GUI)
	else
	{
		if (options)
		{
			sldrVolume->Update(dt);
			sldrFx->Update(dt);
			boxFullScreen->Update(dt);
			boxVSync->Update(dt);
			btnBack->Update(dt);
		}
		else
		{
            btnStart->Update(dt);
            btnContinue->Update(dt);
            btnOptions->Update(dt);
            btnExit->Update(dt);
		}
	}

	//LOG("%d", flags);
	return true;
}

bool SceneTitleScreen::Draw()
{
	app->render->background = { 0, 0, 0, 255 };
	app->render->DrawTexture(nooseBG, 810, 0,false,&noose.GetCurrentFrame());
	app->render->DrawTexture(titleCard, titleCardPos.x, titleCardPos.y);
	app->render->DrawRectangle(titleCardPos, 0, 0, 0, (uchar)(255 - (255 * titleAlpha)));

	if (options)
	{
        app->render->DrawRectangle(app->render->camera, 0, 0, 0, 200);
		char titleOptions[TEXT_LEN] = { 0 };
		strcpy_s(titleOptions, TEXT_LEN, "Options");
        app->render->DrawText(buttonFont, titleOptions, /*app->render->camera.x + */((app->render->camera.w - (strlen(titleOptions) * 24)) / 2), 100, 64, 2, white);

        sldrVolume->Draw();
        sldrFx->Draw();
        if (app->win->fullscreenWindow)
            boxFullScreen->checked = true;
        boxFullScreen->Draw();
        if (app->vsync)
            boxVSync->checked = true;
        boxVSync->Draw();
        btnBack->Draw();
	}
	else
	{
        btnStart->Draw();
        btnContinue->Draw();
        btnOptions->Draw();
        btnExit->Draw();
	}
	if (usingGamepad) app->scene->currentButton->data->Draw();
	//char test[64] = { 0 };
	//sprintf_s(test, 64, "SCORE: %03i", 56);
	//app->render->DrawText(dialogueFont, test, 700, 640, 64, 2, { 0, 255, 255, 255 });

	return true;
}

bool SceneTitleScreen::Unload()
{
	app->tex->UnLoad(nooseBG);
	app->tex->UnLoad(titleCard);
	app->audio->UnloadFx(titleFx);
	app->gui->Disable();
	RELEASE(buttonFont);
	app->scene->currentButton = nullptr;

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
        options = true;
		changeMenu = true;
		app->audio->PlayMusic("Audio/Music/Options.ogg");
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
		app->render->ToggleVsync(boxVSync->checked, (Module*)this);
		break;
	case 9: //BACK
        options = false;
		app->audio->PlayMusic("Audio/Music/Title.ogg");
		changeMenu = true;
		app->gui->ResetButtons();
		usingGamepad = true;
		break;
	default:
		break;
	}

	return true;
}