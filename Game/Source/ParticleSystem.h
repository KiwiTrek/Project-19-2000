#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "Module.h"
#include "List.h"
#include "SString.h"
#include "SDL.h"
#include "Emitter.h"

#define MAX_NUM_EMITTERS_TYPE 7

struct EmitterData;
class Emitter;

class ParticleSystem : public Module
{
public:
	// Constructor
	ParticleSystem();

	// Destructor
	virtual ~ParticleSystem();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Creates a type emitter
	Emitter* AddEmitter(fPoint pos, EmitterData::EmitterType type);

	// Deletes the given emitter
	bool RemoveEmitter(Emitter* emitter);

	// Deletes all active emitters
	bool RemoveAllEmitters();

	// Returns the particle atlas
	SDL_Texture* GetParticleAtlas();

private:
	// Loads all different types of emitters
	void LoadEmittersData();

	SString folder;

	EmitterData emittersData[MAX_NUM_EMITTERS_TYPE];
	List<Emitter*> emittersList;
	List<Emitter*> emittersToDestroy;
	SDL_Texture* particleAtlas;
};
#endif // !__PARTICLE_SYSTEM_H__