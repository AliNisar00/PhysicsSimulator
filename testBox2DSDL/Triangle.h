#pragma once
#include "Shape.h"

class Triangle : public Shape
{
public:
    b2Body* addTriangle(int x, int y, int sideLength, bool dyn = true);

    void displayShape();
    void drawTriangle(b2Vec2 vertices[], b2Vec2 center);
};