#include "Triangle.h"

b2Body* Triangle::addTriangle(int x, int y, int sideLength, bool dyn)
{
    b2BodyDef bodydef;
    bodydef.position.Set(x * P2M, y * P2M);
    if (dyn)
        bodydef.type = b2_dynamicBody;
    b2Body* body = world->CreateBody(&bodydef);

    b2Vec2 vertices[3];
    float height = sideLength * sqrt(3.0f) / 2.0f;
    vertices[0].Set(-sideLength / 2.0f, -height / 2.0f);
    vertices[1].Set(0.0f, height / 2.0f);
    vertices[2].Set(sideLength / 2.0f, -height / 2.0f);

    b2PolygonShape shape;
    shape.Set(vertices, 3);

    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 1.0;
    fixturedef.restitution = 1.0;
    body->CreateFixture(&fixturedef);
    return body;
}

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
    b2Body* tmp = world->GetBodyList();

    while (tmp)
    {
        if (tmp->GetFixtureList()->GetShape()->GetType() == b2Shape::e_polygon)
        {
            b2PolygonShape* poly = (b2PolygonShape*)tmp->GetFixtureList()->GetShape();
            if (poly->m_count == 3) {
                b2Vec2 vertices[3];
                for (int i = 0; i < 3; i++)
                {
                    vertices[i] = tmp->GetWorldPoint(poly->m_vertices[i]);
                }
                drawTriangle(vertices, tmp->GetWorldCenter());
            }
        }
        tmp = tmp->GetNext();
    }
}
