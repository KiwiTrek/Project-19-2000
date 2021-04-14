#ifndef __SCENEDEVROOM_H__
#define __SCENEDEVROOM_H__

#include "Scene.h"
#include "Entity.h"
#include "List.h"

class Entity;
class SceneCombat;
class DialogSystem;

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
    //COMBAT
    SceneCombat* combatScene;
private:

    Entity* player = nullptr;
    Font* dialogueFont = nullptr;
    DialogSystem* dialogSystem = nullptr;
};

#endif // __SCENEDEVROOM_H__

