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
    NONE,
	BUTTON,
	CHECKBOX,
	SLIDER,
	IMAGEBUTTON
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
    GuiControl(GuiControlType type, uint32 id) : type(type), id(id), state(GuiControlState::NORMAL), white({ 255,255,255,255 }), black({ 0,0,0,255 })
    {
    }

    GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) : type(type), state(GuiControlState::NORMAL), bounds(bounds), text(text), white({ 255,255,255,255 }), black({ 0,0,0,255 })
    {
        texture = NULL;
        guiFont = nullptr;
        hover = -1;
        click = -1;
    }

    // Called each loop iteration
    virtual bool Update(float dt)
    {
        return true;
    }
    virtual bool Update(float dt, int minId, int maxId)
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
    void SetFonts(Font* defaultId)
    {
        guiFont = defaultId;
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
    uint32 id = 0;
    GuiControlType type = GuiControlType::NONE;
    GuiControlState state = GuiControlState::NORMAL;
    SString text;
    int textSize = 0;
    int offsetText = 0;

    // Position and size
    SDL_Rect bounds = { 0,0,0,0 };

	// Texture atlas reference
	SDL_Texture* texture = nullptr;
	SDL_Color white;
	SDL_Color black;

	// Fonts
	Font* guiFont = nullptr;

	// Sounds
	int click = -1;
	int hover = -1;

	// Observer scene
	Scene* observer = nullptr;
};

#endif // __GUICONTROL_H__
