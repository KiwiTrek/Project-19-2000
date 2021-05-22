#include "Emitter.h"
#include "App.h"
#include <time.h>
#include "ParticleSystem.h"
#include "Particle.h"
#include "Log.h"

Emitter::Emitter(fPoint pos, EmitterData data)
{
	srand(time(NULL));

	active = true;
	this->pos = pos;
	this->data = data;

	// Calculates the particle cap
	maximumParticlesPerFrame = data.emitNumber + data.emitNumMult;

	// Life timer start
	if (this->data.emitLifetime != -1.0f && this->data.emitLifetime > 0.0f)
	{
		lifeTimer.Start();
	}

	// Clear the particle list
	ListItem<Particle*>* p = particlePool.start;
	while (p != nullptr)
	{
		ListItem<Particle*>* pNext = p->next;
		DestroyParticle(p->data);
		p = pNext;
	}
	particlePool.Clear();
}

Emitter::~Emitter()
{}

bool Emitter::CleanUp()
{
	LOG("Emitter delete");

	ListItem<Particle*>* p = particlePool.start;
	while (p != nullptr)
	{
		if (p->next != nullptr)
		{
			ListItem<Particle*>* pNext = p->next;
			DestroyParticle(p->data);
			p = pNext;
		}
		else
		{
			DestroyParticle(p->data);
			break;
		}
	}
	particlePool.Clear();

	return true;
}

bool Emitter::Update(float dt)
{
	if (active)
	{
		// Particle generation
		int emissionMultiplier = (int)(data.emitNumber + data.emitNumMult * GenerateRandNum(data.randEmitMult.x, data.randEmitMult.y));

		for (int i = 1; i <= emissionMultiplier; i++)
		{
			float pRandInitialSpeed = data.initialSpeed * GenerateRandNum(data.randInitialSpeed.x, data.randInitialSpeed.y);
			float pRandFinalSpeed = data.finalSpeed * GenerateRandNum(data.randFinalSpeed.x, data.randFinalSpeed.y);
			float pRandAngle = GenerateRandNum(data.angleRange.x, data.angleRange.y);
			fPoint pRandInitialSize;
			pRandInitialSize.x = data.initialSize.x * GenerateRandNum(data.randInitialSize.x, data.randInitialSize.y);
			pRandInitialSize.y = data.initialSize.y * GenerateRandNum(data.randInitialSize.x, data.randInitialSize.y);
			fPoint pRandFinalSize;
			pRandFinalSize.x = data.finalSize.x * GenerateRandNum(data.randFinalSize.x, data.randFinalSize.y);
			pRandFinalSize.y = data.finalSize.y * GenerateRandNum(data.randFinalSize.x, data.randFinalSize.y);
			fPoint pStartingSize;
			pStartingSize.x = GenerateRandNum(pRandInitialSize.x, pRandFinalSize.x);
			pStartingSize.y = GenerateRandNum(pRandInitialSize.y, pRandFinalSize.y);
			double randRotationSpeed = data.rotSpeed * GenerateRandNum(data.randRotSpeed.x, data.randRotSpeed.y);

			Particle* p = new Particle(pos, pRandInitialSpeed, pRandFinalSpeed, pRandAngle, randRotationSpeed, pStartingSize, data.finalSize, data.maxParticleLife, data.texRect, data.initialColor, data.finalColor, data.blendMode, data.vortexActive, data.halfTex);
			particlePool.Add(p);
		}
	}

	// Emission timing calculations
	if (data.emitLifetime > 0.0f)
	{
		if (lifeTimer.Read() >= data.emitLifetime)
		{
			active = false;
			data.emitLifetime = 0.0f;
		}
	}

	// Update all alive particles
	ListItem<Particle*>* p = particlePool.start;
	while (p != nullptr)
	{
		if (p->data->IsAlive())
		{
			p->data->Update(dt);
		}
		p = p->next;
	}

	return true;
}

bool Emitter::PostUpdate()
{
	bool ret = true;
	ListItem<Particle*>* p = particlePool.start;
	while (p != nullptr)
	{
		if (!p->data->IsAlive())
		{
			DestroyParticle(p->data);
		}
		else
		{
			if (!p->data->Draw())
			{
				ret = false;
			}
		}
		p = p->next;
	}
	if (particlePool.Count() == 0)
	{
		app->particles->RemoveEmitter(this);
	}

	return ret;
}

float Emitter::GenerateRandNum(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float result = random * diff;

	return min + result;
}

void Emitter::MoveEmitter(fPoint newPos)
{
	pos = newPos;
}

void Emitter::SetVortexActive(bool sensitive)
{
	data.vortexActive = sensitive;
}

bool Emitter::GetVortexActive()
{
	return data.vortexActive;
}

void Emitter::DestroyParticle(Particle* p)
{
	int i = particlePool.Find(p);
	RELEASE(p);
	particlePool.Del(particlePool.At(i));
}