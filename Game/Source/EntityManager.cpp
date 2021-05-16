#include "EntityManager.h"

#include "App.h"
#include "Window.h"
#include "Map.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Fonts.h"
#include "GuiManager.h"
#include "SceneManager.h"
#include "SceneGameplay.h"
#include "SceneCombat.h"
#include "Player.h"
#include "CombatEntity.h"
#include "NPC.h"
#include "Item.h"
#include "PuzzlePieces.h"
#include "Blockers.h"

#include "Defs.h"
#include "Log.h"

#define TITLE_FONT_SIZE 36

EntityManager::EntityManager() : Module()
{
	name.Create("entityManager");
}

EntityManager::~EntityManager()
{}

void EntityManager::Init()
{
	active = false;
}

bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");

	folderAudioFx.Create(config.child("folderAudioFx").child_value());
	folderTexture.Create(config.child("folderTexture").child_value());
	folderMap.Create(config.child("folderMap").child_value());

	return true;
}

bool EntityManager::Start()
{
	// Initializing dialogue bools
	dialogCounter = 0.0f;
	itemPasser.Clear();
	takingItem = false;

	flagsShopkeeper = 0;
	flagsCat = 0;
	flagsSuperhero = 0;
	flagsGrandpa = 0;

	// Loading entities textures

	SString tmp("%s%s", folderTexture.GetString(), "Characters/MCSpriteSheet.png");
	playerTex = app->tex->Load(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "Characters/GrandpaSpriteSheet.png");
	grandpaTex = app->tex->Load(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "Characters/NPCs.png");
	NPCTex = app->tex->Load(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "EnemyAtlas.png");
	enemiesTex = app->tex->Load(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderMap.GetString(), "tutorial_tileset.png");
	puzzleTex = app->tex->Load(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderTexture.GetString(), "GUI/Items.png");
	itemAtlas = app->tex->Load(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "Cat.wav");
	interactCat = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "Grandpa.wav");
	interactGrandpa = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "Hero.wav");
	interactHero = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "Shop.wav");
	interactShop = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "Footstep.wav");
	footstepFx = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "Item.wav");
	itemFx = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "ItemCollected.wav");
	itemCollectedFx = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "Rock.wav");
	rockFx = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "IceRock.wav");
	iceRockFx = app->audio->LoadFx(tmp.GetString());

	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "Solved.wav");
	solvedFx = app->audio->LoadFx(tmp.GetString());

	doLogic = true;

	ListItem<Entity*>* e = entities.start;
	while (e != nullptr)
	{
		ListItem<Entity*>* eNext = e->next;
		DestroyEntity(e->data);
		e = eNext;
	}
	entities.Clear();

	return true;
}

bool EntityManager::CleanUp()
{
	// Destroy entities
	ListItem<Entity*>* e = entities.start;
	ListItem<Entity*>* eNext = nullptr;
	while (e != nullptr)
	{
		if (e->next != nullptr)
		{
			eNext = e->next;
			DestroyEntity(e->data);
			e = eNext;
		}
		else
		{
			DestroyEntity(e->data);
			break;
		}
	}
	entities.Clear();

	// Unloading entities textures
	app->tex->UnLoad(playerTex);
	app->tex->UnLoad(grandpaTex);
	app->tex->UnLoad(enemiesTex);
	app->tex->UnLoad(NPCTex);
	app->tex->UnLoad(puzzleTex);
	app->tex->UnLoad(itemAtlas);

	app->audio->UnloadFx(interactCat);
	app->audio->UnloadFx(interactGrandpa);
	app->audio->UnloadFx(interactHero);
	app->audio->UnloadFx(interactShop);
	app->audio->UnloadFx(footstepFx);
	app->audio->UnloadFx(itemFx);
	app->audio->UnloadFx(itemCollectedFx);
	app->audio->UnloadFx(rockFx);
	app->audio->UnloadFx(iceRockFx);
	app->audio->UnloadFx(solvedFx);

	itemPasser.Clear();

	return true;
}

Entity* EntityManager::CreateEntity(int x, int y, EntityType type, EntityId id, Stats stats, NpcId npcId, Entity* playerPointer, ItemId itemId, int count, PuzzleId puzzleId, BlockerId blockerId)
{
	Entity* ret = nullptr;

	switch (type)
	{
		// Create corresponding type entity
	case EntityType::PLAYER:
	{
		ret = new Player(x, y, footstepFx);
		break;
	}
	case EntityType::COMBAT_ENTITY:
	{
		ret = new CombatEntity(x, y, id, stats);
		break;
	}
	case EntityType::NPC:
	{
		switch (npcId)
		{
		case NpcId::NONE:
			break;
		case NpcId::HERO:
			ret = new Npc(x, y, npcId, playerPointer, interactHero);
			break;
		case NpcId::GRANDPA:
			ret = new Npc(x, y, npcId, playerPointer, interactGrandpa);
			break;
		case NpcId::CAT:
			ret = new Npc(x, y, npcId, playerPointer, interactCat);
			break;
		case NpcId::STORE_GUY:
			ret = new Npc(x, y, npcId, playerPointer, interactShop);
			break;
		default:
			break;
		}
		break;
	}
	case EntityType::ITEM:
	{
		ret = new ItemEntity(x, y, itemId, count);
		break;
	}	
	case EntityType::PUZZLE_PIECE:
	{
		ret = new PuzzlePieces(x, y, puzzleId, playerPointer);
		break;
	}

	case EntityType::BLOCKER:
	{
		ret = new Blockers(x, y, blockerId, playerPointer);
		break;
	}
	}

	// Adds the created entity to the list
	if (ret != nullptr)
	{
		entities.Add(ret);
	}
	
	return ret;
}

bool EntityManager::Update(float dt)
{
	UpdateAll(dt, doLogic);
	return true;
}

bool EntityManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		ListItem<Entity*>* e = entities.start;
		while (e != nullptr)
		{
			if (app->map->data.name == "home.tmx")
			{
				if (e->data->type != EntityType::ITEM && e->data->type != EntityType::PUZZLE_PIECE && e->data->type != EntityType::BLOCKER) e->data->Update(dt); //Should also not draw spikes, just overall entities
			}
			else
			{
				e->data->Update(dt);
			}
			e = e->next;
		}
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	ListItem<Entity*>* e = entities.start;
    SceneGameplay* s = nullptr;
    SceneCombat* cbt = nullptr;
    if (app->scene->current->currentScene == SceneType::GAMEPLAY)
    {
        s = (SceneGameplay*)app->scene->current;
        cbt = s->combatScene;
    }

	Entity* p = nullptr;
	while (e != nullptr)
	{
        if (e->data->type == EntityType::PLAYER) p = e->data;
		if (e->data->pendingToDelete == true)
		{
			DestroyEntity(e->data);
		}
		else
		{
			if (app->scene->current->combat)
			{
                if (cbt != nullptr)
                {
                    if (cbt->waitForTransition == TransitionStatus::END)
                    {
                        if (e->data->type == EntityType::COMBAT_ENTITY)
                        {
                            e->data->Draw();
                        }
                    }
                }
			}
			else
			{
                if (cbt != nullptr)
                {
                    if (cbt->waitForTransition == TransitionStatus::NONE)
                    {
                        if (app->map->data.name == "home.tmx")
                        {
                            if (e->data->type != EntityType::ITEM && e->data->type != EntityType::PUZZLE_PIECE && e->data->type != EntityType::BLOCKER) e->data->Draw(); //Should also not draw spikes, just overall entities
                        }
                        else
                        {
                            e->data->Draw();
                        }
                        if (p != nullptr) p->Draw();
                    }
                }
			}
		}
		e = e->next;
	}
	return true;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	if (entity->collider != nullptr)
	{
		entity->collider->pendingToDelete = true;
	}
	int i = entities.Find(entity);
	RELEASE(entities[i]);
	entities.Del(entities.At(i));
}

void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (int i = 0; i < entities.Count(); i++)
	{
		if (c1 == entities[i]->collider)
		{
			entities[i]->OnCollision(c1, c2);
		}
	}
}

bool EntityManager::OnGuiMouseClickEvent(GuiControl* control)
{
	return true;
}

bool EntityManager::Load(pugi::xml_node& save)
{
	LOG("Loading entities data");
	bool ret = true;

	// Destroy entities
	ListItem<Entity*>* e = entities.start;
	while (e != nullptr)
	{
		ListItem<Entity*>* eNext = e->next;
		if (e->data->id == EntityId::MC || e->data->id == EntityId::VIOLENT) DestroyEntity(e->data);
		e = eNext;
	}

	SceneGameplay* s = nullptr;
	SceneCombat* cbt = nullptr;
	if (app->scene->current->currentScene == SceneType::GAMEPLAY)
	{
		s = (SceneGameplay*)app->scene->current;
		cbt = s->combatScene;
		s->player = nullptr;
	}

	iPoint coords = { 0,0 };
	pugi::xml_node player = save.child("player");
	pugi::xml_node coordsNode = player.child("coordinates");
	coords = { coordsNode.attribute("x").as_int(-1),coordsNode.attribute("y").as_int(-1) };
	s->player = app->entities->CreateEntity(coords.x, coords.y, EntityType::PLAYER, EntityId::NOT_COMBAT, NULL);
	if (app->map->data.name == "home.tmx")
	{
		s->hero = app->entities->CreateEntity(27 * 64 + 15, 12 * 64, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::HERO, s->player);
		s->grandpa = app->entities->CreateEntity((31 * 64) + 48, 7 * 64, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::GRANDPA, s->player);
		s->shopDude = app->entities->CreateEntity(20 * 64 + 10, 33 * 64 + 10, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::STORE_GUY, s->player);
		s->cat = app->entities->CreateEntity(35 * 64 + 10, 15 * 64 - 10, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::CAT, s->player);
	}

	while (!player.empty())
	{
		Stats newChar = (0);
		newChar.hPoints = player.next_sibling().child("stats").attribute("hPoints").as_int(1);
		newChar.hPointsMax = player.next_sibling().child("stats").attribute("hPointsMax").as_int(1);
		newChar.mPoints = player.next_sibling().child("stats").attribute("mPoints").as_int(1);
		newChar.mPointsMax = player.next_sibling().child("stats").attribute("mPointsMax").as_int(1);
		newChar.pAtk = player.next_sibling().child("stats").attribute("pAtk").as_int(0);
		newChar.pDef = player.next_sibling().child("stats").attribute("pDef").as_int(0);
		newChar.mAtk = player.next_sibling().child("stats").attribute("mAtk").as_int(0);
		newChar.mDef = player.next_sibling().child("stats").attribute("mDef").as_int(0);
		newChar.speed = player.next_sibling().child("stats").attribute("speed").as_int(0);
		newChar.stress = player.next_sibling().child("stats").attribute("stress").as_int(0);
		newChar.stressMax = player.next_sibling().child("stats").attribute("stressMax").as_int(0);
		SString name = player.next_sibling().name();
		if (name == "MC")
		{
			cbt->mainChar.character = (CombatEntity*)app->entities->CreateEntity(36, app->render->camera.h - cbt->mainChar.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::MC, newChar);
			cbt->mainChar.hp.Clear();
			cbt->mainChar.hp.Create("HP: %d/%d", cbt->mainChar.character->stats.hPoints, cbt->mainChar.character->stats.hPointsMax);
			cbt->mainChar.mp.Clear();
			cbt->mainChar.mp.Create("MP: %d/%d", cbt->mainChar.character->stats.mPoints, cbt->mainChar.character->stats.mPointsMax);
			cbt->mainChar.stress.Clear();
			cbt->mainChar.stress.Create("ST: %d/%d", cbt->mainChar.character->stats.stress, cbt->mainChar.character->stats.stressMax);
		}
		else if (name == "Grandpa")
		{
			cbt->grandpa.character = (CombatEntity*)app->entities->CreateEntity(cbt->grandpa.box.w + 36, app->render->camera.h - cbt->grandpa.box.h - 25, EntityType::COMBAT_ENTITY, EntityId::VIOLENT, newChar);
			cbt->grandpa.hp.Clear();
			cbt->grandpa.hp.Create("HP: %d/%d", cbt->grandpa.character->stats.hPoints, cbt->grandpa.character->stats.hPointsMax);
			cbt->grandpa.mp.Clear();
			cbt->grandpa.mp.Create("MP: %d/%d", cbt->grandpa.character->stats.mPoints, cbt->grandpa.character->stats.mPointsMax);
		}
		player = player.next_sibling();
	}

	return ret;
}

bool EntityManager::Save(pugi::xml_node& save)
{
	LOG("Saving entities data");
	bool ret = true;

	ListItem<Entity*>* e = entities.start;
	Player* p = nullptr;
	CombatEntity* c = nullptr;
	while (e != nullptr)
	{
		if (e->data->type == EntityType::PLAYER)
		{
			p = (Player*)e->data;
			pugi::xml_node player = save.append_child("player");
			pugi::xml_node coords = player.append_child("coordinates");
			coords.append_attribute("x").set_value(p->entityRect.x);
			coords.append_attribute("y").set_value(p->entityRect.y);
		}
		if (e->data->type == EntityType::COMBAT_ENTITY)
		{
			c = (CombatEntity*)e->data;
			pugi::xml_node character = save.append_child(c->name.GetString());
			pugi::xml_node stats = character.append_child("stats");
			stats.append_attribute("pAtk").set_value(c->stats.pAtk);
			stats.append_attribute("mAtk").set_value(c->stats.mAtk);
			stats.append_attribute("pDef").set_value(c->stats.pDef);
			stats.append_attribute("mDef").set_value(c->stats.mDef);
			stats.append_attribute("hPoints").set_value(c->stats.hPoints);
			stats.append_attribute("hPointsMax").set_value(c->stats.hPointsMax);
			stats.append_attribute("speed").set_value(c->stats.speed);
			stats.append_attribute("mPoints").set_value(c->stats.mPoints);
			stats.append_attribute("mPointsMax").set_value(c->stats.mPointsMax);
			if (c->id == EntityId::MC)
			{
				stats.append_attribute("stress").set_value(c->stats.stress);
				stats.append_attribute("stressMax").set_value(c->stats.stressMax);
			}
		}
		e = e->next;
	}
	return ret;
}