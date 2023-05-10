#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <box2d.h>
#include "InitialSetup.h"
#include "Constants.h"
#include "Shape.h"
#include "Square.h"
#include "Triangle.h"
#include "Circle.h"
#include "Car.h"
#include "Borders.h"

Square square;
Triangle triangle;
Circle circle;


void selectBody(b2Vec2 point)
{
    // Check if the mouse is hovering over any of the bodies in the physics world
    for (b2Body* body = world->GetBodyList(); body; body = body->GetNext())
    {
        if (body->GetFixtureList())
        {
            if (body->GetFixtureList()->TestPoint(point))
            {
                selectedBody = body;
                break;
            }
        }
    }
}

void init()
{
    world = new b2World(b2Vec2(0.0f, 9.81f));
    Borders b2;
    b2.addBorders();
}

void destroyObjects()
{
    b2Body* body = world->GetBodyList();  // get the first body
    while (body != nullptr) {
        b2Body* next = body->GetNext();   // get the next body
        world->DestroyBody(body);         // destroy the current body
        body = next;                      // move to the next body
    }
}

void display()
{
    SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);
    SDL_RenderClear(renderer);

    b2Body* tmp = world->GetBodyList();

    while (tmp)
    {
        if (tmp->GetFixtureList()->GetShape()->GetType() == b2Shape::e_circle)
        {
            b2CircleShape* c = ((b2CircleShape*)tmp->GetFixtureList()->GetShape());
            circle.drawCircle(renderer, tmp->GetWorldCenter(), c->m_radius, c->m_p);
        }
        else if (tmp->GetFixtureList()->GetShape()->GetType() == b2Shape::e_polygon)
        {
            b2PolygonShape* poly = (b2PolygonShape*)tmp->GetFixtureList()->GetShape();
            if (poly->m_count == 4) {
                b2Vec2 vertices[4];
                for (int i = 0; i < 4; i++)
                {
                    vertices[i] = tmp->GetWorldPoint(poly->m_vertices[i]);
                }
                square.drawRect(vertices, tmp->GetWorldCenter());
            }
            if (poly->m_count == 3) {
                b2Vec2 vertices[3];
                for (int i = 0; i < 3; i++)
                {
                    vertices[i] = tmp->GetWorldPoint(poly->m_vertices[i]);
                }
                triangle.drawTriangle(vertices, tmp->GetWorldCenter());
            }
        }
        tmp = tmp->GetNext();
    }

    SDL_RenderPresent(renderer);
}


int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Box2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    Uint32 start;
    SDL_Event event;
    bool running = true;

    init();

    bool addingTriangle = false;
    bool addingRect = false;
    bool addingCircle = false;
    bool windEnabled = false;
    bool gravityEnabled = true;
    bool leftMouseDown = false; // Flag to keep track of whether left mouse button is down
    bool rotateEnabled = false; // Flag to keep track of whether rotation is enabled or disabled
 
    Circle* cir = new Circle;
    Square* sq = new Square;
    Triangle* tri = new Triangle;
    Borders* b1 = new Borders;
    Car* car = new Car;

    while (running)
    {
        start = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (addingRect) {
                        sq->addRect(event.button.x, event.button.y, 15, 15, true);

                    }
                    else if (addingTriangle) {
                        tri->addTriangle(event.button.x, event.button.y, 1, true);
                    }
                    else if (addingCircle) {
                        cir->addCircle(event.button.x, event.button.y, 8, true);
                    }
                }
                if (event.button.button == SDL_BUTTON_LEFT) {
                    selectBody(b2Vec2(event.button.x * P2M, event.button.y * P2M));
                    leftMouseDown = true; // Set flag to indicate left mouse button is down
                }
                break;
            case SDL_MOUSEMOTION:
                if (selectedBody && leftMouseDown) // Update selected body position only if left mouse button is down
                {
                    selectedBody->SetTransform(b2Vec2(event.motion.x * P2M, event.motion.y * P2M), selectedBody->GetAngle());
                }
                break;
            case SDL_MOUSEBUTTONUP:
                selectedBody = nullptr;
                leftMouseDown = false; // Reset flag when left mouse button is released
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_s) {
                    addingRect = true;
                    addingTriangle = false;
                    addingCircle = false;
                }
                else if (event.key.keysym.sym == SDLK_t) {
                    addingTriangle = true;
                    addingRect = false;
                    addingCircle = false;
                }
                else if (event.key.keysym.sym == SDLK_c) {
                    addingCircle = true;
                    addingRect = false;
                    addingTriangle = false;
                }
                else if (event.key.keysym.sym == SDLK_w) {

                    windEnabled = !windEnabled;

                    if (windEnabled) {
                        world->SetGravity(b2Vec2(9.81f, 9.81f));
                    }
                    else {
                        world->SetGravity(b2Vec2(0.0f, 9.81f));
                    }
                }
                else if (event.key.keysym.sym == SDLK_g) {

                    gravityEnabled = !gravityEnabled;

                    if (gravityEnabled) {
                        world->SetGravity(b2Vec2(0.0f, 9.81f));
                    }
                    else {
                        world->SetGravity(b2Vec2(0.0f, 0.0f));
                    }
                }
                else if (event.key.keysym.sym == SDLK_u)
                {
                    world->SetGravity(b2Vec2(0.0f, -9.81f));
                }
                else if (event.key.keysym.sym == SDLK_b)
                {
                    cir->addCircle(WIDTH / 7, HEIGHT / 7, 2.5, true);
                }
                else if (event.key.keysym.sym == SDLK_r)
                {
                    // destroy all Box2D objects
                    destroyObjects();
                    //add borders again
                    b1->addBorders();
                }
                else if (event.key.keysym.sym == SDLK_p) { // Toggle rotation
                    rotateEnabled = !rotateEnabled;

                    if (!rotateEnabled) {

                        for (b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext()) {

                            if (body->GetType() == b2_dynamicBody) {
                                body->SetAngularVelocity(0); // Set the angular velocity of the body to 0
                            }
                        }
                    }

                }
                else if (event.key.keysym.sym == SDLK_m) { // Toggle rotation
                    car->addCar(WIDTH - 350, HEIGHT / 2, 90, 30);
                }
                else if (event.key.keysym.sym == SDLK_LEFT) { // Move car backwards
                    car->carBody->ApplyForceToCenter(b2Vec2(-150.0f, 0.0f), true);
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) { // Move car forwards
                    car->carBody->ApplyForceToCenter(b2Vec2(150.0f, 0.0f), true);
                }
                else if (event.key.keysym.sym == SDLK_UP) { // Move car upwards in zero gravity
                    car->carBody->ApplyForceToCenter(b2Vec2(.0f, -150.0f), true);
                }
                else if (event.key.keysym.sym == SDLK_DOWN) { // Move car downwards in zero gravity
                    car->carBody->ApplyForceToCenter(b2Vec2(0.0f, 150.0f), true);
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                break;
            }
        }

        // Update selected body position if left mouse button is still down
        if (selectedBody && leftMouseDown)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            selectedBody->SetTransform(b2Vec2(mouseX * P2M, mouseY * P2M), selectedBody->GetAngle());
        }

        // Apply rotation if enabled
        if (rotateEnabled) {
            // Apply rotation to each object
            for (b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext()) {
                // Check if body is a dynamic type
                if (body->GetType() == b2_dynamicBody) {
                    body->SetAngularVelocity(50); // Set the angular velocity of the body to 50
                }
            }
        }

        display();
        world->Step(1.0f / 60.0f, 8, 3);  // update
        if (1000.0f / 60.0f > SDL_GetTicks() - start)
            SDL_Delay(1000.0f / 60.0f - (SDL_GetTicks() - start));
    }

    delete sq;
    delete b1;
    delete cir;
    delete tri;
    delete car;
    //carBody = nullptr;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}