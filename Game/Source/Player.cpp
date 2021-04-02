#include "Player.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Map.h"
#include "EntityManager.h"
#include "Animation.h"
#include "Collisions.h"


Player::Player(int x, int y) : Entity(x, y, EntityType::PLAYER)
{
	this->entityRect = { 0,0,50,50 };
	spawnPos = { 150,150 };

	LOG("Init Player");

	if (x == -1 && y == -1)
		this->nextPos = { spawnPos.x, spawnPos.y };
	else
		this->nextPos = { x,y };

	pendingToDelete = false;
	
	// Animation
	
	int flags = 1 << FlagsAnimation::DOWN;

	idle.PushBack({ 2,2,50,50 });

	idleLeft.PushBack({ 2,54,50,50 });

	idleRight.PushBack({ 2,106,50,50 });

	idleUp.PushBack({ 2,158,50,50 });


	for (int i = 0; i < 4; i++)
	{
		walkingDown.PushBack({ (entityRect.w * i) + (2*(i+1)),2,50,50 });
	}
	walkingDown.speed = 10.f;
	walkingDown.loop = true;

	for (int i = 0; i < 4; i++)
	{
		walkingLeft.PushBack({ (entityRect.w * i) + (2 * (i+1)),54,50,50 });
	}
	walkingLeft.speed = 10.f;
	walkingLeft.loop = true;

	for (int i = 0; i < 4; i++)
	{
		walkingRight.PushBack({ (entityRect.w * i) + (2 * (i+1)),106,50,50 });
	}
	walkingRight.speed = 10.f;
	walkingRight.loop = true;

	for (int i = 0; i < 4; i++)
	{
		walkingUp.PushBack({ (entityRect.w * i) + (2 * (i+1)),158,50,50 });
	}
	walkingUp.speed = 10.f;
	walkingUp.loop = true;

	currentAnim = &idle;

	inMenu = false;
}

bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && currentAnim == &idle)
		app->SaveRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && currentAnim == &idle)
		app->LoadRequest();

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		godMode = !godMode;

	if (app->input->CheckButton("select", KEY_DOWN))
		LOG("Pressing Select");

	if (app->input->CheckButton("cancel", KEY_DOWN))
		LOG("Pressing Cancel");

	if (app->input->CheckButton("menu", KEY_DOWN) || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		inMenu = !inMenu;
		LOG("Pressing Menu");
	}

	animFlags = ClearBit(animFlags, FlagsAnimation::WALKING);
	currentAnim->Update(dt);

	if (!inMenu)
	{
		if (app->input->CheckButton("right", KEY_REPEAT))
		{
			animFlags = 0;
			animFlags = SetBit(animFlags, FlagsAnimation::RIGHT);
			animFlags = SetBit(animFlags, FlagsAnimation::WALKING);
			currentAnim = &walkingRight;
			nextPos.x += 3;
		}
		if (app->input->CheckButton("left", KEY_REPEAT))
		{
			animFlags = 0;
			animFlags = SetBit(animFlags, FlagsAnimation::LEFT);
			animFlags = SetBit(animFlags, FlagsAnimation::WALKING);
			currentAnim = &walkingLeft;
			nextPos.x -= 3;
		}
		if (app->input->CheckButton("down", KEY_REPEAT))
		{
			animFlags = 0;
			animFlags = SetBit(animFlags, FlagsAnimation::DOWN);
			animFlags = SetBit(animFlags, FlagsAnimation::WALKING);
			currentAnim = &walkingDown;
			nextPos.y += 3;
		}
		if (app->input->CheckButton("up", KEY_REPEAT))
		{
			animFlags = 0;
			animFlags = SetBit(animFlags, FlagsAnimation::UP);
			animFlags = SetBit(animFlags, FlagsAnimation::WALKING);
			currentAnim = &walkingUp;
			nextPos.y -= 3;
		}
	}

													// IMPORTANT: Before this point use nextPos for referencing the player position
	app->collisions->ResolveCollisions(entityRect, nextPos);
													// IMPORTANT: After this point use entityRect for referencing the player position
	entityRect.x = nextPos.x;
	entityRect.y = nextPos.y;

	if (animFlags == (1 << FlagsAnimation::DOWN)) currentAnim = &idle;
	else if (animFlags == (1 << FlagsAnimation::RIGHT)) currentAnim = &idleRight;
	else if (animFlags == (1 << FlagsAnimation::LEFT)) currentAnim = &idleLeft;
	else if (animFlags == (1 << FlagsAnimation::UP)) currentAnim = &idleUp;

	return true;
}

bool Player::Draw()
{
	app->render->DrawTexture(app->entities->playerTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame(), invert);
	if (app->render->debug)
	{
		app->render->DrawRectangle({ entityRect.x, entityRect.y, entityRect.w, entityRect.h }, 0, 0, 150, 100);
	}
	//app->render->DrawTexture(app->entities->playerTex, playerPos.x, playerPos.y, false, &currentAnim->GetCurrentFrame(), invert);
	return true;
}
