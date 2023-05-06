#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <box2d.h>

const int WIDTH = 640;
const int HEIGHT = 480;
const float M2P = 20;
const float P2M = 1 / M2P;
b2World* world;
SDL_Window* window;
SDL_Renderer* renderer;

// declaring a ground body pointer
b2Body* groundBody = nullptr;

b2Body* draggedObject = nullptr; // to keep track of currently dragged object
b2Vec2 mouseOffset; // to track the offset between an object's position and mouse position
b2MouseJoint* mouseJoint = nullptr;

// creating the ground body
b2BodyDef groundBodyDef;
groundBodyDef.type = b2_staticBody; // Set body type as static

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
    fixturedef.restitution = 0.8;
    body->CreateFixture(&fixturedef);
    //body->GetUserData();
    return body;
}

void drawCircle(SDL_Renderer* renderer, b2Vec2 center, float r, b2Vec2 offset = { 0,0 })
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

b2Body* addTriangle(int x, int y, int sideLength, bool dyn = true)
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
    body->CreateFixture(&fixturedef);
    return body;
}

void drawTriangle(b2Vec2 vertices[], b2Vec2 center)
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

void drawRect(b2Vec2 vertices[], b2Vec2 center)
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

void addBorders() {

    addRect(WIDTH / 2, HEIGHT - 30, 800, 10, false); //floor
    addRect(WIDTH / 30, HEIGHT - 90, 10, 800, false); //leftBorder
    addRect(618, HEIGHT - 90, 10, 800, false); //rightBorder
    addRect(WIDTH / 2, 25, 800, 10, false); //top

}

void init()
{
    world = new b2World(b2Vec2(0.0f, 9.81f));

    addBorders();

    for (int i = 0; i < 250; i++) {
        addCircle(WIDTH / 2, HEIGHT / 2, 8, true);
    }
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
            drawCircle(renderer, tmp->GetWorldCenter(), c->m_radius, c->m_p);
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
                drawRect(vertices, tmp->GetWorldCenter());
            }
            else if (poly->m_count == 3) {
                b2Vec2 vertices[3];
                for (int i = 0; i < 3; i++)
                {
                    vertices[i] = tmp->GetWorldPoint(poly->m_vertices[i]);
                }
                drawTriangle(vertices, tmp->GetWorldCenter());
            }
        }
        /* OPTIONAL CODE TO HAVE HIGHLIGHTING FOR SELECTED OBJECT BEING MOVED AROUND; CODE INCOMPLETE
        if (tmp == draggedObject) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // yellow outline for the dragged object
            // draw the shape based on its type (circle, rectangle, triangle)
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);  // default color for other objects
            // draw the shape based on its type (circle, rectangle, triangle)
        }
        */
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
                        addRect(event.button.x, event.button.y, 15, 15, true);
                    }
                    else if (addingTriangle) {
                        addTriangle(event.button.x, event.button.y, 1, true);
                    }
                    else if (addingCircle) {
                        addCircle(event.button.x, event.button.y, 8, true);
                    }
                }
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Check if the mouse is hovering over any object
                    b2Vec2 mousePos(event.button.x * P2M, event.button.y * P2M);
                    b2AABB aabb;
                    b2Vec2 d(0.001f, 0.001f);
                    aabb.lowerBound = mousePos - d;
                    aabb.upperBound = mousePos + d;
                    b2MouseJointDef md;
                    md.bodyA = world->GetGroundBody();
                    md.bodyB = nullptr;
                    md.target = mousePos;
                    md.maxForce = 1000.0f * draggedBody->GetMass();
                    draggedObject = nullptr;

                    // Query the world to find overlapping shapes
                    QueryCallback callback(mousePos);
                    world->QueryAABB(&callback, aabb);

                    if (callback.fixture) {
                        // Get the body associated with the fixture
                        b2Body* body = callback.fixture->GetBody();

                        // Ensure the body is dynamic (movable) and not a ground/body
                        if (body->GetType() == b2_dynamicBody && body != groundBody) {
                            draggedObject = body;
                            mouseOffset = body->GetLocalPoint(mousePos);

                            // Create a mouse joint to enable dragging
                            md.bodyB = body;
                            md.target = mousePos;
                            md.collideConnected = true;
                            mouseJoint = (b2MouseJoint*)world->CreateJoint(&md);
                        }
                    }
                break;
            case SDL_MOUSEMOTION:
                // if a body is being dragged, update its position
                if (draggedObject) {
                    b2Vec2 newPos(event.motion.x * P2M, event.motion.y * P2M);
                    b2Vec2 newPos(event.motion.x * P2M, event.motion.y * P2M);
                    newPos -= mouseOffset;

                    // Update the position of the dragged body
                    draggedObject->SetTransform(newPos, draggedObject->GetAngle());

                    // Move the mouse joint target to the new position
                    mouseJoint->SetTarget(newPos);
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
                    addCircle(WIDTH / 7, HEIGHT / 7, 2.5, true);
                }
                else if (event.key.keysym.sym == SDLK_r)
                {
                    // destroy all Box2D objects
                    destroyObjects();
                    //add borders again
                    addBorders();
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