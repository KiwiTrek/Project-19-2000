#include "App.h"
#include "ParticleSystem.h"
#include "Textures.h"

#include "Defs.h"
#include "Log.h"

ParticleSystem::ParticleSystem() : Module()
{
	memset(name, 0, TEXT_LEN);
	strcpy_s(name,TEXT_LEN, "particleManager");
}

ParticleSystem::~ParticleSystem()
{}

void ParticleSystem::Init()
{
	active = false;
}

bool ParticleSystem::Awake(pugi::xml_node& config)
{
	LOG("Loading Particle System");
	strcpy_s(folder,TEXT_LEN, config.child("folder").child_value());
	LoadEmittersData();
	return true;
}

bool ParticleSystem::Start()
{
	// Loads the particle atlas
	char tmp[TEXT_LEN] = { 0 };
	sprintf_s(tmp,TEXT_LEN, "%s%s", folder, "ParticlesAtlas.png");
	particleAtlas = app->tex->Load(tmp);

	// Clear the particle list
	ListItem<Emitter*>* e = emittersList.start;
	while (e != nullptr)
	{
		ListItem<Emitter*>* eNext = e->next;
		e->data->CleanUp();
		emittersList.Del(emittersList.At(emittersList.Find(e->data)));
		delete e->data;
		e = eNext;
	}
	emittersList.Clear();

	return true;
}

bool ParticleSystem::PreUpdate()
{
	return true;
}

bool ParticleSystem::Update(float dt)
{
	// Updates all emitters
	ListItem<Emitter*>* e = emittersList.start;
	while (e != nullptr)
	{
		e->data->Update(dt);
		e = e->next;
	}

	return true;
}

bool ParticleSystem::PostUpdate()
{
	// Checks if there are any emitters to delete
	if (emittersToDestroy.Count() != 0)
	{
		ListItem<Emitter*>* e = emittersToDestroy.start;
		while (e != nullptr)
		{
			ListItem<Emitter*>* eNext = e->next;
			e->data->CleanUp();
			emittersList.Del(emittersList.At(emittersList.Find(e->data)));
			delete e->data;
			e = eNext;
		}
		emittersToDestroy.Clear();
	}

	// PostUpdates all emitters
	ListItem<Emitter*>* e = emittersList.start;
	while (e != nullptr)
	{
		e->data->PostUpdate();
		e = e->next;
	}

	return true;
}

bool ParticleSystem::CleanUp()
{
	ListItem<Emitter*>* e = emittersList.start;
	while (e != nullptr)
	{
		ListItem<Emitter*>* eNext = e->next;
		e->data->CleanUp();
		delete e;
		e = eNext;
	}
	emittersList.Clear();

	ListItem<Emitter*>* eToDestroy = emittersToDestroy.start;
	while (eToDestroy != nullptr)
	{
		ListItem<Emitter*>* eNext = eToDestroy->next;
		eToDestroy->data->CleanUp();
		delete eToDestroy;
		eToDestroy = eNext;
	}
	emittersToDestroy.Clear();

	app->tex->UnLoad(particleAtlas);

	return true;
}

Emitter* ParticleSystem::AddEmitter(fPoint pos, EmitterData::EmitterType type)
{
	Emitter* e = new Emitter(pos, emittersData[type]);
	emittersList.Add(e);
	return e;
}

bool ParticleSystem::RemoveEmitter(Emitter* emitter)
{
	if (emitter != nullptr && emittersList.Count() != 0)
	{
		emittersToDestroy.Add(emitter);
		return true;
	}
	return false;
}

bool ParticleSystem::RemoveAllEmitters()
{
	if (emittersList.Count() != 0)
	{
		ListItem<Emitter*>* e = emittersList.start;
		while (e != nullptr)
		{
			emittersToDestroy.Add(e->data);
			e = e->next;
		}
		return true;
	}
	return false;
}

SDL_Texture* ParticleSystem::GetParticleAtlas()
{
	return particleAtlas;
}

void ParticleSystem::LoadEmittersData()
{
	EmitterData::EmitterType type = EmitterData::EmitterType::NONE;
	for (int i = 0; i <= MAX_NUM_EMITTERS_TYPE; i++)
	{
		switch (type)
		{
		case EmitterData::EmitterType::FIRE:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 80,110 };
			eData.initialSpeed = 200;
			eData.finalSpeed = 200;
			eData.initialSize = { 200,200 };
			eData.finalSize = { 80,80 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 100;
			eData.emitLifetime = -1;

			eData.texRect = { 512,256,256,256 };
			eData.initialColor = { 255,100,0,255 };
			eData.finalColor = { 210,210,210,0 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,1 };
			eData.randFinalSpeed = { 0,1 };
			eData.randEmitMult = { 0,1 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = true;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::FIRE;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::STUN:
		{
			EmitterData eData;
			eData.rotSpeed = 5;
			eData.angleRange = { 0,180 };
			eData.initialSpeed = 10;
			eData.finalSpeed = 10;
			eData.initialSize = { 80,80 };
			eData.finalSize = { 100,100 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 60;
			eData.emitLifetime = 60;

			eData.texRect = { 512,768,256,256 };
			eData.initialColor = { 255,255,100,255 };
			eData.finalColor = { 255,255,150,0 };
			eData.blendMode = SDL_BLENDMODE_BLEND;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,1 };
			eData.randFinalSpeed = { 0,1 };
			eData.randEmitMult = { 0,1 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = false;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::STUN;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::NOVA:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 50,130 };
			eData.initialSpeed = 0;
			eData.finalSpeed = 0;
			eData.initialSize = { 0,0 };
			eData.finalSize = { 800,800 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 140;
			eData.emitLifetime = 100;

			eData.texRect = { 768,512,256,256 };
			eData.initialColor = { 255,255,100,255 };
			eData.finalColor = { 255,255,100,0 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,1 };
			eData.randFinalSpeed = { 0,1 };
			eData.randEmitMult = { 0,1 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = false;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::NOVA;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::SLASH:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 50,130 };
			eData.initialSpeed = 0;
			eData.finalSpeed = 0;
			eData.initialSize = { 250,250 };
			eData.finalSize = { 400,400 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 10;
			eData.emitLifetime = 10;

			eData.texRect = { 0,768,256,256 };
			eData.initialColor = { 255,255,50,255 };
			eData.finalColor = { 255,255,200,0 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,1 };
			eData.randFinalSpeed = { 0,1 };
			eData.randEmitMult = { 0,1 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = false;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::SLASH;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::BUBBLE:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 30,150 };
			eData.initialSpeed = 250;
			eData.finalSpeed = 250;
			eData.initialSize = { 200,200 };
			eData.finalSize = { 30,30 };

			eData.emitNumber = 5;
			eData.emitNumMult = 7;
			eData.maxParticleLife = 300;
			eData.emitLifetime = 600;

			eData.texRect = { 1270,0,256,256 };
			eData.initialColor = { 0,40,255,255 };
			eData.finalColor = { 0,100,255,150 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,1 };
			eData.randFinalSpeed = { 0,1 };
			eData.randEmitMult = { 0,1 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = true;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::BUBBLE;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::HEAL:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 30,150 };
			eData.initialSpeed = 250;
			eData.finalSpeed = 250;
			eData.initialSize = { 200,200 };
			eData.finalSize = { 300,300 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 100;
			eData.emitLifetime = 1;

			eData.texRect = { 0,0,256,256 };
			eData.initialColor = { 30,255,30,255 };
			eData.finalColor = { 50,255,50,0 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,0 };
			eData.randFinalSpeed = { 0,0 };
			eData.randEmitMult = { 0,0 };
			eData.randLife = { 0,0 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = false;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::HEAL;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::BLESS:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 30,150 };
			eData.initialSpeed = 250;
			eData.finalSpeed = 250;
			eData.initialSize = { 250,250 };
			eData.finalSize = { 300,300 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 50;
			eData.emitLifetime = 50;

			eData.texRect = { 768,0,256,256 };
			eData.initialColor = { 255,255,255,255 };
			eData.finalColor = { 100,100,100,0 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,0 };
			eData.randFinalSpeed = { 0,0 };
			eData.randEmitMult = { 0,0 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = false;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::BLESS;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::BUFF:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 30,150 };
			eData.initialSpeed = 250;
			eData.finalSpeed = 250;
			eData.initialSize = { 200,200 };
			eData.finalSize = { 200,250 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 45;
			eData.emitLifetime = 45;

			eData.texRect = { 256*4,256*3,256,256 };
			eData.initialColor = { 255,255,50,255 };
			eData.finalColor = { 255,200,0,0 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,0 };
			eData.randFinalSpeed = { 0,0 };
			eData.randEmitMult = { 0,0 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = false;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::BUFF;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::DEBUFF:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 30,150 };
			eData.initialSpeed = 250;
			eData.finalSpeed = 250;
			eData.initialSize = { 200,200 };
			eData.finalSize = { 200,250 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 45;
			eData.emitLifetime = 45;

			eData.texRect = { 256 * 5,256 * 3,256,256 };
			eData.initialColor = { 150,50,255,255 };
			eData.finalColor = { 150,0,255,0 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,0 };
			eData.randFinalSpeed = { 0,0 };
			eData.randEmitMult = { 0,0 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = false;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::DEBUFF;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::TAUNT:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 30,150 };
			eData.initialSpeed = 0;
			eData.finalSpeed = 0;
			eData.initialSize = { 150,150 };
			eData.finalSize = { 200,200 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 60;
			eData.emitLifetime = 60;

			eData.texRect = { 768,768,256,256 };
			eData.initialColor = { 150,150,150,100 };
			eData.finalColor = { 50,50,50,0 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,0 };
			eData.randFinalSpeed = { 0,0 };
			eData.randEmitMult = { 0,0 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = false;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::TAUNT;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::ITEM:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 30,150 };
			eData.initialSpeed = 0;
			eData.finalSpeed = 0;
			eData.initialSize = { 100,50 };
			eData.finalSize = { 100,50 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 40;
			eData.emitLifetime = 40;

			eData.texRect = { 3* 256 + 128,4*256 + 128,256,128 };
			eData.initialColor = { 255,150,0,255 };
			eData.finalColor = { 255,200,0,0 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,0 };
			eData.randFinalSpeed = { 0,0 };
			eData.randEmitMult = { 0,0 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 0.6f,0.6f };
			eData.randFinalSize = { 0.8f,0.8f };

			eData.vortexActive = false;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::ITEM;
			emittersData[type] = eData;
			break;
		}
		case EmitterData::EmitterType::MAGIC:
		{
			EmitterData eData;
			eData.rotSpeed = 0;
			eData.angleRange = { 30,150 };
			eData.initialSpeed = 0;
			eData.finalSpeed = 0;
			eData.initialSize = { 180,180 };
			eData.finalSize = { 200,200 };

			eData.emitNumber = 1;
			eData.emitNumMult = 0;
			eData.maxParticleLife = 50;
			eData.emitLifetime = 50;

			eData.texRect = { 1 * 256,3 * 256 ,256,256 };
			eData.initialColor = { 0,150,255,255 };
			eData.finalColor = { 0,100,255,0 };
			eData.blendMode = SDL_BLENDMODE_ADD;

			eData.randRotSpeed = { -1,1 };
			eData.randInitialSpeed = { 0,1 };
			eData.randFinalSpeed = { 0,1 };
			eData.randEmitMult = { 0,1 };
			eData.randLife = { 0,1 };
			eData.randInitialSize = { 1,1 };
			eData.randFinalSize = { 1,1 };

			eData.vortexActive = false;
			eData.halfTex = false;
			eData.eType = EmitterData::EmitterType::MAGIC;
			emittersData[type] = eData;
			break;
		}
		default:
		{
			break;
		}
		}

		// Next emitter type
		switch (i)
		{
		case 0:
			type = EmitterData::EmitterType::FIRE;
			break;
		case 1:
			type = EmitterData::EmitterType::STUN;
			break;
		case 2:
			type = EmitterData::EmitterType::NOVA;
			break;
		case 3:
			type = EmitterData::EmitterType::SLASH;
			break;
		case 4:
			type = EmitterData::EmitterType::BUBBLE;
			break;
		case 5:
			type = EmitterData::EmitterType::HEAL;
			break;
		case 6:
			type = EmitterData::EmitterType::BLESS;
			break;
		case 7:
			type = EmitterData::EmitterType::BUFF;
			break;
		case 8:
			type = EmitterData::EmitterType::DEBUFF;
			break;
		case 9:
			type = EmitterData::EmitterType::TAUNT;
			break;
		case 10:
			type = EmitterData::EmitterType::ITEM;
			break;
		case 11:
			type = EmitterData::EmitterType::MAGIC;
			break;
		default:
			break;
		}
	}
}