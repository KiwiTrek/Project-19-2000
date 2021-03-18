#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

class GuiButton : public GuiControl
{
public:
	// Constructor
	GuiButton(uint32 id, SDL_Rect bounds, const char* text);
	GuiButton(uint32 id, SDL_Rect bounds, const char* text, const char* secondText);

	// Destructor
	virtual ~GuiButton();

	// Called each loop iteration
	bool Update(float dt);

	// Blit
	bool Draw(int cPosX = 0, int cPosY = 0);

private:
	// Second text variables
	SString secondText;
	int offsetTextSecond;
	bool secondaryText;

	// GUI Button specific properties
	SDL_Rect disabled;
	SDL_Rect normal;
	SDL_Rect focused;
	SDL_Rect pressed;
};

#endif // __GUIBUTTON_H__
