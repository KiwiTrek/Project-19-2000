#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"

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

    int state;
    float timeCounter;
    float logoAlpha;
};

#endif // __SCENELOGO_H__