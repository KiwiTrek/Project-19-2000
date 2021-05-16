#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"
#include "Point.h"

#include "SDL/include/SDL.h"

class SceneLogo : public Scene
{
public:

	SceneLogo();
	virtual ~SceneLogo();

	bool Load();

	bool Update(float dt);

	bool Draw();

	bool Unload();

private:

	SDL_Texture* logo;
	iPoint pos;

	bool onceFx;
	bool onceOut;
	int logoFx;
	
	int state;
	float timeCounter;
	float timeVelocity;
};

#endif // __SCENELOGO_H__