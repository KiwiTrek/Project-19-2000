#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "GuiManager.h"
#include "Map.h"
#include "SceneLogo.h"
#include "SceneTitleScreen.h"
#include "SceneGameplay.h"
#include "SceneEnding.h"
#include "SceneCombat.h"

#include "Defs.h"
#include "Log.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager() : Module()
{
	name.Create("scenemanager");

	onTransition = false;
	fadeOutCompleted = false;
	continueLoadRequest = false;

	transitionAlpha = 0.0f;;
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Manager");
	bool ret = true;

	folderAudioFx.Create(config.child("folderAudioFx").child_value());
	folderTexture.Create(config.child("folderTexture").child_value());
	folderFonts.Create(config.child("folderFonts").child_value());

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	current = new SceneLogo();
	current->Load();

	next = nullptr;

	onTransition = false;
	fadeOutCompleted = false;
	continueLoadRequest = false;

	transitionAlpha = 0.0f;

	return true;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	if (!onTransition)
	{
		//if (app->render->debug)
		//{
		//	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) app->render->camera.y -= 1;
		//	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) app->render->camera.y += 1;
		//	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) app->render->camera.x -= 1;
		//	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) app->render->camera.x += 1;
		//}

		current->Update(dt);
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f)
			{
				transitionAlpha = 1.0f;

				current->Unload();	// Unload current screen
				next->Load();	// Load next screen

				if (current != nullptr)
				{
					RELEASE(current);	// Free current pointer
					current = nullptr;
				}
				current = next;		// Assign next pointer
				next = nullptr;

				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;
			}
		}
		else // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && current->currentScene != SceneType::GAMEPLAY) return false;

	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	bool ret = true;

	// Draw current scene
	current->Draw();

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
		case SceneType::LOGO:
			next = new SceneLogo();
			next->currentScene = SceneType::LOGO;
			break;
		case SceneType::TITLE_SCREEN:
			next = new SceneTitleScreen();
			next->currentScene = SceneType::TITLE_SCREEN;
			break;
		case SceneType::GAMEPLAY:
			next = new SceneGameplay();
			next->currentScene = SceneType::GAMEPLAY;
			break;
		case SceneType::ENDING:
			next = new SceneEnding();
			next->currentScene = SceneType::ENDING;
			break;
		default:
			break;
		}

		current->transitionRequired = false;
	}

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing scene");

	current->Unload();
	next = nullptr;

	return true;
}

bool SceneManager::Save(pugi::xml_node& save)
{
	LOG("Saving items data");
	bool ret = true;

	SceneGameplay* s = (SceneGameplay*)current;
	ListItem<Item*>* i = s->combatScene->items.start;
	while (i != nullptr)
	{
		pugi::xml_node item = save.append_child("item");
		item.append_attribute("id").set_value((int)i->data->id);
		pugi::xml_node effect = item.append_child("effect");
		effect.append_attribute("name").set_value(i->data->effect.attackName.GetString());
		effect.append_attribute("turns").set_value(i->data->effect.turns);
		item.append_attribute("count").set_value(i->data->count);
		i = i->next;
	}

	return ret;
}

bool SceneManager::Load(pugi::xml_node& save)
{
	LOG("Loading items data");
	bool ret = true;

	SceneGameplay* s = (SceneGameplay*)current;
	pugi::xml_node item = save.child("item");
	s->combatScene->items.Clear();

	while (!item.empty())
	{
		pugi::xml_node effect = item.child("effect");
		Item* i = nullptr;
		ItemId id = ItemId::NONE;
		Attack e;
		int turns = effect.attribute("turns").as_int();
		int count = item.attribute("count").as_int();
		SDL_Rect sec = { 288,416,32,32 };
		GuiControl* button = nullptr;

		switch (item.attribute("id").as_int())
		{
		case 1:
			id = ItemId::HP_POTION;
			sec = { 5 * 32,8 * 32,32,32 };
			break;
		case 2:
			id = ItemId::MANA_POTION;
			sec = { 8 * 32,8 * 32,32,32 };
			break;
		case 3:
			id = ItemId::ELIXIR;
			sec = { 9 * 32,8 * 32,32,32 };
			break;
		case 4:
			id = ItemId::GRANDMA_STEW;
			sec = { 8 * 32,7 * 32,32,32 };
			break;
		case 5:
			id = ItemId::BOTTLED_SMITE;
			sec = { 6 * 32,8 * 32,32,32 };
			break;
		case 6:
			id = ItemId::HAPPILLS;
			sec = { 4 * 32,8 * 32,32,32 };
			break;
		case 7:
			id = ItemId::PHYS_BUFFER;
			sec = { 3 * 32,7 * 32,32,32 };
			break;
		case 8:
			id = ItemId::MAGIC_BUFFER;
			sec = { 7 * 32,8 * 32,32,32 };
			break;
		default:
			break;
		}

		e = Attack(effect.attribute("name").as_string());
		e.turns = turns;
		i = new Item(id, e, count);
		i->UpdateCountText();
		i->texSec = sec;
		i->button = button;
		s->combatScene->items.Add(i);
		item = item.next_sibling();
	}

	return true;
}