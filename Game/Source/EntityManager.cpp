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
#include "Scene.h"
#include "Player.h"
#include "CombatEntity.h"

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
	// Loading entities textures
	//playerTex = app->tex->Load("Assets/Textures/playerTest.png");
	playerTex = app->tex->Load("Assets/Textures/Characters/MCSpriteSheet.png");

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
	while (e != nullptr)
	{
		ListItem<Entity*>* eNext = e->next;
		DestroyEntity(e->data);
		e = eNext;
	}
	entities.Clear();

	// Unloading entities textures
	app->tex->UnLoad(playerTex);

	return true;
}

Entity* EntityManager::CreateEntity(int x, int y, EntityType type, EntityId id, Stats stats/*, Entity* playerPointer, EnemyType eType*/)
{
	Entity* ret = nullptr;

	switch (type)
	{
		// Create corresponding type entity
	case EntityType::PLAYER:
	{
		ret = new Player(x, y);
		break;
	}
	case EntityType::COMBAT_ENTITY:
	{
		ret = new CombatEntity(x, y, id, stats);
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
			e->data->Update(dt);
			e = e->next;
		}
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	ListItem<Entity*>* e = entities.start;
	while (e != nullptr)
	{
		if (e->data->pendingToDelete == true)
		{
			DestroyEntity(e->data);
		}
		else
		{
			e->data->Draw();
		}
		e = e->next;
	}

	return true;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	//if (entity->collider != nullptr)
	//{
	//	entity->collider->pendingToDelete = true;
	//}
	int i = entities.Find(entity);
	delete entities[i];
	entities.Del(entities.At(i));
}

void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	/*for (int i = 0; i < entities.Count(); i++)
	{
		if (c1 == entities[i]->collider)
		{
			entities[i]->OnCollision(c1, c2);
		}
	}*/
}

bool EntityManager::OnGuiMouseClickEvent(GuiControl* control)
{
	
	return true;
}

bool EntityManager::Load(pugi::xml_node& save)
{
	LOG("Loading entities data");
	bool ret = true;

	//// Clear the list
	//ListItem<Entity*>* e = entities.start;
	//while (e != nullptr)
	//{
	//	ListItem<Entity*>* eNext = e->next;
	//	DestroyEntity(e->data);
	//	e = eNext;
	//}
	//entities.Clear();

	//// Initialize the entity variables
	//int x = 0;
	//int y = 0;
	//EntityType type = EntityType::UNKNOWN;
	//EnemyType eType = EnemyType::NO_TYPE;
	//Player* pp = nullptr;

	//for (pugi::xml_node entity = save.child("entity"); entity && ret; entity = entity.next_sibling("entity"))
	//{
	//	x = entity.child("coordinates").attribute("x").as_int();
	//	y = entity.child("coordinates").attribute("y").as_int();
	//	switch (entity.child("type").attribute("value").as_int())
	//	{
	//	case 0:
	//		type = EntityType::PLAYER;
	//		break;
	//	case 1:
	//		type = EntityType::ENEMY;
	//		break;
	//	case 2:
	//		type = EntityType::COIN;
	//		break;
	//	case 3:
	//		type = EntityType::FRUIT;
	//		break;
	//	default:
	//		type = EntityType::UNKNOWN;
	//		break;
	//	}
	//	switch (entity.child("eType").attribute("value").as_int())
	//	{
	//	case 1:
	//		eType = EnemyType::GROUND;
	//		break;
	//	case 2:
	//		eType = EnemyType::FLYING;
	//		break;
	//	default:
	//		eType = EnemyType::NO_TYPE;
	//		break;
	//	}

	//	if (type == EntityType::PLAYER)
	//	{
	//		pp = (Player*)CreateEntity(x, y, type, nullptr, eType);
	//		pp->lives = entity.child("lives").attribute("value").as_int(3);
	//		pp->firstCheckpoint = entity.child("checkpoint").attribute("value").as_bool();
	//		app->scene->player = pp;
	//	}
	//	else
	//	{
	//		CreateEntity(x, y, type, pp, eType);
	//	}
	//}
	return ret;
}

bool EntityManager::Save(pugi::xml_node& save)
{
	LOG("Saving entities data");
	bool ret = true;

	/*ListItem<Entity*>* e = entities.start;
	while (e != nullptr)
	{
		pugi::xml_node entity = save.append_child("entity");
		pugi::xml_node entityCoords = entity.append_child("coordinates");
		entityCoords.append_attribute("x").set_value(e->data->entityRect.x);
		entityCoords.append_attribute("y").set_value(e->data->entityRect.y);
		int type = 0;
		switch (e->data->type)
		{
		case EntityType::PLAYER:
		{
			Player* pp = (Player*)e->data;
			entity.append_child("lives").append_attribute("value").set_value(pp->lives);
			entity.append_child("checkpoint").append_attribute("value").set_value(pp->firstCheckpoint);
			type = 0;
			break;
		}
		case EntityType::ENEMY:
		{
			type = 1;
			break;
		}
		case EntityType::COIN:
		{
			type = 2;
			break;
		}
		case EntityType::FRUIT:
		{
			type = 3;
			break;
		}
		default:
		{
			break;
		}
		}
		entity.append_child("type").append_attribute("value").set_value(type);
		int eType = 0;
		switch (e->data->eType)
		{
		case EnemyType::GROUND:
			eType = 1;
			break;
		case EnemyType::FLYING:
			eType = 2;
			break;
		default:
			break;
		}
		entity.append_child("eType").append_attribute("value").set_value(eType);

		e = e->next;
	}*/
	return ret;
}