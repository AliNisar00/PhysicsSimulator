#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <box2d.h>
#include "Shape.h"

class Triangle : public Shape
{
public:
    b2Body* addTriangle(int x, int y, int sideLength, bool dyn = true);

    void displayShape();
};