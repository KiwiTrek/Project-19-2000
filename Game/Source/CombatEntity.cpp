#include "CombatEntity.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"
#include "Animation.h"

CombatEntity::CombatEntity(int x, int y, EntityId id, Stats stats) : Entity(x, y, EntityType::COMBAT_ENTITY, id, stats)
{
	this->entityRect = { x,y,128,128 };

	LOG("Init Enemy");
	pendingToDelete = false;

	// Animation

	//idle.PushBack({ 0,0,38,48 });

	//currentAnim = &idle;

	// depending on the id each combat entity will have a diferent pool of attacks/skills
	switch (id)
	{
	case EntityId::MC:
		break;
	case EntityId::VIOLENT:
		break;
	case EntityId::STUBBORN:
		break;
	case EntityId::KIND:
		break;
	case EntityId::ENEMY_1:
	{
		AttackPool.Add(&Attack("auch", AttackType::DAMAGE, Stats(10)));							// example of an attack
		break;
	}
	case EntityId::ENEMY_2:
		break;
	case EntityId::ENEMY_3:
		break;
	case EntityId::ENEMY_4:
		break;
	case EntityId::ENEMY_5:
		break;
	default:
		break;
	}
}

bool CombatEntity::Update(float dt)
{
	return true;
}

bool CombatEntity::Draw()
{
	app->render->DrawRectangle(entityRect, 255, 0, 0, 255);
	//app->render->DrawTexture(app->entities->playerTex, playerPos.x, playerPos.y, false, &currentAnim->GetCurrentFrame(), invert);

	return true;
}