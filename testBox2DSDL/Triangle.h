#pragma once
#include "Shape.h"

class Triangle : public Shape
{
public:
    b2Body* addTriangle(int x, int y, int sideLength, bool dyn = true);

    void displayShape();
};