#include "Collisions.h"

#include "App.h"
#include "Input.h"
#include "Map.h"
#include "Audio.h"
#include <time.h>
#include "SceneManager.h"
#include "SceneGameplay.h"
#include "SceneCombat.h"
#include "EntityManager.h"
#include "NPC.h" 
#include "QuestManager.h"
#include "PuzzlePieces.h"


#include "Defs.h"
#include "Log.h"

Collisions::Collisions()
{
	memset(name, 0, TEXT_LEN);
	strcpy_s(name,TEXT_LEN, "collisions");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		colliders[i] = nullptr;
	}

	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::MOVEABLE] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::PLAYER] = false;
	matrix[(uint)Collider::Type::AIR][(uint)Collider::Type::SOLID_ROCK] = false;

	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::MOVEABLE] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::SOLID][(uint)Collider::Type::PLAYER] = true;

	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::MOVEABLE] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::PLAYER] = true;
	matrix[(uint)Collider::Type::DOOR][(uint)Collider::Type::SOLID_ROCK] = false;

	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::MOVEABLE] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::PLAYER] = false;
	matrix[(uint)Collider::Type::PUZZLE][(uint)Collider::Type::SOLID_ROCK] = false;

	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::MOVEABLE] = true;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::PLAYER] = true;
	matrix[(uint)Collider::Type::BUTTON][(uint)Collider::Type::SOLID_ROCK] = false;

	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::BUTTON] = true;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::MOVEABLE] = true;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::PLAYER] = true;
	matrix[(uint)Collider::Type::MOVEABLE][(uint)Collider::Type::SOLID_ROCK] = false;

	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::MOVEABLE] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::PLAYER] = false;
	matrix[(uint)Collider::Type::OTHER][(uint)Collider::Type::SOLID_ROCK] = false;

	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::MOVEABLE] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::PLAYER] = true;
	matrix[(uint)Collider::Type::EVENT][(uint)Collider::Type::SOLID_ROCK] = false;

	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::MOVEABLE] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::PLAYER] = true;
	matrix[(uint)Collider::Type::ENEMY_SPAWN][(uint)Collider::Type::SOLID_ROCK] = false;

	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::MOVEABLE] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::PLAYER] = true;
	matrix[(uint)Collider::Type::INTERACTABLE][(uint)Collider::Type::SOLID_ROCK] = false;

	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::MOVEABLE] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::PLAYER] = false;
	matrix[(uint)Collider::Type::DEBUG][(uint)Collider::Type::SOLID_ROCK] = false;

	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::SOLID] = true;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::DOOR] = true;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::BUTTON] = true;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::MOVEABLE] = true;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::EVENT] = true;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::ENEMY_SPAWN] = true;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::INTERACTABLE] = true;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::PLAYER] = false;
	matrix[(uint)Collider::Type::PLAYER][(uint)Collider::Type::SOLID_ROCK] = true;

	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::AIR] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::SOLID] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::DOOR] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::PUZZLE] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::BUTTON] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::MOVEABLE] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::OTHER] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::EVENT] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::ENEMY_SPAWN] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::INTERACTABLE] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::DEBUG] = false;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::PLAYER] = true;
	matrix[(uint)Collider::Type::SOLID_ROCK][(uint)Collider::Type::SOLID_ROCK] = false;

}

Collisions::~Collisions()
{}

void Collisions::Init()
{
	active = false;
}

bool Collisions::Awake(pugi::xml_node& config)
{
	strcpy_s(folderAudioFx, TEXT_LEN, config.child("folderAudioFx").child_value());
	return true;
}

bool Collisions::Start()
{
	onceNightmare = true;
	saveOnce = false;
	onceDoor = false;

	char tmp[TEXT_LEN] = { 0 };
	sprintf_s(tmp,TEXT_LEN, "%s%s", folderAudioFx, "Save.wav");
	saveFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", folderAudioFx, "Toilet.wav");
	toiletFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", folderAudioFx, "TutorialWarp.wav");
	tutorialWarpFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", folderAudioFx, "Door.wav");
	doorFx = app->audio->LoadFx(tmp);

	memset(tmp, 0, TEXT_LEN);
	sprintf_s(tmp, TEXT_LEN, "%s%s", folderAudioFx, "WakingUp.wav");
	wakingUpFx = app->audio->LoadFx(tmp);

	return true;
}

bool Collisions::PreUpdate()
{
	// Delete colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			RELEASE(colliders[i]);
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

			if (c1->Intersects(c2->rect) && matrix[(uint)c1->type][(uint)c2->type])
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
		RELEASE(colliders[i]);
	}

	app->audio->UnloadFx(saveFx);
	app->audio->UnloadFx(toiletFx);
	app->audio->UnloadFx(tutorialWarpFx);
	app->audio->UnloadFx(doorFx);
	app->audio->UnloadFx(wakingUpFx);

	return true;
}

bool Collisions::Load(pugi::xml_node& save)
{
	bool ret = true;
	pugi::xml_attribute flags = save.child("flags").attribute("nightmareBoss");
	if (flags.empty()) ret = false;
	else
	{
		onceNightmare = flags.as_bool(true);
	}
	return ret;
}

bool Collisions::Save(pugi::xml_node& save)
{
	bool ret = true;
	ret = save.append_child("flags").append_attribute("nightmareBoss").set_value(onceNightmare);
	return ret;
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
	if (app->map->data.type != MapTypes::MAPTYPE_UNKNOWN && nextFrame.x >= 0 && nextFrame.y >= 0)
	{
		if (nextFrame.x < app->map->data.width * 64 && nextFrame.y < app->map->data.height * 64)
		{
			iPoint difference = { nextFrame.x - collider->rect.x,nextFrame.y - collider->rect.y};
			iPoint tilePos = app->map->WorldToMap(nextFrame.x+3, nextFrame.y+1);

			iPoint tileWorldPos = app->map->MapToWorld(tilePos.x, tilePos.y);
			iPoint tilePosLowerRight = app->map->WorldToMap(nextFrame.x + collider->rect.w, nextFrame.y + collider->rect.h);

			SDL_Rect tileRect = { tileWorldPos.x,tileWorldPos.y,app->map->data.tileWidth,app->map->data.tileHeight };
			//LOG("Tile Pos = %d %d", tilePos.x, tilePos.y);
			if ((app->map->GetTileProperty(tilePos.x, tilePos.y, "CollisionId") == (uint)Collider::Type::SOLID
				|| app->map->GetTileProperty(tilePosLowerRight.x, tilePosLowerRight.y, "CollisionId") == (uint)Collider::Type::SOLID)
				&& collider->Intersects(tileRect))
			{
				int changes = 0;
				if (difference.x > 0)
				{
					changes++;
					nextFrame.x--;
				}
				else if (difference.x < 0)
				{
					changes++;
					nextFrame.x++;
				}
				if (difference.y > 0)
				{
					changes++;
					nextFrame.y--;
				}
				else if (difference.y < 0)
				{
					changes++;
					nextFrame.y++;
				}
				if (changes != 0) return ResolveCollisions(collider, nextFrame, dt);
				//if (difference != iPoint(0, 0)) return ResolveCollisions(collider, nextFrame, dt);
			}
			else if ((app->map->GetTileProperty(tilePos.x, tilePos.y, "CollisionId") == (uint)Collider::Type::DOOR
				|| app->map->GetTileProperty(tilePosLowerRight.x, tilePosLowerRight.y, "CollisionId") == (uint)Collider::Type::DOOR)
				&& collider->Intersects(tileRect))
			{
				if (strcmp(app->map->data.name,"tutorial.tmx") == 0)
				{
					if (onceDoor)
					{
						app->audio->PlayFx(tutorialWarpFx);
						onceDoor = false;
					}
					switch (tilePos.x)
					{
					case 15:
					{
						switch (tilePos.y)
						{
						case 28:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::SMALL_PUZZLE) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::SMALL_PUZZLE;
							}
							return { 15 * app->map->data.tileWidth, (39 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
							break;
						case 38:

							if ((app->quests->tpFlags & QuestManager::TpFlags::FIGHT) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::FIGHT;
							}							

							if (app->scene->current->currentScene == SceneType::GAMEPLAY)
							{
								onceDoor = true;
								return { 15 * app->map->data.tileWidth, (28 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
							}	

							break;
						case 50:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::GRANDPA) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::GRANDPA;
							}
							return { 15 * app->map->data.tileWidth, (64 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
							break;
						case 63:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::SMALL_PUZZLE) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::SMALL_PUZZLE;
							}
							return { 15 * app->map->data.tileWidth, (50 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
							break;
						case 78:
							onceDoor = true;
							app->quests->tpFlags = 0;
							return { 15 * app->map->data.tileWidth, (88 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
							break;
						case 87:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::GRANDPA) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::GRANDPA;
							}
							return { 15 * app->map->data.tileWidth, (78 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
							break;
						default:
							break;
						}
						break;
					}
					case 21:
					{
						switch (tilePos.y)
						{
						case 26:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::BIG_PUZZLE) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::BIG_PUZZLE;
							}
							return { (39 * app->map->data.tileWidth) + 1, 14 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
							break;
						default:
							break;
						}
						break;
					}
					case 38:
					{
						switch (tilePos.y)
						{
						case 14:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::FIGHT) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::FIGHT;
							}
							return { (20 * app->map->data.tileWidth) - 1, 26 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
							break;
						case 36:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::BOSS_FIGHT) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::BOSS_FIGHT;
							}
							return { 64 * app->map->data.tileWidth, (89 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
							break;
						case 92:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::BIG_PUZZLE) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::BIG_PUZZLE;
							}
							return { 64 * app->map->data.tileWidth, (10 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
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
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::LABYRINTH) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::LABYRINTH;
							}
							return { 38 * app->map->data.tileWidth, (92 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
							break;
						case 65:
							app->map->CleanUp();
							if ((app->quests->tpFlags & QuestManager::TpFlags::BEDROOM) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::BEDROOM;
							}
							if (app->map->data.type == MapTypes::MAPTYPE_UNKNOWN)
							{
								if (app->map->LoadNewMap("home.tmx"))
								{
									app->audio->PlayMusic("Audio/Music/Home.ogg");
									app->audio->PlayFx(wakingUpFx);
									if (app->scene->current->currentScene == SceneType::GAMEPLAY)
									{
										SceneGameplay* s = (SceneGameplay*)app->scene->current;
										app->entities->DestroyEntity(s->grandpaTutorial);
										s->hero = app->entities->CreateEntity(27 * 64 + 15, 12 * 64, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::HERO, s->player);
										s->grandpa = app->entities->CreateEntity((31 * 64) + 48, 7 * 64, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::GRANDPA, s->player);
										s->shopDude = app->entities->CreateEntity(20 * 64 + 10, 33 * 64 + 10, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::STORE_GUY, s->player);
										s->cat = app->entities->CreateEntity(35 * 64 + 10, 15 * 64 - 10, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::CAT, s->player);
									}
									return { (44 * app->map->data.tileWidth) - 1, (30 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
								}
							}
							break;
						case 89:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::LABYRINTH) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::LABYRINTH;
							}
							return { 38 * app->map->data.tileWidth, (37 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
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
				else if (strcmp(app->map->data.name, "home.tmx") == 0)
				{
					if (onceDoor)
					{
						app->audio->PlayFx(doorFx);
						onceDoor = false;
					}
					switch (tilePos.x)
					{
					case 12:
					{
						onceDoor = true;
						if ((app->quests->tpFlags & QuestManager::TpFlags::MAIN_LOBBY) == 0)
						{
							app->quests->tpFlags = 0;
							app->quests->tpFlags |= QuestManager::TpFlags::MAIN_LOBBY;
						}
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
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::BATHROOM) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::BATHROOM;
							}
							return { (11 * app->map->data.tileWidth) - 1, 14 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
							break;
						case 14:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::BATHROOM) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::BATHROOM;
							}
							return { (11 * app->map->data.tileWidth) - 1, 14 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
							break;
						case 36:
							app->audio->PlayMusic("Audio/Music/Home.ogg");
							if ((app->quests->tpFlags & QuestManager::TpFlags::BEDROOM) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::BEDROOM;
							}
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
						onceDoor = true;
						if ((app->quests->tpFlags & QuestManager::TpFlags::BEDROOM) == 0)
						{
							app->quests->tpFlags = 0;
							app->quests->tpFlags |= QuestManager::TpFlags::BEDROOM;
						}
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
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::BEDROOM) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::BEDROOM;
							}
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
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::BEDROOM) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::BEDROOM;
							}
							return { 38 * app->map->data.tileWidth, (29 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
							break;
						default:
							break;
						}
						break;
					}
					case 33:
					{
						onceDoor = true;
						if ((app->quests->tpFlags & QuestManager::TpFlags::BEDROOM) == 0)
						{
							app->quests->tpFlags = 0;
							app->quests->tpFlags |= QuestManager::TpFlags::BEDROOM;
						}
						return { 38 * app->map->data.tileWidth, (29 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
						break;
					}
					case 38:
					{
						onceDoor = true;
						if ((app->quests->tpFlags & QuestManager::TpFlags::MAIN_LOBBY) == 0)
						{
							app->quests->tpFlags = 0;
							app->quests->tpFlags |= QuestManager::TpFlags::MAIN_LOBBY;
						}
						return { 32 * app->map->data.tileWidth, (17 * app->map->data.tileHeight) - 1, collider->rect.w, collider->rect.h };
						break;
					}
					case 39:
					{
						switch (tilePos.y)
						{
						case 34:
							app->audio->PlayMusic("Audio/Music/Options.ogg");
							if ((app->quests->tpFlags & QuestManager::TpFlags::SHOP) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::SHOP;
							}
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
						case 7:
							return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
							break;
						case 8:
							return { nextFrame.x, nextFrame.y,collider->rect.w, collider->rect.h };
							break;
						case 13:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::KITCHEN) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::KITCHEN;
							}
							return { (53 * app->map->data.tileWidth) + 1, 13 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
							break;
						case 14:
							onceDoor = true;
							if ((app->quests->tpFlags & QuestManager::TpFlags::KITCHEN) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::KITCHEN;
							}
							return { (53 * app->map->data.tileWidth) + 1, 14 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
							break;
						case 34:
							app->audio->PlayMusic("Audio/Music/Options.ogg");
							if ((app->quests->tpFlags & QuestManager::TpFlags::SHOP) == 0)
							{
								app->quests->tpFlags = 0;
								app->quests->tpFlags |= QuestManager::TpFlags::SHOP;
							}
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
							if (app->map->data.type == MapTypes::MAPTYPE_UNKNOWN)
							{
								if (app->map->LoadNewMap("tutorial.tmx"))
								{
									if (app->scene->current->currentScene == SceneType::GAMEPLAY)
									{
										SceneGameplay* s = (SceneGameplay*)app->scene->current;
										s->grandpaTutorial = app->entities->CreateEntity((15 * 64) + 15, 66 * 64, EntityType::NPC, EntityId::NOT_COMBAT, Stats(0), NpcId::GRANDPA_TUTORIAL, s->player);
										app->entities->DestroyEntity(s->hero);
										app->entities->DestroyEntity(s->grandpa);
										app->entities->DestroyEntity(s->shopDude);
										app->entities->DestroyEntity(s->cat);
									}
									app->audio->PlayMusic("Audio/Music/Tutorial.ogg");
									if ((app->quests->tpFlags & QuestManager::TpFlags::BOSS_FIGHT) == 0)
									{
										app->quests->tpFlags = 0;
										app->quests->tpFlags |= QuestManager::TpFlags::BOSS_FIGHT;
									}
									return { 64 * app->map->data.tileWidth, (66 * app->map->data.tileHeight) + 1, collider->rect.w, collider->rect.h };
								}
							}
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
							onceDoor = true;
							return { (39 * app->map->data.tileWidth) - 1, 13 * app->map->data.tileHeight, collider->rect.w, collider->rect.h };
							break;
						case 14:
							onceDoor = true;
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
			else if ((app->map->GetTileProperty(tilePos.x, tilePos.y, "CollisionId") == (uint)Collider::Type::ENEMY_SPAWN
				|| app->map->GetTileProperty(tilePosLowerRight.x, tilePosLowerRight.y, "CollisionId") == (uint)Collider::Type::ENEMY_SPAWN)
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
			else if ((app->map->GetTileProperty(tilePos.x, tilePos.y, "CollisionId") == (uint)Collider::Type::EVENT
				|| app->map->GetTileProperty(tilePosLowerRight.x, tilePosLowerRight.y, "CollisionId") == (uint)Collider::Type::EVENT)
				&& collider->Intersects(tileRect) && !app->scene->current->combat)
			{
				if (strcmp(app->map->data.name, "tutorial.tmx") == 0)
				{
					switch (tilePos.y)
					{
					case 82:
						//LOG("EVENT TILE");
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
							}
						}
						break;

					case 43:
						app->quests->boolFlags |= QuestManager::BoolFlags::ESCAPEDLABYRINTH;
						break;
					default:
						break;
					}
				}
			}
			else if ((app->map->GetTileProperty(tilePos.x, tilePos.y, "CollisionId") == (uint)Collider::Type::INTERACTABLE
				|| app->map->GetTileProperty(tilePosLowerRight.x, tilePosLowerRight.y, "CollisionId") == (uint)Collider::Type::INTERACTABLE)
				&& collider->Intersects(tileRect) && !app->scene->current->combat)
			{
				if (strcmp(app->map->data.name, "home.tmx") == 0)
				{
					switch (tilePos.y)
					{
					case 12:
					{
						if (tilePos.x == 7 && app->input->CheckButton("select", KeyState::KEY_DOWN))
						{
							app->audio->PlayFx(toiletFx);
						}
					}
					case 16:
					{
						switch (tilePos.x)
						{
						case 38:
						case 39:
							saveOnce = false;
							break;
						default:
							break;
						}
						break;
					}
					case 17:
						switch (tilePos.x)
						{
						case 38:
						case 39:
							if (saveOnce == false)
							{
								app->audio->PlayFx(saveFx);
								app->SaveRequest();
								saveOnce = true;
							}
							break;
						default:
							break;
						}
						break;
					default:
						break;
					}
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
	int res = 0;
	if (rect.x < r.x + r.w
		&& rect.x + rect.w > r.x
		&& rect.y < r.y + r.h
		&& rect.h + rect.y > r.y)
	{
		res = 1;
		//if (rect.x < r.x + r.w
		//	&& rect.x + rect.w > r.x)
		//{
		//	if (abs((rect.x + rect.w) - r.x) + abs((rect.x + rect.w) - (r.x + r.w)) == r.w)
		//	{
		//		LOG("dreta");
		//	}
		//	else if (abs(rect.x - r.x) + abs(rect.x - (r.x + r.w)) == r.w)
		//	{
		//		LOG("esquerra");
		//	}
		//	//if (abs(rect.x - r.x) < abs((rect.x + rect.w) - (r.x + r.w)))
		//	//{
		//	//	LOG("ESQUERRA");
		//	//}
		//	//else
		//	//{
		//	//	LOG("DRETA");
		//	//}
		//}

		//if (rect.y < r.y + r.h
		//	&& rect.y + rect.h > r.y)
		//{

		//}
	}

	return res;
}