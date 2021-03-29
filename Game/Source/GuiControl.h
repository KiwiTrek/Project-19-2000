#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Scene.h"
#include "Point.h"
#include "SString.h"
#include "SDL.h"

#include "App.h"
#include "Audio.h"

enum class GuiControlType
{
	BUTTON,
	CHECKBOX,
	SLIDER,
};

enum class GuiControlState
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
};

class GuiControl
{
public:
	// Constructors
	GuiControl(GuiControlType type, uint32 id) : type(type), id(id), state(GuiControlState::NORMAL)
	{}

	GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) : type(type), state(GuiControlState::NORMAL), bounds(bounds), text(text)
	{
		texture = NULL;
		guiFont = nullptr;
	/*	titleFont = -1;
		hoverFont = -1;
		pressedFont = -1;
		disabledFont = -1;*/

		hover = -1;
		click = -1;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Blit
	virtual bool Draw() const
	{
		return true;
	}

	// Sets texture
	void SetTexture(SDL_Texture* tex)
	{
		texture = tex;
	}

	// Sets all fonts used in gui
	void SetFonts(Font* defaultId/*, int titleId, int hoverId, int pressedId, int disabledId*/)
	{
		guiFont = defaultId;
		//titleFont = titleId;
		//hoverFont = hoverId;
		//pressedFont = pressedId;
		//disabledFont = disabledId;
	}

	void SetSounds(int hoverSoundId, int clickSoundId)
	{
		hover = hoverSoundId;
		click = clickSoundId;
	}

	// Sets the gui control observer
	void SetObserver(Scene* scene)
	{
		observer = scene;
	}

	// Notifies the gui control observer
	void NotifyObserver()
	{
		observer->OnGuiMouseClickEvent(this);
	}

public:
	uint32 id;
	GuiControlType type;
	GuiControlState state;
	SString text;
	int textSize;
	int offsetText;

	// Position and size
	SDL_Rect bounds;

	// Texture atlas reference
	SDL_Texture* texture;

	// Fonts
	Font* guiFont;
	//int titleFont;
	//int hoverFont;
	//int pressedFont;
	//int disabledFont;

	// Sounds
	int click;
	int hover;

	// Observer scene
	Scene* observer;
};

#endif // __GUICONTROL_H__
