#include <SDL.h>
#include <box2d.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_FPS = 60;

int main(int argc, char* argv[])
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Box2D + SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create a Box2D world
    b2Vec2 gravity(0.0f, 9.81f);
    b2World world(gravity);

    // Create a ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, SCREEN_HEIGHT);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Create a ground shape
    b2PolygonShape groundShape;
    groundShape.SetAsBox(SCREEN_WIDTH, 10.0f);

    // Add the ground fixture to the ground body
    groundBody->CreateFixture(&groundShape, 0.0f);

    // Create a dynamic body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    b2Body* body = world.CreateBody(&bodyDef);

    // Create a polygon shape for the body
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(50.0f, 50.0f);

    // Add the polygon fixture to the body
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

    // Set up the game loop
    bool quit = false;
    SDL_Event e;
    Uint32 startTime, endTime, frameTime, sleepTime;

    while (!quit)
    {
        // Get the start time of the frame
        startTime = SDL_GetTicks();

        // Process events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Step the Box2D world
        world.Step(1.0f / SCREEN_FPS, 6, 2);

        // Draw the ground
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        b2Vec2 groundVertices[4];
        for (int i = 0; i < 4; i++)
        {
            groundVertices[i] = groundBody->GetWorldPoint(groundShape.m_vertices[i]);
        }
        SDL_RenderDrawLines(renderer, (SDL_Point*)groundVertices, 5);

        // Draw the body
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        b2Vec2 bodyVertices[4];
        for (int i = 0; i < polygonShape.m_count; i++)
        {
            bodyVertices[i] = body->GetWorldPoint(polygonShape.m_vertices[i]);
        }
        SDL_RenderDrawLines(renderer, (SDL_Point*)bodyVertices, polygonShape.m_count + 1);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Get the end time of the frame
        endTime = SDL_GetTicks();

        // Calculate the time taken to render the frame
        frameTime = endTime - startTime;

        // Calculate the amount of time to sleep to maintain a constant frame rate
        sleepTime = (1000 / SCREEN_FPS) - frameTime;

        // If there is still time to sleep, then sleep
        if (sleepTime > 0)
        {
            SDL_Delay(sleepTime);
        }
    }

    // Destroy the renderer
    SDL_DestroyRenderer(renderer);

    // Destroy the window
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();

    return 0;

}