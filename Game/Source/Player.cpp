#include "Player.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Map.h"
#include "EntityManager.h"
#include "Animation.h"
#include "Physics.h"

Player::Player(int x, int y) : Entity(x, y, EntityType::PLAYER)
{
	this->entityRect = { 0,0,64,64 };

	LOG("Init Player");
	spawnPos = { 100,100 };
	playerPos = spawnPos;

	pendingToDelete = false;
	
	// Animation
	
	idle.PushBack({ 0,0,38,48 });

	currentAnim = &idle;
}

bool Player::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && currentAnim == &idle)
	{
		app->SaveRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && currentAnim == &idle)
	{
		app->LoadRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
	}

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		playerPos.x += 3;
	}

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		playerPos.x -= 3;
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		playerPos.y += 3;
	}

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		playerPos.y -= 3;
	}

	return true;
}

bool Player::Draw()
{
	app->render->DrawTexture(app->entities->playerTex, playerPos.x, playerPos.y, false, &currentAnim->GetCurrentFrame(), invert);

	return true;
}
