// initial setup for the simulator
#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <box2d.h>

extern b2World* world;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern b2Body* selectedBody;