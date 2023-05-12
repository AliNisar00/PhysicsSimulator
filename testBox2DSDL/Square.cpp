#include "Square.h"

// to add a Circle and define its parameters in Box2D following the Box2D standard practice
b2Body* Square::addRect(int x, int y, int w, int h, bool dyn)
{
    // defines a body
    b2BodyDef bodydef;
    bodydef.position.Set(x * P2M, y * P2M);

    if (dyn) // if the 'dyn' parameter passed is true, the object is created as a dynamic object that can react with physics
        bodydef.type = b2_dynamicBody;

    // creates a body
    b2Body* body = world->CreateBody(&bodydef);

    // defining parameters for a box i.e. square (in our case)
    b2PolygonShape shape;
    shape.SetAsBox(P2M * w / 2, P2M * h / 2);

    // defining fixture
    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 1.0;
    fixturedef.restitution = 0.0;

    // creating fixture on the body
    body->CreateFixture(&fixturedef);

    return body;
}

// to draw the added rectangle in the SDL2 environment
void Square::drawRect(b2Vec2 vertices[], b2Vec2 center)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Point points[5];
    for (int i = 0; i < 4; i++)
    {
        points[i] = { (int)(vertices[i].x * M2P), (int)(vertices[i].y * M2P) };
    }
    points[4] = { (int)(vertices[0].x * M2P), (int)(vertices[0].y * M2P) };
    SDL_RenderDrawLines(renderer, points, 5);
}

void Square::displayShape()
{

}
