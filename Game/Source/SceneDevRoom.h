#ifndef __SCENEDEVROOM_H__
#define __SCENEDEVROOM_H__

#include "Scene.h"
#include "Entity.h"
#include "List.h"

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

    //bool SortSpeed(bool isSorted);

    //bool IsCharacter(Entity* e);

public:
    SDL_Texture* tileSetTex;
private:

    Entity* player = nullptr;
    //Entity* enemyTest = nullptr;
    //Entity* enemyTest2 = nullptr;
    //Entity* playerTest = nullptr;

    //COMBAT VARS
    //List<Entity*> turnOrder;
};

#endif // __SCENEDEVROOM_H__

