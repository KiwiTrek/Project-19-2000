#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

class Entity;

class SceneGameplay : public Scene
{
public:

    SceneGameplay();
    virtual ~SceneGameplay();

    bool Load();

    bool Update(float dt);

    bool Draw();

    bool Unload();

private:

    Entity* player = nullptr;
};

#endif // __SCENEGAMEPLAY_H__