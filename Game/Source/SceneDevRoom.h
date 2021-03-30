#ifndef __SCENEDEVROOM_H__
#define __SCENEDEVROOM_H__

#include "Scene.h"

class Entity;

class SceneDevRoom : public Scene
{
public:

    SceneDevRoom();
    virtual ~SceneDevRoom();

    bool Load();

    bool Update(float dt);

    bool Draw();

    bool Unload();

public:
    SDL_Texture* tileSetTex;
private:

    Entity* player = nullptr;
    Entity* enemyTest = nullptr;
};

#endif // __SCENEDEVROOM_H__

