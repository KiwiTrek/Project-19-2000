//#ifndef __OBJECT_H__
//#define __OBJECT_H__
//
//#include "Globals.h"
//#include "p2Point.h"
//
//class Collider;
//
//struct Object
//{
//    enum Shape
//    {
//        RECT = 0,
//        CIRCLE
//    };
//
//    Object(iPoint pos, fPoint speed, fPoint force, float mass = 0.0f, Shape shape = Shape::RECT, Collider* c = nullptr, float angle = 0.0f) : pos(pos), speed(speed), force(force), mass(mass), shape(shape), collider(c), angle(angle)
//    {}
//
//    Object()
//    {}
//
//    iPoint pos;
//    iPoint pastPos;
//    fPoint speed;
//    fPoint pastSpeed;
//    fPoint force;
//    float mass;
//    Shape shape;
//    Collider* collider;
//    float angle;
//};
//
//struct Circle : public Object
//{
//    int radius;
//};
//
//#endif // !__OBJECT_H__
