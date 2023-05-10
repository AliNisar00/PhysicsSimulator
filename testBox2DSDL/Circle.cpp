#include "Circle.h"

b2Body* Circle::addCircle(int x, int y, int r, bool dyn)
{
    b2BodyDef bodydef;
    bodydef.position.Set(x * P2M, y * P2M);
    if (dyn)
        bodydef.type = b2_dynamicBody;
    b2Body* body = world->CreateBody(&bodydef);

    b2CircleShape shape;
    shape.m_radius = r * P2M;
    shape.m_p.Set(0, 0);

    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 0.5;
    fixturedef.friction = 0.8;
    fixturedef.restitution = 1.0;
    body->CreateFixture(&fixturedef);
    return body;
}

void Circle::displayShape() 
{

}
