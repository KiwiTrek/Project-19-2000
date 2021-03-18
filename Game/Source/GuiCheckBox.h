#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

class GuiCheckBox : public GuiControl
{
public:
	// Constructor
	GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text);

	// Destructor
	virtual ~GuiCheckBox();

	// Called each loop iteration
	bool Update(float dt);

	// Blit
	bool Draw(int cPosX = 0, int cPosY = 0);

public:
	bool checked;

private:
	// GUI Checkbox specific properties
	SDL_Rect disabled;
	SDL_Rect normal;
	SDL_Rect focused;
	SDL_Rect pressed;

	SDL_Rect disabledChecked;
	SDL_Rect normalChecked;
	SDL_Rect focusedChecked;
	SDL_Rect pressedChecked;
};

#endif // __GUICHECKBOX_H__
