#include "Player.h"

#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Map.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Animation.h"
#include "Collisions.h"


Player::Player(int x, int y, int footstepFx) : Entity(x, y, EntityType::PLAYER)
{
	if (app->map->data.type != MapTypes::MAPTYPE_UNKNOWN)
	{
		if (app->map->data.name == "tutorial.tmx") spawnPos = { 15,90 };
		spawnPos.x *= app->map->data.tileWidth;
		spawnPos.y *= app->map->data.tileHeight;
	}
	else
	{
		spawnPos = { 960,5760 };
	}

	LOG("Init Player");

	if (x == -1 && y == -1)
		this->nextPos = { spawnPos.x, spawnPos.y };
	else
		this->nextPos = { x,y };

	this->entityRect = { nextPos.x, nextPos.y,50,50 };

	pendingToDelete = false;
	godMode = false;
    inDialog = false;

	// Animation
	
	animFlags = 0;

	idle.PushBack({ 2,2,50,50 });

	idleLeft.PushBack({ 2,54,50,50 });

	idleRight.PushBack({ 2,106,50,50 });

	idleUp.PushBack({ 2,158,50,50 });


	for (int i = 0; i < 4; i++)
	{
		walkingDown.PushBack({ (entityRect.w * i) + (2 * (i + 1)),2,50,50 });
	}
	walkingDown.speed = 5.f;
	walkingDown.loop = true;

	for (int i = 0; i < 4; i++)
	{
		walkingLeft.PushBack({ (entityRect.w * i) + (2 * (i+1)),54,50,50 });
	}
	walkingLeft.speed = 5.f;
	walkingLeft.loop = true;

	for (int i = 0; i < 4; i++)
	{
		walkingRight.PushBack({ (entityRect.w * i) + (2 * (i+1)),106,50,50 });
	}
	walkingRight.speed = 5.f;
	walkingRight.loop = true;

	for (int i = 0; i < 4; i++)
	{
		walkingUp.PushBack({ (entityRect.w * i) + (2 * (i+1)),158,50,50 });
	}
	walkingUp.speed = 5.f;
	walkingUp.loop = true;

	currentAnim = &idle;
	this->footstepFx = footstepFx;
	timerFootstep = 0.0f;

	collider = app->collisions->AddCollider({ this->entityRect }, Collider::Type::PLAYER, (Module*)app->entities);
}

bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		godMode = !godMode;

	if (!app->scene->current->combat)
	{
		if (app->scene->current->currentScene == SceneType::GAMEPLAY)
		{
			if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN && currentAnim == &idle)
			{
				app->SaveRequest();
				app->audio->PlayFx(app->collisions->saveFx);
			}
			if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && currentAnim == &idle)
				app->LoadRequest();
		}

		if (timerFootstep >= 0.4f)
		{
			timerFootstep = 0;
			app->audio->PlayFx(footstepFx);
		}

		if (app->input->CheckButton("select", KEY_DOWN))
			LOG("Pressing Select");

		if (app->input->CheckButton("cancel", KEY_DOWN))
			LOG("Pressing Cancel");

		if ((app->input->CheckButton("menu", KEY_DOWN) || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN))
			LOG("Pressing Menu");
		
		animFlags = ClearBit(animFlags, FlagsAnimation::WALKING);
		currentAnim->Update(dt);
		nextPos = { entityRect.x, entityRect.y };

		if (!app->scene->current->combat && !app->entities->inPause && inDialog == false)
		{
			if (app->input->CheckButton("right", KEY_REPEAT))
			{
				animFlags = 0;
				animFlags = SetBit(animFlags, FlagsAnimation::RIGHT);
				animFlags = SetBit(animFlags, FlagsAnimation::WALKING);
				currentAnim = &walkingRight;
				nextPos.x += 3;
				if (godMode && app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) nextPos.x += 10;
				timerFootstep += dt;
			}
			if (app->input->CheckButton("left", KEY_REPEAT))
			{
				animFlags = 0;
				animFlags = SetBit(animFlags, FlagsAnimation::LEFT);
				animFlags = SetBit(animFlags, FlagsAnimation::WALKING);
				currentAnim = &walkingLeft;
				nextPos.x -= 3;
				if (godMode && app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) nextPos.x -= 10;
				timerFootstep += dt;
			}
			if (app->input->CheckButton("down", KEY_REPEAT))
			{
				animFlags = 0;
				animFlags = SetBit(animFlags, FlagsAnimation::DOWN);
				animFlags = SetBit(animFlags, FlagsAnimation::WALKING);
				currentAnim = &walkingDown;
				nextPos.y += 3;
				if (godMode && app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) nextPos.y += 10;
				timerFootstep += dt;
			}
			if (app->input->CheckButton("up", KEY_REPEAT))
			{
				animFlags = 0;
				animFlags = SetBit(animFlags, FlagsAnimation::UP);
				animFlags = SetBit(animFlags, FlagsAnimation::WALKING);
				currentAnim = &walkingUp;
				nextPos.y -= 3;
				if (godMode && app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) nextPos.y -= 10;
				timerFootstep += dt;
			}
		}

		if (!godMode) entityRect = app->collisions->ResolveCollisions(collider, nextPos,dt);
		else entityRect = { nextPos.x,nextPos.y,entityRect.w,entityRect.h };

		collider->SetPos(entityRect.x, entityRect.y, entityRect.w, entityRect.h);
		uint width, height;
		app->win->GetWindowSize(width, height);
		app->render->camera.x = -(entityRect.x - (int)width / 2 + entityRect.w / 2);
		app->render->camera.y = -(entityRect.y - (int)height / 2 + entityRect.h / 2);
		//app->render->camera.x = -(entityRect.x - (int)width / 2 + entityRect.w);
		//app->render->camera.y = -(entityRect.y - (int)height / 2 - 64 + entityRect.h);

		//if ((app->render->camera.x + entityRect.x) < (app->map->data.tileWidth * 9)) app->render->camera.x += (250.0f * dt);
		//if ((entityRect.w + app->render->camera.x + entityRect.x) > ((int)width - app->map->data.tileWidth * 11)) app->render->camera.x -= (250.0f * dt);
		//if ((app->render->camera.y + entityRect.y) < (app->map->data.tileHeight * 5)) app->render->camera.y += floor(250.0f * dt);
		//if ((entityRect.h + app->render->camera.y + entityRect.y) > ((int)height - app->map->data.tileHeight * 7)) app->render->camera.y -= floor(250.0f * dt);

		if (animFlags == (1 << FlagsAnimation::DOWN)) currentAnim = &idle;
		else if (animFlags == (1 << FlagsAnimation::RIGHT)) currentAnim = &idleRight;
		else if (animFlags == (1 << FlagsAnimation::LEFT)) currentAnim = &idleLeft;
		else if (animFlags == (1 << FlagsAnimation::UP)) currentAnim = &idleUp;
	}

	return true;
}

bool Player::Draw()
{
	app->render->DrawTexture(app->entities->playerTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame(), invert);
	if (app->render->debug)
	{
		app->render->DrawRectangle(collider->rect, 0, 0, 150, 100);
	}
	//app->render->DrawTexture(app->entities->playerTex, playerPos.x, playerPos.y, false, &currentAnim->GetCurrentFrame(), invert);
	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == Collider::Type::SOLID)
	{
		while (c1->Intersects(c2->rect))
		{
			iPoint difference = { c2->rect.x + c2->rect.w/2 - c1->rect.x - c1->rect.w/2, c2->rect.y + c2->rect.h/2 - c1->rect.y - c1->rect.h / 2 };

			if (difference.x > 0) c1->rect.x--;
			else if (difference.x < 0) c1->rect.x++;

			if (difference.y > 0) c1->rect.y--;
			else if (difference.y < 0) c1->rect.y++;
		}
	}
	
	entityRect.x = c1->rect.x;
	entityRect.y = c1->rect.y;
}