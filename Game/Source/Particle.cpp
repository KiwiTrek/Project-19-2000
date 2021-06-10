#include "Particle.h"
#include "App.h"
#include "Render.h"
#include "ParticleSystem.h"
#include "Defs.h"
#include "Log.h"


Particle::Particle(fPoint pos, float initialSpeed, float finalSpeed, float angle, double rotSpeed, fPoint initialSize, fPoint finalSize, uint life, SDL_Rect texRect, SDL_Color initialColor, SDL_Color finalColor, SDL_BlendMode blendMode, bool vortexActive, bool halfTex) : life(0)
{
	// Movement properties
	this->pos = pos;
	this->initialSpeed.x = initialSpeed * cos(DEG_2_RAD(angle));
	this->initialSpeed.y = -initialSpeed * sin(DEG_2_RAD(angle));
	this->finalSpeed.x = finalSpeed * cos(DEG_2_RAD(angle));
	this->finalSpeed.y = -finalSpeed * sin(DEG_2_RAD(angle));
	this->curRotSpeed = this->rotSpeedIncrement = rotSpeed;

	// Size properties
	this->curSize = this->initialSize = initialSize;
	this->finalSize = finalSize;

	// Color and texture properties
	this->pRect = this->rectSize = texRect;
	this->initialColor = initialColor;
	this->finalColor = finalColor;
	this->blendMode = blendMode;

	// Life properties
	this->life = this->initialLife = life;
	timeStep = 0.0f;

	// Vortex property
	this->vortexActive = vortexActive;

	// If half a texture
	this->halfTex = halfTex;

	pendingToDelete = false;
}

void Particle::Update(float dt)
{
	// Adds a vortex to the system (you can only have one)
	if (vortexActive)
	{
		SetVortex({ 400.0f, 250.0f }, 10.0f, 50.0f);
	}
	else
	{
		SetVortex({ 0.0f, 0.0f }, 0.0f, 0.0f);
	}

	curSize = InterpolateBetween(initialSize, timeStep, finalSize);
	rectSize.w = curSize.x;
	rectSize.h = curSize.y;

	curSpeed = InterpolateBetween(initialSpeed, timeStep, finalSpeed);

	// Calculates the new particle's position
	CalculateParticlePos(dt);

	// Particle's life decrease
	life--;
}

bool Particle::Draw()
{
	bool ret = true;

	// Calculates the center of the particle's current texture
	fPoint center;
	center.x = pos.x - rectSize.w / 2;
	if (halfTex)
	{
		center.y = pos.y - rectSize.h;
	}
	else
	{
		center.y = pos.y - rectSize.h / 2;
	}

	SDL_Color resColor = InterpolateColor(initialColor, timeStep, finalColor);

	// Drawing particle on the screen
	ret = app->render->DrawParticle(app->particles->GetParticleAtlas(), (int)center.x - app->render->camera.x, (int)center.y - app->render->camera.y, &pRect, &rectSize, resColor, blendMode, 1.0f, curRotSpeed);

	// Debug drawing rect
	if (app->render->debug)
	{
		app->render->DrawRectangle({ (int)pos.x - rectSize.w / 2 - app->render->camera.x,(int)pos.y - rectSize.h / 2 - app->render->camera.y,rectSize.w,rectSize.h }, resColor.r, resColor.g, resColor.b, 100);
	}

	// Calculates the new rotation according to the rotation speed
	curRotSpeed += rotSpeedIncrement;

	// Time step increment to interpolate colors and size
	timeStep += (1.0f / (float)initialLife);

	if (timeStep >= 1.0f)
	{
		timeStep = 0.0f;
		pendingToDelete = true;
	}

	return ret;
}

bool Particle::IsAlive()
{
	return (life > 0 && pendingToDelete == false);
}

SDL_Color Particle::InterpolateColor(SDL_Color initialColor, float timeStep, SDL_Color finalColor)
{
	SDL_Color resultingColor;
	resultingColor.r = Uint8(initialColor.r + (finalColor.r - initialColor.r) * timeStep);
	resultingColor.g = Uint8(initialColor.g + (finalColor.g - initialColor.g) * timeStep);
	resultingColor.b = Uint8(initialColor.b + (finalColor.b - initialColor.b) * timeStep);
	resultingColor.a = Uint8(initialColor.a + (finalColor.a - initialColor.a) * timeStep);

	return resultingColor;
}

float Particle::InterpolateBetween(float min, float timeStep, float max)
{
	return min + (max - min) * timeStep;
}

fPoint Particle::InterpolateBetween(fPoint min, float timeStep, fPoint max)
{
	fPoint result;
	result.x = InterpolateBetween(min.x, timeStep, max.x);
	result.y = InterpolateBetween(min.y, timeStep, max.y);
	return result;
}

void Particle::SetVortex(fPoint pos, float speed, float scale)
{
	vortex.pos = pos;
	vortex.speed = speed;
	vortex.scale = scale;
}

void Particle::CalculateParticlePos(float dt)
{
	fPoint dif = { pos.x - vortex.pos.x, pos.y - vortex.pos.y };
	fPoint vel = { -dif.y * vortex.speed, dif.x * vortex.speed };
	float inversePower = (dif.x * dif.x + dif.y * dif.y) / vortex.scale;
	float power = 1.0f / (1.0f + inversePower);
	pos.x += (vel.x - curSpeed.x) * power + curSpeed.x * dt;
	pos.y += (vel.y - curSpeed.y) * power + curSpeed.y * dt;
}