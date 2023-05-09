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

b2Body* selectedBody = nullptr;

b2Body* carBody;
b2Body* wheel1;
b2Body* wheel2;
b2Body* wheel3;
b2Body* wheel4;
b2RevoluteJoint* joint1;
b2RevoluteJoint* joint2;
b2RevoluteJoint* joint3;
b2RevoluteJoint* joint4;

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
    fixturedef.restitution = 1.0;
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
    fixturedef.restitution = 1.0;
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
    fixturedef.restitution = 0.0;
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

void addCar(int x, int y, int w, int h)
{
    // Add the car body
    carBody = addRect(x, y, w, h);

    wheel1 = addCircle(x - w / 2, y + h / 2, h / 2.5);
    wheel2 = addCircle(x + w / 2, y + h / 2, h / 2.5);
    //wheel3 = addCircle(x - w / 2, y - h / 2, h / 2.5); 
    //wheel4 = addCircle(x + w / 2, y - h / 2, h / 2.5);

    // Add the revolute joints
    b2RevoluteJointDef jointdef1, jointdef2, jointdef3, jointdef4;
    jointdef1.Initialize(carBody, wheel1, wheel1->GetWorldCenter());
    jointdef2.Initialize(carBody, wheel2, wheel2->GetWorldCenter());
    //jointdef3.Initialize(carBody, wheel3, wheel3->GetWorldCenter()); 
    //jointdef4.Initialize(carBody, wheel4, wheel4->GetWorldCenter()); 
    jointdef1.enableMotor = true;
    jointdef2.enableMotor = true;
    //jointdef3.enableMotor = true;
    //jointdef4.enableMotor = true;
    joint1 = (b2RevoluteJoint*)world->CreateJoint(&jointdef1);
    joint2 = (b2RevoluteJoint*)world->CreateJoint(&jointdef2);
    //joint3 = (b2RevoluteJoint*)world->CreateJoint(&jointdef3); 
    //joint4 = (b2RevoluteJoint*)world->CreateJoint(&jointdef4); 
}


void addBorders() {

    addRect(WIDTH / 2, HEIGHT - 30, 800, 10, false); //floor
    addRect(WIDTH / 30, HEIGHT - 90, 10, 800, false); //leftBorder
    addRect(618, HEIGHT - 90, 10, 800, false); //rightBorder
    addRect(WIDTH / 2, 25, 800, 10, false); //top

}

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

    addBorders();

    //   for (int i = 0; i < 250; i++) {
    //       addCircle(WIDTH / 2, HEIGHT / 2, 8, true);
    //   }
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
                    addCircle(WIDTH / 7, HEIGHT / 7, 2.5, true);
                }
                else if (event.key.keysym.sym == SDLK_r)
                {
                    // destroy all Box2D objects
                    destroyObjects();
                    //add borders again
                    addBorders();
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
                    addCar(WIDTH / 2, HEIGHT / 2, 90, 30);
                }
                else if (event.key.keysym.sym == SDLK_LEFT) { // Move car backwards
                    carBody->ApplyForceToCenter(b2Vec2(-150.0f, 0.0f), true);
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) { // Move car forwards
                    carBody->ApplyForceToCenter(b2Vec2(150.0f, 0.0f), true);
                }
                else if (event.key.keysym.sym == SDLK_UP) { // Move car upwards in zero gravity
                    carBody->ApplyForceToCenter(b2Vec2(.0f, -150.0f), true);
                }
                else if (event.key.keysym.sym == SDLK_DOWN) { // Move car downwards in zero gravity
                    carBody->ApplyForceToCenter(b2Vec2(0.0f, 150.0f), true);
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

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}