#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <box2d.h>
#include "InitialSetup.h"
#include "Constants.h"

class Shape
{
protected:

public:
    virtual void displayShape() = 0;
};