#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"

class CombatEntity;

class Item
{
public:
    Item(Attack effect, int count) : effect(effect), count(count)
    {
        UpdateCountText();
    }

    Item()
    {}

    void Use(CombatEntity* target)
    {
        LOG("ITEM USE %s", effect.attackName.GetString());
        switch (effect.type)
        {
        case AttackType::DAMAGE:
            break;
        case AttackType::BUFF:
            break;
        case AttackType::HEAL:
            break;
        case AttackType::TAUNT:
            break;
        default:
            break;
        }
        count--;
        UpdateCountText();
    }

    void UpdateCountText()
    {
        switch (count)
        {
        case 1:
            countText.Clear();
            countText.Create("x1");
            break;
        case 2:
            countText.Clear();
            countText.Create("x2");
            break;
        case 3:
            countText.Clear();
            countText.Create("x3");
            break;
        case 4:
            countText.Clear();
            countText.Create("x4");
            break;
        case 5:
            countText.Clear();
            countText.Create("x5");
            break;
        default:
            break;
        }
    }

public:
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

	ItemId itemId = ItemId::NONE;
    Item item;
};

#endif // !__ITEM_H__