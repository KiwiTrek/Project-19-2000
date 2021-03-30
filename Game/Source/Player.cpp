#include "Player.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Map.h"
#include "EntityManager.h"
#include "Animation.h"
#include "Physics.h"

Player::Player(int x, int y) : Entity(x, y, EntityType::PLAYER)
{
	this->entityRect = { 0,0,64,64 };
	spawnPos = { 150,150 };

	LOG("Init Player");

	if (x == -1 && y == -1)
		this->nextPos = { spawnPos.x, spawnPos.y };
	else
		this->nextPos = { x,y };

	pendingToDelete = false;
	
	// Animation
	
	idle.PushBack({ 0,0,38,48 });

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

	if (!inMenu)
	{
		if (app->input->CheckButton("right", KEY_REPEAT))
			nextPos.x += 3;

		if (app->input->CheckButton("left", KEY_REPEAT))
			nextPos.x -= 3;

		if (app->input->CheckButton("down", KEY_REPEAT))
			nextPos.y += 3;

		if (app->input->CheckButton("up", KEY_REPEAT))
			nextPos.y -= 3;
	}

													// IMPORTANT: Before this point use nextPos for referencing the player position
	//physics.ResolveCollisions(entityRect, nextPos, invert);
													// IMPORTANT: After this point use entityRect for referencing the player position
	entityRect.x = nextPos.x;
	entityRect.y = nextPos.y;
	return true;
}

bool Player::Draw()
{
	app->render->DrawTexture(app->entities->playerTex, entityRect.x, entityRect.y, false, &currentAnim->GetCurrentFrame(), invert);
	if (app->render->debug)
	{
		app->render->DrawRectangle({ entityRect.x - 13, entityRect.y - 8,entityRect.w,entityRect.h }, 0, 0, 150, 100);
	}
	//app->render->DrawTexture(app->entities->playerTex, playerPos.x, playerPos.y, false, &currentAnim->GetCurrentFrame(), invert);

	return true;
}
