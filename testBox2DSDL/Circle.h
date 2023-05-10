#pragma once
#include "Shape.h"

class Circle : public Shape
{
public:
    b2Body* addCircle(int x, int y, int r, bool dyn = true);

    void displayShape();
};