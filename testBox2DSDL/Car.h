#pragma once
#include "Square.h"
#include "Circle.h"

class Car
{
    Square* sq1;
    Circle* c1;

public:
    b2Body* carBody;
    b2Body* wheel1;
    b2Body* wheel2;
    b2Body* wheel3;
    b2Body* wheel4;
    b2RevoluteJoint* joint1;
    b2RevoluteJoint* joint2;
    b2RevoluteJoint* joint3;
    b2RevoluteJoint* joint4;

    void addCar(int x, int y, int w, int h);
};