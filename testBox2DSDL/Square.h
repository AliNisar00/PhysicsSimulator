#pragma once
#include "Shape.h"

class Square : public Shape
{
public:
    b2Body* addRect(int x, int y, int w, int h, bool dyn = true);

    void displayShape();
};