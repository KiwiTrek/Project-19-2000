#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Point.h"
#include "SDL.h"

#define MIN_LIFE_TO_INTERPOLATE 15

class Particle
{
public:
	// Constructor
	Particle(fPoint pos, float initialSpeed, float finalSpeed, float angle, double rotSpeed, fPoint initialSize, fPoint finalSize, uint life, SDL_Rect texRect, SDL_Color initialColor, SDL_Color finalColor, SDL_BlendMode blendMode, bool vortexActive, bool halfTex);

	// Called each loop iteration
	void Update(float dt);

	// Draws the particle
	bool Draw();

	// Returns if the particle is considered alive or not
	bool IsAlive();

	// Transitions the color from the starting to the end color
	SDL_Color InterpolateColor(SDL_Color initialColor, float timeStep, SDL_Color finalColor);

	// Transitions one variable from the starting value to the end value
	float InterpolateBetween(float min, float timeStep, float max);
	fPoint InterpolateBetween(fPoint min, float timeStep, fPoint max);

	// Makes the Particle sensitive to the given vortex
	void SetVortex(fPoint pos, float speed, float scale);

	// Calculates the particle next frame's position considering the vortex if there is one
	void CalculateParticlePos(float dt);

public:

	bool pendingToDelete;

private:

	struct Vortex
	{
	public:

		fPoint pos;
		float speed;
		float scale;
	};

	fPoint pos;
	fPoint initialSpeed;
	fPoint curSpeed;
	fPoint finalSpeed;
	double rotSpeedIncrement;
	double curRotSpeed;

	fPoint initialSize;
	fPoint curSize;
	fPoint finalSize;

	SDL_Rect pRect;
	SDL_Rect rectSize;
	SDL_Color initialColor;
	SDL_Color finalColor;
	SDL_BlendMode blendMode;

	uint life;
	uint initialLife;
	float timeStep;

	Vortex vortex;
	bool vortexActive;
	bool halfTex;
};
#endif // !__PARTICLE_H__