#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"
#include "List.h"
#include "Font.h"

class GuiManager : public Module
{
public:
	// Constructor
	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called when program is executed
	void Init();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Create/Destroy control
	GuiControl* CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* text, int textSize, Scene* observer, int widthInUnits = 0, SDL_Texture* tex = nullptr);
	void DestroyGuiControl(GuiControl* entity);

	void ResetButtons();

public:
	char folderTexture[TEXT_LEN] = { 0 };
	char folderAudio[TEXT_LEN] = { 0 };
	char folderFonts[TEXT_LEN] = { 0 };

	SDL_Texture* atlas = nullptr;

	// Fonts & sounds

	Font* guiFontId = nullptr;

	int clickSoundId = -1;
	int hoverSoundId = -1;

	List<GuiControl*> controls;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif // __GUIMANAGER_H__
