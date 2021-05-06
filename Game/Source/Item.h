#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "CombatEntity.h"

class CombatEntity;

class Item
{
public:
    Item(ItemId id, Attack effect, int count);

    Item();

    void Use(CombatEntity* target);

    void UpdateCountText();

public:
    ItemId id;
    Attack effect;
    SString countText;
    int count;
    SDL_Rect texSec = { 288,416,32,32 };
    GuiControl* button;
};

class ItemEntity : public Entity
{
public:
	// Constructor
	ItemEntity(int x, int y, ItemId id, int count);

	// Called each loop iteration
	bool Update(float dt);

	// Blit
	bool Draw();

	void OnCollision(Collider* c1, Collider* c2);

private:

    Item item;
    int itemCollectedFx;

};

#endif // !__ITEM_H__