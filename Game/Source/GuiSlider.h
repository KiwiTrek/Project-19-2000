#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

class GuiSlider : public GuiControl
{
public:
	// Constructor
	GuiSlider(uint32 id, SDL_Rect bounds, int widthInUnits, const char* text);

	// Destructor
	virtual ~GuiSlider();

	// Called each loop iteration
	bool Update(float dt);
	bool Update(float dt, int minId, int maxId);

	// Blit
	bool Draw(int cPosX = 0, int cPosY = 0);

public:
	SDL_Rect limits;

	int value = -1;
	int maxValue = -1;

private:
	// GUI Slider specific properties
	int widthInUnits = 0;

	SDL_Rect normal;
	SDL_Rect focused;
	SDL_Rect pressed;
	SDL_Rect disabled;

	SDL_Rect normalLimitsBegin;
	SDL_Rect disabledLimitsBegin;
};

#endif // __GUISLIDER_H__
