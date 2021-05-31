#ifndef __GUIIMAGEBUTTON_H__
#define __GUIIMAGEBUTTON_H__

#include "GuiButton.h"
#include "Item.h"

struct SDL_Texture;

class GuiImageButton : public GuiButton
{
public:
	// Constructor
	GuiImageButton(uint32 id, SDL_Rect bounds, const char* text, SDL_Texture* tex);

	// Destructor
	virtual ~GuiImageButton();

	// Called each loop iteration
	bool Update(float dt);
	bool Update(float dt, int minId, int maxId); // Update for gamepad


	// Blit
	bool Draw(int cPosX = 0, int cPosY = 0);

public:
	// GUI Image Button specific properties
	char count[TEXT_LEN] = { 0 };
	SDL_Texture* tex;
	SDL_Rect sec;
	ItemId itemId;
};

#endif // __GUIIMAGEBUTTON_H__