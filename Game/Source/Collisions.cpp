#include "Collisions.h"

#include "App.h"
#include "Map.h"
#include "Audio.h"
#include <time.h>
#include "SceneManager.h"
#include "SceneGameplay.h"
#include "SceneDevRoom.h"
#include "SceneCombat.h"
#include "EntityManager.h"
#include "NPC.h" 

#include "Defs.h"
#include "Log.h"

Collisions::Collisions()
{
	name.Create("collisions");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		colliders[i] = nullptr;
	}

	matrix[Collider::Type::AIR][Collider::Type::AIR] = false;
	matrix[Collider::Type::AIR][Collider::Type::SOLID] = false;
	matrix[Collider::Type::AIR][Collider::Type::DOOR] = false;
	matrix[Collider::Type::AIR][Collider::Type::PUZZLE] = false;
	matrix[Collider::Type::AIR][Collider::Type::OTHER] = false;
	matrix[Collider::Type::AIR][Collider::Type::EVENT] = false;
	matrix[Collider::Type::AIR][Collider::Type::ENEMY_SPAWN] = false;
	matrix[Collider::Type::AIR][Collider::Type::INTERACTABLE] = false;
	matrix[Collider::Type::AIR][Collider::Type::DEBUG] = false;
	matrix[Collider::Type::AIR][Collider::Type::PLAYER] = false;

	matrix[Collider::Type::SOLID][Collider::Type::AIR] = false;
	matrix[Collider::Type::SOLID][Collider::Type::SOLID] = false;
	matrix[Collider::Type::SOLID][Collider::Type::DOOR] = false;
	matrix[Collider::Type::SOLID][Collider::Type::PUZZLE] = false;
	matrix[Collider::Type::SOLID][Collider::Type::OTHER] = false;
	matrix[Collider::Type::SOLID][Collider::Type::EVENT] = false;
	matrix[Collider::Type::SOLID][Collider::Type::ENEMY_SPAWN] = false;
	matrix[Collider::Type::SOLID][Collider::Type::INTERACTABLE] = false;
	matrix[Collider::Type::SOLID][Collider::Type::DEBUG] = false;
	matrix[Collider::Type::SOLID][Collider::Type::PLAYER] = false;

	matrix[Collider::Type::DOOR][Collider::Type::AIR] = false;
	matrix[Collider::Type::DOOR][Collider::Type::SOLID] = false;
	matrix[Collider::Type::DOOR][Collider::Type::DOOR] = false;
	matrix[Collider::Type::DOOR][Collider::Type::PUZZLE] = false;
	matrix[Collider::Type::DOOR][Collider::Type::OTHER] = false;
	matrix[Collider::Type::DOOR][Collider::Type::EVENT] = false;
	matrix[Collider::Type::DOOR][Collider::Type::ENEMY_SPAWN] = false;
	matrix[Collider::Type::DOOR][Collider::Type::INTERACTABLE] = false;
	matrix[Collider::Type::DOOR][Collider::Type::DEBUG] = false;
	matrix[Collider::Type::DOOR][Collider::Type::PLAYER] = true;

	matrix[Collider::Type::PUZZLE][Collider::Type::AIR] = false;
	matrix[Collider::Type::PUZZLE][Collider::Type::SOLID] = false;
	matrix[Collider::Type::PUZZLE][Collider::Type::DOOR] = false;
	matrix[Collider::Type::PUZZLE][Collider::Type::PUZZLE] = false;
	matrix[Collider::Type::PUZZLE][Collider::Type::OTHER] = false;
	matrix[Collider::Type::PUZZLE][Collider::Type::EVENT] = false;
	matrix[Collider::Type::PUZZLE][Collider::Type::ENEMY_SPAWN] = false;
	matrix[Collider::Type::PUZZLE][Collider::Type::INTERACTABLE] = false;
	matrix[Collider::Type::PUZZLE][Collider::Type::DEBUG] = false;
	matrix[Collider::Type::PUZZLE][Collider::Type::PLAYER] = true;

	matrix[Collider::Type::OTHER][Collider::Type::AIR] = false;
	matrix[Collider::Type::OTHER][Collider::Type::SOLID] = false;
	matrix[Collider::Type::OTHER][Collider::Type::DOOR] = false;
	matrix[Collider::Type::OTHER][Collider::Type::PUZZLE] = false;
	matrix[Collider::Type::OTHER][Collider::Type::OTHER] = false;
	matrix[Collider::Type::OTHER][Collider::Type::EVENT] = false;
	matrix[Collider::Type::OTHER][Collider::Type::ENEMY_SPAWN] = false;
	matrix[Collider::Type::OTHER][Collider::Type::INTERACTABLE] = false;
	matrix[Collider::Type::OTHER][Collider::Type::DEBUG] = false;
	matrix[Collider::Type::OTHER][Collider::Type::PLAYER] = false;

	matrix[Collider::Type::EVENT][Collider::Type::AIR] = false;
	matrix[Collider::Type::EVENT][Collider::Type::SOLID] = false;
	matrix[Collider::Type::EVENT][Collider::Type::DOOR] = false;
	matrix[Collider::Type::EVENT][Collider::Type::PUZZLE] = false;
	matrix[Collider::Type::EVENT][Collider::Type::OTHER] = false;
	matrix[Collider::Type::EVENT][Collider::Type::EVENT] = false;
	matrix[Collider::Type::EVENT][Collider::Type::ENEMY_SPAWN] = false;
	matrix[Collider::Type::EVENT][Collider::Type::INTERACTABLE] = false;
	matrix[Collider::Type::EVENT][Collider::Type::DEBUG] = false;
	matrix[Collider::Type::EVENT][Collider::Type::PLAYER] = true;

	matrix[Collider::Type::ENEMY_SPAWN][Collider::Type::AIR] = false;
	matrix[Collider::Type::ENEMY_SPAWN][Collider::Type::SOLID] = false;
	matrix[Collider::Type::ENEMY_SPAWN][Collider::Type::DOOR] = false;
	matrix[Collider::Type::ENEMY_SPAWN][Collider::Type::PUZZLE] = false;
	matrix[Collider::Type::ENEMY_SPAWN][Collider::Type::OTHER] = false;
	matrix[Collider::Type::ENEMY_SPAWN][Collider::Type::EVENT] = false;
	matrix[Collider::Type::ENEMY_SPAWN][Collider::Type::ENEMY_SPAWN] = false;
	matrix[Collider::Type::ENEMY_SPAWN][Collider::Type::INTERACTABLE] = false;
	matrix[Collider::Type::ENEMY_SPAWN][Collider::Type::DEBUG] = false;
	matrix[Collider::Type::ENEMY_SPAWN][Collider::Type::PLAYER] = true;

	matrix[Collider::Type::INTERACTABLE][Collider::Type::AIR] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::SOLID] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::DOOR] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::PUZZLE] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::OTHER] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::EVENT] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::ENEMY_SPAWN] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::INTERACTABLE] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::DEBUG] = false;
	matrix[Collider::Type::INTERACTABLE][Collider::Type::PLAYER] = true;

	matrix[Collider::Type::DEBUG][Collider::Type::AIR] = false;
	matrix[Collider::Type::DEBUG][Collider::Type::SOLID] = false;
	matrix[Collider::Type::DEBUG][Collider::Type::DOOR] = false;
	matrix[Collider::Type::DEBUG][Collider::Type::PUZZLE] = false;
	matrix[Collider::Type::DEBUG][Collider::Type::OTHER] = false;
	matrix[Collider::Type::DEBUG][Collider::Type::EVENT] = false;
	matrix[Collider::Type::DEBUG][Collider::Type::ENEMY_SPAWN] = false;
	matrix[Collider::Type::DEBUG][Collider::Type::INTERACTABLE] = false;
	matrix[Collider::Type::DEBUG][Collider::Type::DEBUG] = false;
	matrix[Collider::Type::DEBUG][Collider::Type::PLAYER] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::AIR] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::SOLID] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::DOOR] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PUZZLE] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::OTHER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::EVENT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY_SPAWN] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::INTERACTABLE] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::DEBUG] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;

	srand(time(NULL));
}

Collisions::~Collisions()
{}

void Collisions::Init()
{
	active = false;
}

bool Collisions::Start()
{
	onceNightmare = true;
	return true;
}

bool Collisions::Awake(pugi::xml_node&)
{
	return true;
}

bool Collisions::PreUpdate()
{
	// Delete colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	// Check collisions
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
		{
			continue;
		}

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
			{
				continue;
			}

			c2 = colliders[k];

			if (c1->Intersects(c2->rect) && matrix[c1->type][c2->type])
			{
				if (c1->listener)
				{
					c1->listener->OnCollision(c1, c2);
				}
				if (c2->listener)
				{
					c2->listener->OnCollision(c2, c1);
				}
			}
		}
	}

	return true;
}

bool Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

bool Collisions::Load(pugi::xml_node&)
{
	return true;
}

bool Collisions::Save(pugi::xml_node&)
{
	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;
	// Adds the collider in the first null spot on the array
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}


SDL_Rect Collisions::ResolveCollisions(Collider* collider, iPoint nextFrame,float dt)
{
	if (app->map->data.type != MapTypes::MAPTYPE_UNKNOWN)
	{
		iPoint difference = { nextFrame.x - collider->rect.x,nextFrame.y - collider->rect.y };
		iPoint tilePos = app->map->WorldToMap(nextFrame.x, nextFrame.y);

		iPoint tileWorldPos = app->map->MapToWorld(tilePos.x, tilePos.y);
		iPoint tilePosLowerRight = app->map->WorldToMap(nextFrame.x + collider->rect.w, nextFrame.y + collider->rect.h);

		SDL_Rect tileRect = { tileWorldPos.x,tileWorldPos.y,app->map->data.tileWidth,app->map->data.tileHeight };
		//LOG("Tile Pos = %d %d", tilePos.x, tilePos.y);
		if ((app->map->GetTileProperty(tilePos.x, tilePos.y, "CollisionId") == Collider::Type::SOLID
			|| app->map->GetTileProperty(tilePosLowerRight.x, tilePosLowerRight.y, "CollisionId") == Collider::Type::SOLID)
			&& collider->Intersects(tileRect))
		{
			if (difference.x > 0) nextFrame.x--;
			else if (difference.x < 0) nextFrame.x++;
			if (difference.y > 0) nextFrame.y--;
			else if (difference.y < 0) nextFrame.y++;

			if (difference != iPoint(0, 0)) return ResolveCollisions(collider, nextFrame, dt);
		}
		else if ((app->map->GetTileProperty(tilePos.x, tilePos.y, "CollisionId") == Collider::Type::DOOR
			|| app->map->GetTileProperty(tilePosLowerRight.x, tilePosLowerRight.y, "CollisionId") == Collider::Type::DOOR)
			&& collider->Intersects(tileRect))
		{
			if (app->map->data.name == "tutorial.tmx")
			{
				switch (tilePos.x)
				{
				case 15:
				{
					switch (tilePos.y)
					{
					case 28:
						return { 15 * app->map->data.tileWidth, (39 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
						break;
					case 38:
						return { 15 * app->map->data.tileWidth, (28 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
						break;
					case 50:
						return { 15 * app->map->data.tileWidth, (64 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
						break;
					case 63:
						return { 15 * app->map->data.tileWidth, (50 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
						break;
					case 78:
						return { 15 * app->map->data.tileWidth, (88 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
						break;
					case 87:
						return { 15 * app->map->data.tileWidth, (78 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
						break;
					default:
						break;
					}
				}
				case 21:
				{
					return { (39 * app->map->data.tileWidth) + 1, 14 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
					break;
				}
				case 38:
				{
					switch (tilePos.y)
					{
					case 14:
						return { (20 * app->map->data.tileWidth) - 1, 26 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
						break;
					case 36:
						return { 64 * app->map->data.tileWidth, (89 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
						break;
					case 92:
						return { 64 * app->map->data.tileWidth, (10 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
						break;
					default:
						break;
					}
				}
				case 63:
				{
					switch (tilePos.y)
					{
					case 82:
						break;
					default:
						break;
					}
					break;
				}
				case 64:
				{
					switch (tilePos.y)
					{
					case 9:
						return { 38 * app->map->data.tileWidth, (92 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
						break;
					case 65:
						app->map->CleanUp();
						app->map->Load("home.tmx");
						app->audio->PlayMusic("Assets/Audio/Music/Home.ogg", 0.0f);
						if (app->scene->current->currentScene == SceneType::GAMEPLAY)
						{
							SceneGameplay* s = (SceneGameplay*)app->scene->current;
							s->hero = app->entities->CreateEntity(27 * 64 + 15, 12 * 64, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::HERO, s->player);
							s->shopDude = app->entities->CreateEntity(20 * 64 + 10, 33 * 64 + 10, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::STORE_GUY, s->player);
							s->cat = app->entities->CreateEntity(35 * 64 + 10, 15 * 64 - 10, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::CAT, s->player);
						}
						if (app->map->data.type != MapTypes::MAPTYPE_UNKNOWN) return { (44 * app->map->data.tileWidth) - 1, (30 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
						break;
					case 82:
						break;
					case 90:
						return { 38 * app->map->data.tileWidth, (37 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
						break;
					default:
						break;
					}
				}
				case 65:
				{
					switch (tilePos.y)
					{
					case 82:
						break;
					default:
						break;
					}
				}
				case 66:
				{
					switch (tilePos.y)
					{
					case 82:
						break;
					default:
						break;
					}
				}
				default:
					break;
				}
			}
			else if (app->map->data.name == "home.tmx")
			{
				switch (tilePos.x)
				{
				case 12:
				{
					return { (24 * app->map->data.tileWidth) + 1, 13 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
					break;
				}
				case 23:
				{
					switch (tilePos.y)
					{
					case 7:
						return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
						break;
					case 8:
						return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
						break;
					case 13:
						return { (11 * app->map->data.tileWidth) - 1, 14 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
						break;
					case 14:
						return { (11 * app->map->data.tileWidth) - 1, 14 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
						break;
					case 36:
						return { 39 * app->map->data.tileWidth, 32 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
						break;
					default:
						break;
					}
					break;
				}
				case 24:
				{
					break;
				}
				case 25:
				{
					break;
				}
				case 30:
				{
					return { 38 * app->map->data.tileWidth, (29 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
					break;
				}
				case 31:
				{
					switch (tilePos.y)
					{
					case 4:
						return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
						break;
					case 18:
						return { 38 * app->map->data.tileWidth, (29 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
						break;
					default:
						break;
					}
					break;
				}
				case 32:
				{
					switch (tilePos.y)
					{
					case 4:
						return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
						break;
					case 18:
						return { 38 * app->map->data.tileWidth, (29 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
						break;
					default:
						break;
					}
					break;
				}
				case 33:
				{
					return { 38 * app->map->data.tileWidth, (29 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
					break;
				}
				case 38:
				{
					return { 32 * app->map->data.tileWidth, (17 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
					break;
				}
				case 39:
				{
					switch (tilePos.y)
					{
					case 7:
						return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
						break;
					case 8:
						return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
						break;
					case 13:
						return { (53 * app->map->data.tileWidth) + 1, 13 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
						break;
					case 14:
						return { (53 * app->map->data.tileWidth) + 1, 14 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
						break;
					case 34:
						return { 23 * app->map->data.tileWidth, (35 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
						break;
					default:
						break;
					}
					break;
				}
				case 40:
				{
					switch (tilePos.y)
					{
					case 34:
						return { 23 * app->map->data.tileWidth, (35 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
						break;
					default:
						break;
					}
					break;
				}
				case 45:
				{
					switch (tilePos.y)
					{
					case 30:
						app->map->CleanUp();
						app->map->Load("tutorial.tmx");
						if (app->scene->current->currentScene == SceneType::GAMEPLAY)
						{
							SceneGameplay* s = (SceneGameplay*)app->scene->current;
							app->entities->DestroyEntity(s->hero);
							app->entities->DestroyEntity(s->shopDude);
							app->entities->DestroyEntity(s->cat);
						}
						app->audio->PlayMusic("Assets/Audio/Music/Tutorial.ogg", 0.0f);
						if (app->map->data.type != MapTypes::MAPTYPE_UNKNOWN) return { 64 * app->map->data.tileWidth, (66 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
						break;
					default:
						break;
					}
					break;
				}
				case 52:
				{
					switch (tilePos.y)
					{
					case 13:
						return { (39 * app->map->data.tileWidth) - 1, 13 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
						break;
					case 14:
						return { (39 * app->map->data.tileWidth) - 1, 14 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
						break;
					default:
						break;
					}
					break;
				}
				default:
					break;
				}
			}
		}
		else if ((app->map->GetTileProperty(tilePos.x, tilePos.y, "CollisionId") == Collider::Type::ENEMY_SPAWN
			|| app->map->GetTileProperty(tilePosLowerRight.x, tilePosLowerRight.y, "CollisionId") == Collider::Type::ENEMY_SPAWN)
			&& collider->Intersects(tileRect) && !app->scene->current->combat)
		{
			if (difference != iPoint(0, 0)) app->scene->current->combatCooldown -= dt;
			if (app->scene->current->combatCooldown <= 0)
			{
				int e = rand() % 100 + 1;
				if (e <= 30)
				{
					app->scene->current->combat = true;
					app->scene->current->enteringCombat = true;
				}
				else app->scene->current->combatCooldown = 1.0f;

			}
		}
		else if ((app->map->GetTileProperty(tilePos.x, tilePos.y, "CollisionId") == Collider::Type::EVENT
			|| app->map->GetTileProperty(tilePosLowerRight.x, tilePosLowerRight.y, "CollisionId") == Collider::Type::EVENT)
			&& collider->Intersects(tileRect) && !app->scene->current->combat)
		{
			if (app->map->data.name == "tutorial.tmx")
			{
				switch (tilePos.y)
				{
				case 82:
					LOG("EVENT TILE");
					if (onceNightmare)
					{
						onceNightmare = false;
						app->scene->current->combat = true;
						app->scene->current->enteringCombat = true;

						if (app->scene->current->enteringCombat)
						{
							app->scene->current->enteringCombat = false;
							if (app->scene->current->currentScene == SceneType::GAMEPLAY)
							{
								SceneGameplay* g = (SceneGameplay*)app->scene->current;
								g->combatScene->Start(EntityId::STRESSING_SHADOW, EntityId::NIGHTMARE, EntityId::STRESSING_SHADOW);
							}
							else if (app->scene->current->currentScene == SceneType::DEV_ROOM)
							{
								SceneDevRoom* d = (SceneDevRoom*)app->scene->current;
								d->combatScene->Start(EntityId::STRESSING_SHADOW, EntityId::NIGHTMARE, EntityId::STRESSING_SHADOW);
							}
						}
					}
					break;
				default:
					break;
				}
			}
		}

	}
	return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
}

void Collider::SetPos(int x, int y, int w, int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

bool Collider::Intersects(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w
		&& rect.x + rect.w > r.x
		&& rect.y < r.y + r.h 
		&& rect.h + rect.y > r.y);
}