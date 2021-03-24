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

private:

    Entity* player = nullptr;
};

#endif // __SCENEDEVROOM_H__

