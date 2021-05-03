#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

class GuiButton : public GuiControl
{
public:
	// Constructor
	GuiButton(uint32 id, SDL_Rect bounds, const char* text);

	// Destructor
	virtual ~GuiButton();

	// Called each loop iteration
	bool Update(float dt);
	bool Update(float dt, int minId, int maxId); // Update for gamepad
	

	// Blit
	bool Draw(int cPosX = 0, int cPosY = 0);

protected:
	// GUI Button specific properties
	SDL_Rect disabled;
	SDL_Rect normal;
	SDL_Rect focused;
	SDL_Rect pressed;

	SDL_Rect disabledSmall;
	SDL_Rect normalSmall;
	SDL_Rect focusedSmall;
	SDL_Rect pressedSmall;

	SDL_Rect disabledLeft;
	SDL_Rect normalLeft;
	SDL_Rect focusedLeft;
	SDL_Rect pressedLeft;

	SDL_Rect disabledRight;
	SDL_Rect normalRight;
	SDL_Rect focusedRight;
	SDL_Rect pressedRight;

	bool hoverPlay;
	bool clickPlay;
};

#endif // __GUIBUTTON_H__
