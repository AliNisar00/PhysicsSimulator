#include "display.h"

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