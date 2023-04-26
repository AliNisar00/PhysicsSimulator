#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <box2d.h>

//const int RADIUS = 25;
const int WIDTH = 640;
const int HEIGHT = 480;
const float M2P = 20;
const float P2M = 1 / M2P;
b2World* world;
SDL_Window* window;
SDL_Renderer* renderer;

void putPixel(SDL_Renderer* renderer, int x, int y, int r, int g, int b)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, x, y);
}

void swapValue(int& a, int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

void drawLine(SDL_Renderer* renderer, int x0, int y0, int x1, int y1)
{
    int tmp;
    bool step;

    step = abs(y1 - y0) > abs(x1 - x0);
    if (step)
    {
        swapValue(x0, y0);
        swapValue(x1, y1);
    }

    if (x0 > x1)
    {
        swapValue(x1, x0);
        swapValue(y1, y0);
    }
    float error = 0.0;
    float y = y0;
    float roundError = (float)abs(y1 - y0) / (x1 - x0);
    int ystep = (y1 > y0 ? 1 : -1);
    for (int i = x0; i < x1; i++)
    {
        if (step)
            putPixel(renderer, y, i, 255, 255, 255);
        else
            putPixel(renderer, i, y, 255, 255, 255);
        error += roundError;
        if (error >= 0.5)
        {
            y += ystep;
            error -= 1;
        }
    }
}

void rotateTranslate(b2Vec2& vector, const b2Vec2& center, float angle)
{
    b2Vec2 tmp;
    tmp.x = vector.x * cos(angle) - vector.y * sin(angle);
    tmp.y = vector.x * sin(angle) + vector.y * cos(angle);
    vector = tmp + center;
}

b2Body* addCircle(int x, int y, int r, bool dyn = true)
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
    body->CreateFixture(&fixturedef);
    body->GetUserData();
    return body;
}

void drawCircle(SDL_Renderer* renderer, b2Vec2 center, float r, b2Vec2 offset = { 0,0 })
{
    const int PRECISION = 200;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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



b2Body* addRect(int x, int y, int w, int h, bool dyn = true)
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
    body->CreateFixture(&fixturedef);
    return body;
}

void drawSquare(b2Vec2* points, b2Vec2 center, float angle)
{
    for (int i = 0; i < 4; i++)
        drawLine(renderer, points[i].x * M2P, points[i].y * M2P, points[(i + 1) > 3 ? 0 : (i + 1)].x * M2P, points[(i + 1) > 3 ? 0 : (i + 1)].y * M2P);
}


void init()
{
    world = new b2World(b2Vec2(0.0f, 9.81f));
    addRect(WIDTH / 2, HEIGHT - 50, WIDTH, 30, false);
}

void display()
{
    SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);
    SDL_RenderClear(renderer);
    b2Body* tmp = world->GetBodyList();
    b2Vec2 points[4];
    while (tmp)
    {
        if (tmp->GetFixtureList()->GetShape()->GetType() == b2Shape::e_circle)
        {
            b2CircleShape* c = ((b2CircleShape*)tmp->GetFixtureList()->GetShape());
            drawCircle(renderer, tmp->GetWorldCenter(), c->m_radius, c->m_p);
        }
        else {
            for (int i = 0; i < ((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->m_count; i++) {
                points[i] = tmp->GetWorldPoint(((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->m_vertices[i]);
            }
            drawSquare(points, tmp->GetWorldCenter(), tmp->GetAngle());
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
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    addRect(event.button.x, event.button.y, 25, 25, true);
                }
                else if (event.button.button == SDL_BUTTON_LEFT) {
                    addCircle(event.button.x, event.button.y, 15, true);
                }
                break;
            }
        }
        display();
        world->Step(1.0f / 60.0f, 8, 3);  // update
        if (1000.0f / 60.0f > SDL_GetTicks() - start)
            SDL_Delay(1000.0f / 60.0f - (SDL_GetTicks() - start));
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}