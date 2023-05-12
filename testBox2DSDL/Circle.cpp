#include "Circle.h"

// to add a Circle and define its parameters in Box2D following the Box2D standard practice
b2Body* Circle::addCircle(int x, int y, int r, bool dyn)
{
    // defines a body
    b2BodyDef bodydef;
    bodydef.position.Set(x * P2M, y * P2M);

    if (dyn) // if the 'dyn' parameter passed is true, the object is created as a dynamic object that can react with physics
        bodydef.type = b2_dynamicBody;

    // creates a body
    b2Body* body = world->CreateBody(&bodydef);

    // defining parameters for a circle
    b2CircleShape shape;
    shape.m_radius = r * P2M;
    shape.m_p.Set(0, 0);

    // defining fixture
    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 0.5;
    fixturedef.friction = 0.8;
    fixturedef.restitution = 1.0;

    // creating fixture on the body
    body->CreateFixture(&fixturedef);

    return body;
}

// to draw the added circle in the SDL2 environment
void Circle::drawCircle(SDL_Renderer* renderer, b2Vec2 center, float r, b2Vec2 offset = { 0,0 })
{
    const int PRECISION = 30;
    SDL_SetRenderDrawColor(renderer, 3, 37, 126, 255);
    SDL_Point vertices[PRECISION];
    for (int i = 0; i < PRECISION; i++)
    {
        float theta = (i / (float)PRECISION) * 2 * b2_pi;
        float x = r * cos(theta);
        float y = r * sin(theta);
        vertices[i] = { (int)((center.x + offset.x + x) * M2P), (int)((center.y + offset.y + y) * M2P) };
    }
    SDL_RenderDrawLines(renderer, vertices, PRECISION);
}

void Circle::displayShape() 
{

}
