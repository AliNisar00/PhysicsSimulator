#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <box2d.h>
#include "Shape.h"

class Circle : public Shape
{
public:
    b2Body* addCircle(int x, int y, int r, bool dyn = true);

    void displayShape();
};