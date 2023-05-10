#include "Square.h"

b2Body* Square::addRect(int x, int y, int w, int h, bool dyn)
{
    b2BodyDef bodydef;
    bodydef.position.Set(x * P2M, y * P2M);
    if (dyn)
        bodydef.type = b2_dynamicBody;
    b2Body* body = world->CreateBody(&bodydef);

    b2PolygonShape shape;
    shape.SetAsBox(P2M * w / 2, P2M * h / 2);

    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 1.0;
    fixturedef.restitution = 0.0;
    body->CreateFixture(&fixturedef);
    return body;
}

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
    b2Body* tmp = world->GetBodyList();
    while (tmp)
    {
        if (tmp->GetFixtureList()->GetShape()->GetType() == b2Shape::e_polygon)
        {
            b2PolygonShape* poly = (b2PolygonShape*)tmp->GetFixtureList()->GetShape();
            if (poly->m_count == 4) {
                b2Vec2 vertices[4];
                for (int i = 0; i < 4; i++)
                {
                    vertices[i] = tmp->GetWorldPoint(poly->m_vertices[i]);
                }
                drawRect(vertices, tmp->GetWorldCenter());
            }
        }
        tmp = tmp->GetNext();
    }

}
