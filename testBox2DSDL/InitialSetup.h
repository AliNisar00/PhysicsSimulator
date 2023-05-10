#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <box2d.h>

b2World* world;
SDL_Window* window;
SDL_Renderer* renderer;

b2Body* selectedBody = nullptr;