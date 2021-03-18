#include "GuiManager.h"

#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Fonts.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

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

	return true;
}

bool GuiManager::Start()
{
	// Atlas
	SString tmp("%s%s", folderTexture.GetString(), "gui_atlas.png");
	atlas = app->tex->Load(tmp.GetString());

	// Sounds
	tmp.Clear();
	tmp.Create("%s%s", folderAudio.GetString(), "mouse_click.wav");
	clickSoundId = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudio.GetString(), "mouse_hover.wav");
	hoverSoundId = app->audio->LoadFx(tmp.GetString());

	// Default
	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "default_label.png");
	defaultFont = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "default_label_small.png");
	defaultFontSmall = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	// Disabled
	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "disabled_label.png");
	disabledFont = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "disabled_label_small.png");
	disabledFontSmall = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	// Hover
	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "hover_label.png");
	hoverFont = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "hover_label_small.png");
	hoverFontSmall = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	// Pressed
	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "pressed_label.png");
	pressedFont = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "pressed_label_small.png");
	pressedFontSmall = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	// Title
	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "title_label.png");
	titleFont = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "title_label_medium.png");
	titleFontMedium = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "title_label_small.png");
	titleFontSmall = app->fonts->Load(tmp.GetString(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,_-@#*^()[]<>: ", 3);

	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* text, Module* observer, int widthInUnits, bool secondText, const char* text2)
{
	GuiControl* control = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON:
	{
		if (secondText)
		{
			control = new GuiButton(id, bounds, text, text2);
		}
		else
		{
			control = new GuiButton(id, bounds, text);
		}
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
	default: break;
	}

	control->SetSounds(hoverSoundId, clickSoundId);
	control->SetObserver(observer);
	control->SetTexture(atlas);
	if (secondText)
	{
		control->SetFonts(defaultFontSmall, titleFontMedium, hoverFontSmall, pressedFontSmall, disabledFontSmall);
	}
	else
	{
		control->SetFonts(defaultFont, titleFontMedium, hoverFont, pressedFont, disabledFont);
	}

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

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* e = controls.start;
	while (e != nullptr)
	{
		ListItem<GuiControl*>* eNext = e->next;
		DestroyGuiControl(e->data);
		e = eNext;
	}
	controls.Clear();

	app->tex->UnLoad(atlas);

	app->fonts->Unload(defaultFont);
	app->fonts->Unload(disabledFont);
	app->fonts->Unload(hoverFont);
	app->fonts->Unload(pressedFont);
	app->fonts->Unload(titleFont);
	app->fonts->Unload(defaultFontSmall);
	app->fonts->Unload(disabledFontSmall);
	app->fonts->Unload(hoverFontSmall);
	app->fonts->Unload(pressedFontSmall);
	app->fonts->Unload(titleFontMedium);
	app->fonts->Unload(titleFontSmall);

	app->audio->UnloadFx(hoverSoundId);
	app->audio->UnloadFx(clickSoundId);

	return true;
}