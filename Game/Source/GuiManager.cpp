#include "GuiManager.h"

#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Fonts.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "GuiImageButton.h"

#include "Log.h"

GuiManager::GuiManager() : Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager()
{}

void GuiManager::Init()
{
	active = false;
}

bool GuiManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Gui Manager");

	folderTexture.Create(config.child("folderTexture").child_value());
	folderAudio.Create(config.child("folderAudio").child_value());
	folderFonts.Create(config.child("folderFonts").child_value());

	return true;
}

bool GuiManager::Start()
{
	// Load texture fonts & fx
	SString tmp("%s%s", folderTexture.GetString(), "gui.png");
	atlas = app->tex->Load(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudio.GetString(), "Click.wav");
	clickSoundId = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudio.GetString(), "Hover.wav");
	hoverSoundId = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderFonts.GetString(), "ButtonFont.xml");
	guiFontId = new Font(tmp.GetString());

	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* text, int textSize, Scene* observer, int widthInUnits, SDL_Texture* tex)
{
	GuiControl* control = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON:
	{
		control = new GuiButton(id, bounds, text);
		break;
	}
	case GuiControlType::CHECKBOX:
	{
		control = new GuiCheckBox(id, bounds, text);
		break;
	}
	case GuiControlType::SLIDER:
	{
		control = new GuiSlider(id, bounds, widthInUnits, text);
		break;
	}
	case GuiControlType::IMAGEBUTTON:
	{
		control = new GuiImageButton(id, bounds, text, tex);
		break;
	}
	default: break;
	}

	control->SetSounds(hoverSoundId, clickSoundId);
	control->SetObserver(observer);
	control->textSize = textSize;
	control->SetTexture(atlas);
	control->SetFonts(guiFontId);

	// Adds the created entity to the list
	if (control != nullptr)
	{
		controls.Add(control);
	}

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	int i = controls.Find(entity);
	delete controls[i];
	controls.Del(controls.At(i));
}

void GuiManager::ResetButtons()
{
	ListItem<GuiControl*>* currentControlList = controls.start;
	while (currentControlList->next != nullptr)
	{
		currentControlList->data->state = GuiControlState::NORMAL;
		if (currentControlList->next != nullptr) currentControlList = currentControlList->next;
	}
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* e = controls.start;
	ListItem<GuiControl*>* eNext;
	while (e != nullptr)
	{
		if (e->next != nullptr)
		{
			ListItem<GuiControl*>* eNext = e->next;
			DestroyGuiControl(e->data);
			e = eNext;
		}
		else
		{
			DestroyGuiControl(e->data);
			break;
		}
	}
	controls.Clear();

	// Unload FONTS
	app->tex->UnLoad(atlas);
	RELEASE(guiFontId);

	if (hoverSoundId != -1)
	{
		app->audio->UnloadFx(hoverSoundId);
		hoverSoundId = -1;
	}
	if (clickSoundId != -1)
	{
		app->audio->UnloadFx(clickSoundId);
		clickSoundId = -1;
	}

	return true;
}