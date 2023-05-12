#include "Triangle.h"

b2Body* Triangle::addTriangle(int x, int y, int sideLength, bool dyn)
{
    // defines a body
    b2BodyDef bodydef;
    bodydef.position.Set(x * P2M, y * P2M);

    if (dyn) // if the 'dyn' parameter passed is true, the object is created as a dynamic object that can react with physics
        bodydef.type = b2_dynamicBody;

    // creates a body
    b2Body* body = world->CreateBody(&bodydef);

    // defining parameters for a triangle
    b2Vec2 vertices[3];
    float height = sideLength * sqrt(3.0f) / 2.0f;
    vertices[0].Set(-sideLength / 2.0f, -height / 2.0f);
    vertices[1].Set(0.0f, height / 2.0f);
    vertices[2].Set(sideLength / 2.0f, -height / 2.0f);
    b2PolygonShape shape;
    shape.Set(vertices, 3);

    // defining fixture
    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 1.0;
    fixturedef.restitution = 1.0;

    // creating fixture on the body
    body->CreateFixture(&fixturedef);

    return body;
}

// to draw the added triangle in the SDL2 environment
void Triangle::drawTriangle(b2Vec2 vertices[], b2Vec2 center)
{
    SDL_SetRenderDrawColor(renderer, 0, 110, 51, 255);
    SDL_Point points[4];
    for (int i = 0; i < 3; i++)
    {
        points[i] = { (int)(vertices[i].x * M2P), (int)(vertices[i].y * M2P) };
    }
    points[3] = { (int)(vertices[0].x * M2P), (int)(vertices[0].y * M2P) };
    SDL_RenderDrawLines(renderer, points, 4);
}

void Triangle::displayShape() 
{

}


