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
	//SDL_Rect disabled;
	//SDL_Rect normal;
	//SDL_Rect focused;
	//SDL_Rect pressed;

	//SDL_Rect disabledSmall;
	//SDL_Rect normalSmall;
	//SDL_Rect focusedSmall;
	//SDL_Rect pressedSmall;

	//bool hoverPlay;
	//bool clickPlay;
	SString count;
	SDL_Texture* tex;
	SDL_Rect sec;
	ItemId itemId;
};

#endif // __GUIIMAGEBUTTON_H__