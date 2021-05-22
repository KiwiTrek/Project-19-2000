#ifndef __EMITTER_H__
#define __EMITTER_H__

#include "Point.h"
#include "Timer.h"
#include "List.h"
#include "SDL.h"

class Particle;

struct EmitterData
{
	enum EmitterType
	{
		NONE = -1,
		FIRE,
		SMOKE,
		NOVA,
		SLASH,
		BUBBLE,
		HEAL,
		BLESS
	};

	double rotSpeed;
	fPoint angleRange;
	float initialSpeed;
	float finalSpeed;
	fPoint initialSize;
	fPoint finalSize;

	uint emitNumber;
	uint emitNumMult;
	uint maxParticleLife;
	double emitLifetime;

	SDL_Rect texRect;
	SDL_Color initialColor;
	SDL_Color finalColor;
	SDL_BlendMode blendMode;

	fPoint randRotSpeed;
	fPoint randInitialSpeed;
	fPoint randFinalSpeed;
	fPoint randEmitMult;
	fPoint randLife;
	fPoint randInitialSize;
	fPoint randFinalSize;

	bool vortexActive;
	bool halfTex;
	EmitterType eType;
};

class Emitter
{
public:
	// Constructor
	Emitter(fPoint pos, EmitterData data);

	// Destructor
	virtual ~Emitter();

	// Called each loop iteration
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Returns a random number between two given numbers
	float GenerateRandNum(float min, float max);

	// Moves the emitter to the given position
	void MoveEmitter(fPoint newPos);

	// Set/Get emitter vortex
	void SetVortexActive(bool sensitive);
	bool GetVortexActive();

	// To destroy particles
	void DestroyParticle(Particle* p);

private:

	EmitterData data;
	fPoint pos;
	uint maximumParticlesPerFrame;

	List<Particle*> particlePool;

	bool active;
	Timer lifeTimer;
};
#endif // !__EMITTER_H__