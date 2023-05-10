#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <box2d.h>
#include "Shape.h"

class Square : public Shape
{
public:
    b2Body* addRect(int x, int y, int w, int h, bool dyn = true);

    void displayShape();
};