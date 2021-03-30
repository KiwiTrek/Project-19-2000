//#include "Application.h"
//#include "ModuleSceneIntro.h"
//#include "ModulePlayer.h"
//#include "ModuleInput.h"
//#include "ModulePhysics.h"
//#include "math.h"
//
//ModulePhysics::ModulePhysics(Application* app, bool startEnabled) : Module(app, startEnabled)
//{
//    debug = true;
//
//    matrix[Collider::Type::SOLID][Collider::Type::SOLID] = false;
//    matrix[Collider::Type::SOLID][Collider::Type::WATER] = false;
//    matrix[Collider::Type::SOLID][Collider::Type::MOON] = false;
//    matrix[Collider::Type::SOLID][Collider::Type::PLAYER] = true;
//    matrix[Collider::Type::SOLID][Collider::Type::ASTEROID] = false;
//
//    matrix[Collider::Type::WATER][Collider::Type::SOLID] = false;
//    matrix[Collider::Type::WATER][Collider::Type::WATER] = false;
//    matrix[Collider::Type::WATER][Collider::Type::MOON] = false;
//    matrix[Collider::Type::WATER][Collider::Type::PLAYER] = true;
//    matrix[Collider::Type::WATER][Collider::Type::ASTEROID] = false;
//
//    matrix[Collider::Type::MOON][Collider::Type::SOLID] = false;
//    matrix[Collider::Type::MOON][Collider::Type::WATER] = false;
//    matrix[Collider::Type::MOON][Collider::Type::MOON] = false;
//    matrix[Collider::Type::MOON][Collider::Type::PLAYER] = true;
//    matrix[Collider::Type::MOON][Collider::Type::ASTEROID] = false;
//
//    matrix[Collider::Type::PLAYER][Collider::Type::SOLID] = true;
//    matrix[Collider::Type::PLAYER][Collider::Type::WATER] = true;
//    matrix[Collider::Type::PLAYER][Collider::Type::MOON] = true;
//    matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
//    matrix[Collider::Type::PLAYER][Collider::Type::ASTEROID] = true;
//
//    matrix[Collider::Type::ASTEROID][Collider::Type::SOLID] = false;
//    matrix[Collider::Type::ASTEROID][Collider::Type::WATER] = false;
//    matrix[Collider::Type::ASTEROID][Collider::Type::MOON] = false;
//    matrix[Collider::Type::ASTEROID][Collider::Type::PLAYER] = true;
//    matrix[Collider::Type::ASTEROID][Collider::Type::ASTEROID] = false;
//}
//
//// Destructor
//ModulePhysics::~ModulePhysics()
//{}
//
//bool ModulePhysics::Start()
//{
//    LOG("Creating Physics 2D environment");
//    gravity.SetToZero();
//    debug = false;
//    return true;
//}
//
//// 
//UpdateStatus ModulePhysics::PreUpdate()
//{
//    Collider* c1;
//    Collider* c2;
//
//    // Check collisions
//    p2List_item<Object*>* tmp = objects.GetFirst();
//    while (tmp != nullptr)
//    {
//        // skip empty colliders
//        if (tmp->data->collider == nullptr)
//        {
//            tmp = tmp->next;
//            continue;
//        }
//
//        c1 = tmp->data->collider;
//
//        // avoid checking collisions already checked
//        p2List_item<Object*>* tmp2 = tmp->next;
//        while (tmp2 != nullptr)
//        {
//            // skip empty colliders
//            if (tmp2->data->collider == nullptr)
//            {
//                tmp2 = tmp2->next;
//                continue;
//            }
//
//            c2 = tmp2->data->collider;
//
//            if (matrix[c1->type][c2->type] && Intersects(tmp->data, tmp2->data) && App->player->godMode == false)
//            {
//                ResolveCollisions(tmp->data, tmp2->data);
//                if (c1->listener)
//                    c1->listener->OnCollision(c1, c2);
//                if (c2->listener)
//                    c2->listener->OnCollision(c2, c1);
//            }
//            tmp2 = tmp2->next;
//        }
//        tmp = tmp->next;
//    }
//
//    return UPDATE_CONTINUE;
//}
//
//UpdateStatus ModulePhysics::Update(float dt)
//{
//    UpdateGravity();
//    p2List_item<Object*>* tmp = objects.GetFirst();
//    while (tmp != nullptr)
//    {
//        if (tmp->data->mass != 0.0f)
//            UpdatePhysics(tmp->data, dt);
//        tmp = tmp->next;
//    }
//    return UPDATE_CONTINUE;
//}
//
//// Called before quitting
//bool ModulePhysics::CleanUp()
//{
//    LOG("Freeing all colliders");
//    p2List_item<Object*>* tmp = objects.GetFirst();
//    while (tmp != nullptr)
//    {
//        p2List_item<Object*>* tmpNext = tmp->next;
//        RemoveObject(tmp->data);
//        tmp = tmpNext;
//    }
//
//    return true;
//}
//
//void ModulePhysics::UpdatePhysics(Object* object, float dt)
//{
//    // Update past variables
//    object->pastPos = object->pos;
//    object->pastSpeed = object->speed;
//
//    // Calculate acceleration
//    fPoint a = Force2Accel(object->force, object->mass);
//
//    // Calculate Verlet
//    object->pos.x = object->pos.x + object->speed.x * dt + ((gravity.x + a.x) * dt * dt * 0.5);
//    object->speed.x = object->speed.x + ((gravity.x + a.x) * dt);
//    object->pos.y = object->pos.y + object->speed.y * dt + ((gravity.y + a.y) * dt * dt * 0.5);
//    object->speed.y = object->speed.y + ((gravity.y + a.y) * dt);
//
//    // Reset force
//    object->force.SetToZero();
//
//    // Update the collider position
//    if (object->collider != nullptr)
//    {
//        if (object->shape == Object::Shape::CIRCLE)
//        {
//            Circle* tmp = (Circle*)object;
//            object->collider->SetPos(object->pos.x - tmp->radius, object->pos.y - tmp->radius, object->collider->rect.w, object->collider->rect.h);
//        }
//        else
//        {
//            object->collider->SetPos(object->pos.x, object->pos.y, object->collider->rect.w, object->collider->rect.h);
//        }
//    }
//
//    if (object->collider->type == Collider::Type::PLAYER)
//    {
//        if (object->speed.y < 0)
//        {
//            App->player->isMovingUp = true;
//            App->player->isMovingDown = false;
//        }
//        else
//        {
//            App->player->isMovingUp = false;
//            App->player->isMovingDown = true;
//        }
//    }
//}
//
//void ModulePhysics::ResolveCollisions(Object* A, Object* B)
//{
//    if (A->collider->type == Collider::Type::WATER || B->collider->type == Collider::Type::WATER)
//    {
//        if (A->collider->type == Collider::Type::WATER)
//        {
//            A->speed.SetToZero();
//            Buoyancy(A, B);
//        }
//        else
//        {
//            ResolveCollisions(B, A);
//        }
//    }
//    else
//    {
//        A->speed.SetToZero();
//        B->speed.SetToZero();
//        if (A->shape == Object::Shape::RECT && B->shape == Object::Shape::RECT)
//        {
//            iPoint frameDifB = B->pos - B->pastPos;
//            iPoint tmpB = B->pos;
//            iPoint frameDifA = A->pos - A->pastPos;
//            iPoint tmpA = A->pos;
//
//            while (Intersects(A, B))
//            {
//                if (frameDifB.x > 0)
//                {
//                    tmpB.x--;
//                    frameDifB.x--;
//                }
//                else if (frameDifB.x < 0)
//                {
//                    tmpB.x++;
//                    frameDifB.x++;
//                }
//                if (frameDifB.y > 0)
//                {
//                    tmpB.y--;
//                    frameDifB.y--;
//                }
//                else if (frameDifB.y < 0)
//                {
//                    tmpB.y++;
//                    frameDifB.y++;
//                }
//
//                if (frameDifA.x > 0)
//                {
//                    tmpA.x--;
//                    frameDifA.x--;
//                }
//                else if (frameDifA.x < 0)
//                {
//                    tmpA.x++;
//                    frameDifA.x++;
//                }
//                if (frameDifA.y > 0)
//                {
//                    tmpA.y--;
//                    frameDifA.y--;
//                }
//                else if (frameDifA.y < 0)
//                {
//                    tmpA.y++;
//                    frameDifA.y++;
//                }
//
//                A->collider->rect.x = tmpA.x;
//                A->collider->rect.y = tmpA.y;
//                B->collider->rect.x = tmpB.x;
//                B->collider->rect.y = tmpB.y;
//            }
//            B->pos = tmpB;
//            A->pos = tmpA;
//        }
//        else if (A->shape == Object::Shape::RECT && B->shape == Object::Shape::CIRCLE)
//        {
//            Circle* circle = (Circle*)B;
//            iPoint rPos;
//            float dist = ShortestDist(A, circle, rPos);
//
//            iPoint frameDifB = B->pos - B->pastPos;
//            iPoint tmpB = B->pos;
//            iPoint frameDifA = A->pos - A->pastPos;
//            iPoint tmpA = rPos;
//
//            while (dist < circle->radius)
//            {
//                if (frameDifB.x > 0)
//                {
//                    tmpB.x--;
//                    frameDifB.x--;
//                }
//                else if (frameDifB.x < 0)
//                {
//                    tmpB.x++;
//                    frameDifB.x++;
//                }
//                if (frameDifB.y > 0)
//                {
//                    tmpB.y--;
//                    frameDifB.y--;
//                }
//                else if (frameDifB.y < 0)
//                {
//                    tmpB.y++;
//                    frameDifB.y++;
//                }
//
//                if (frameDifA.x > 0)
//                {
//                    tmpA.x--;
//                    frameDifA.x--;
//                }
//                else if (frameDifA.x < 0)
//                {
//                    tmpA.x++;
//                    frameDifA.x++;
//                }
//                if (frameDifA.y > 0)
//                {
//                    tmpA.y--;
//                    frameDifA.y--;
//                }
//                else if (frameDifA.y < 0)
//                {
//                    tmpA.y++;
//                    frameDifA.y++;
//                }
//
//                dist = tmpA.DistanceTo(tmpB);
//            }
//            B->pos = tmpB;
//            A->pos += tmpA - rPos;
//        }
//        else if (A->shape == Object::Shape::CIRCLE && B->shape == Object::Shape::RECT)
//        {
//            ResolveCollisions(B, A);
//        }
//        else if (A->shape == Object::Shape::CIRCLE && B->shape == Object::Shape::CIRCLE)
//        {
//            Circle* circleA = (Circle*)A;
//            Circle* circleB = (Circle*)B;
//            float dist = A->pos.DistanceTo(B->pos);
//
//            iPoint frameDifB = B->pos - B->pastPos;
//            iPoint tmpB = B->pos;
//            iPoint frameDifA = A->pos - A->pastPos;
//            iPoint tmpA = A->pos;
//
//            while (dist < (circleA->radius + circleB->radius))
//            {
//                if (frameDifB.x > 0)
//                {
//                    tmpB.x--;
//                    frameDifB.x--;
//                }
//                else if (frameDifB.x < 0)
//                {
//                    tmpB.x++;
//                    frameDifB.x++;
//                }
//                if (frameDifB.y > 0)
//                {
//                    tmpB.y--;
//                    frameDifB.y--;
//                }
//                else if (frameDifB.y < 0)
//                {
//                    tmpB.y++;
//                    frameDifB.y++;
//                }
//
//                if (frameDifA.x > 0)
//                {
//                    tmpA.x--;
//                    frameDifA.x--;
//                }
//                else if (frameDifA.x < 0)
//                {
//                    tmpA.x++;
//                    frameDifA.x++;
//                }
//                if (frameDifA.y > 0)
//                {
//                    tmpA.y--;
//                    frameDifA.y--;
//                }
//                else if (frameDifA.y < 0)
//                {
//                    tmpA.y++;
//                    frameDifA.y++;
//                }
//
//                dist = tmpA.DistanceTo(tmpB);
//            }
//            B->pos = tmpB;
//            A->pos = tmpA;
//        }
//    }
//}
//
//void ModulePhysics::AddObject(Object* object)
//{
//    objects.Add(object);
//}
//
//void ModulePhysics::RemoveObject(Object* object)
//{
//    p2List_item<Object*>* tmp = objects.FindNode(object);
//    object->collider = nullptr;
//    objects.Del(tmp);
//}
//
//float ModulePhysics::ShortestDist(Object* A, Circle* B, iPoint& rPos)
//{
//    // find closest X
//    // left
//    if (A->collider->rect.x > B->pos.x)
//        rPos.x = A->collider->rect.x;
//    // right
//    else if (A->collider->rect.x + A->collider->rect.w < B->pos.x)
//        rPos.x = A->collider->rect.x + A->collider->rect.w;
//    // inside
//    else
//        rPos.x = B->pos.x;
//
//    // find closest Y
//    // top
//    if (A->collider->rect.y > B->pos.y)
//        rPos.y = A->collider->rect.y;
//    // bot
//    else if (A->collider->rect.y + A->collider->rect.h < B->pos.y)
//        rPos.y = A->collider->rect.y + A->collider->rect.h;
//    // inside
//    else
//        rPos.y = B->pos.y;
//
//    // compute distance
//    return rPos.DistanceTo(B->pos);
//}
//
//void Collider::SetPos(int x, int y, int w, int h)
//{
//    rect.x = x;
//    rect.y = y;
//    rect.w = w;
//    rect.h = h;
//}
//
//bool ModulePhysics::Intersects(Object* A, Object* B)
//{
//    if (A->shape == Object::Shape::RECT && B->shape == Object::Shape::RECT)
//    {
//        return (A->collider->rect.x < B->collider->rect.x + B->collider->rect.w
//            && A->collider->rect.x + A->collider->rect.w > B->collider->rect.x
//            && A->collider->rect.y < B->collider->rect.y + B->collider->rect.h
//            && A->collider->rect.h + A->collider->rect.y > B->collider->rect.y);
//    }
//    else if (A->shape == Object::Shape::RECT && B->shape == Object::Shape::CIRCLE)
//    {
//        iPoint tmp;
//        Circle* circle = (Circle*)B;
//        float dist = ShortestDist(A, circle, tmp);
//        return (dist < circle->radius);
//    }
//    else if (A->shape == Object::Shape::CIRCLE && B->shape == Object::Shape::RECT)
//    {
//        return Intersects(B, A);
//    }
//    else if (A->shape == Object::Shape::CIRCLE && B->shape == Object::Shape::CIRCLE)
//    {
//        Circle* circleA = (Circle*)A;
//        Circle* circleB = (Circle*)B;
//        float dist = A->pos.DistanceTo(B->pos);
//        return (dist < (circleA->radius + circleB->radius));
//    }
//}
//
//void ModulePhysics::Buoyancy(Object* A, Object* B)
//{
//    float volume;
//    if (A->collider->rect.y <= B->collider->rect.y)
//        volume = B->collider->rect.h * B->collider->rect.w;
//    else
//        volume = (float)abs((A->collider->rect.y - B->collider->rect.y) - B->collider->rect.h) * B->collider->rect.w;
//
//    float tmpForce = B->force.y + (B->mass * gravity.y - (WATER_DENSITY)*volume * gravity.y);
//    B->force.y = tmpForce;
//    B->speed.y *= DAMPEN;
//}
//
//void ModulePhysics::UpdateDrag(fPoint direction)
//{
//    dragDirection.x = (direction.x != 0 ? -1 * direction.x : 0);
//    dragDirection.y = (direction.y != 0 ? -1 * direction.y : 0);
//}
