#ifndef __MODULE_ENTITY_MANAGER_H__
#define __MODULE_ENTITY_MANAGER_H__

#include "Module.h"

#include "Entity.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "List.h"

#define MAX_ENTITIES 100

class Entity;

enum DialogueFlags
{
	TALKING_TO,
	FINISHED_TALK,
	FINISHED_TALK_REQUEST,
	ACTIVE
};

class EntityManager : public Module
{
public:
    // Constructor
    EntityManager();

    // Destructor
    virtual ~EntityManager();

    // Called when program is executed
    void Init();

    // Called before render is available
    bool Awake(pugi::xml_node&);

    // Called before the first frame
    // Loads the necessary textures for the entities
    bool Start();

    // Called each loop iteration
    bool Update(float dt);

    // Called after all Updates
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

    // Create/Destroy entity
    Entity* CreateEntity(int x, int y, EntityType type, EntityId id, Stats stats, NpcId npcId = NpcId::NONE, Entity* playerPointer = nullptr, ItemId itemId = ItemId::NONE, int count = 0, PuzzleId puzzleId = PuzzleId::NONE, BlockerId blockerId = BlockerId::NONE);
    void DestroyEntity(Entity* entity);

    // Calls Update for every entity
    bool UpdateAll(float dt, bool doLogic);

    // Collision response
    void OnCollision(Collider* c1, Collider* c2);

    // On mouse click response
    bool OnGuiMouseClickEvent(GuiControl* control);

    // Load/Save
    bool Load(pugi::xml_node&);
    bool Save(pugi::xml_node&);

public:
    List<Entity*> entities;

    bool doLogic = false;

    char folderTexture[TEXT_LEN] = { 0 };
    char folderAudioFx[TEXT_LEN] = { 0 };
    char folderMap[TEXT_LEN] = { 0 };

    // textures
    SDL_Texture* playerTex = nullptr;
    SDL_Texture* grandpaTex = nullptr;
    SDL_Texture* NPCTex = nullptr;
    SDL_Texture* enemiesTex = nullptr;
    SDL_Texture* itemAtlas = nullptr;
    SDL_Texture* puzzleTex = nullptr;

    // sounds
    int interactCat = -1;
    int interactGrandpa = -1;
    int interactHero = -1;
    int interactShop = -1;
    int footstepFx = -1;
    int itemFx = -1;
    int itemCollectedFx = -1;
    int rockFx = -1;
    int iceRockFx = -1;
    int solvedFx = -1;
    int playerFx = -1;

    bool inPause = false;

    // Some options for dialogs.

    int flagsShopkeeper = 0;
    int flagsCat = 0;
    int flagsSuperhero = 0;
    int flagsGrandpa = 0;

    bool takingItem = false;
    float dialogCounter = 0.0f;
    char itemPasser[TEXT_LEN] = { 0 };
};

#endif // __MODULE_ENTITY_MANAGER_H__