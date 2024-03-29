#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "pugixml.hpp"

#define CONFIG_FILENAME "config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Map;
class AssetsManager;
class EntityManager;
class Collisions;
class GuiManager;
class QuestManager;
class ParticleSystem;

class SceneManager;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// Requests Savefile
	bool CheckSaveFile();

	// Requests Load/Save
	void LoadRequest();
	void SaveRequest();

	// Requests Cap
	void CapRequest();

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Save/Load inside an XML file
	bool LoadGame();
	bool SaveGame();

	// Changes framerate cap
	bool ChangeCap();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	AssetsManager* assetsManager;
	Textures* tex;
	Audio* audio;
	Map* map;
	EntityManager* entities;
	Collisions* collisions;
	GuiManager* gui;

	QuestManager* quests;
	ParticleSystem* particles;

	SceneManager* scene;

	bool vsync = true;
	bool exitRequest = false;

	bool saveRequest;// public so I can draw Miku's saving game dialog
private:

	int argc;
	char** args;
	char title[TEXT_LEN] = { 0 };
	char organization[TEXT_LEN] = { 0 };

	List<Module *> modules;

	bool loadRequest;
	bool capRequest;
	pugi::xml_document saveFile;
	pugi::xml_node save;

	// Frame variables
	PerfTimer pTimer;
	uint64 frameCount = 0;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;
	float cappedMs = -1;
	int cap = 0;
};

extern App* app;

#endif	// __APP_H__