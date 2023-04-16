#include <iostream>
#include <box2d.h>
#include <SDL.h>

// Convert meters to pixels
const float SCALE = 30.0f;

// Window dimensions
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// Box2D world dimensions
const float WORLD_WIDTH = 10.0f;
const float WORLD_HEIGHT = 10.0f;

// Helper function to check if a point is inside a box
bool pointInBox(float x, float y, b2Vec2 boxPosition, float boxWidth, float boxHeight) {
    if (x >= boxPosition.x - boxWidth / 2.0f && x <= boxPosition.x + boxWidth / 2.0f &&
        y >= boxPosition.y - boxHeight / 2.0f && y <= boxPosition.y + boxHeight / 2.0f) {
        return true;
    }
    return false;
}

int main(int argc, char* argv[])
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create window
    SDL_Window* window = SDL_CreateWindow("Box2D with SDL2",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Create Box2D world
    b2Vec2 gravity(0.0f, -0.04f); // 9.8 m/s^2 downwards gravity
    b2World world(gravity);

    // Create ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(WORLD_WIDTH / 2.0f, WORLD_HEIGHT - 15.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(WORLD_WIDTH / 2.0f, 0.5f);

    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundBox;

    groundBody->CreateFixture(&groundFixtureDef);

    // Create dynamic box body
    b2BodyDef boxBodyDef;
    boxBodyDef.type = b2_dynamicBody;
    boxBodyDef.position.Set(WORLD_WIDTH / 2.0f, 2.0f);
    b2Body* boxBody = world.CreateBody(&boxBodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(1.0f, 1.0f);

    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1.0f;

    boxBody->CreateFixture(&boxFixtureDef);

    // Main loop
    bool quit = false;
    bool boxSelected = false; // Flag to check if box is selected
    b2Vec2 boxOffset; // Offset of mouse click from box center
    while (!quit)
    {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }

            // Handle mouse events
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                // Check if mouse is within box bounds
                float boxLeft = (boxBody->GetPosition().x - 1.0f) * SCALE;
                float boxTop = (WORLD_HEIGHT - boxBody->GetPosition().y - 1.0f) * SCALE;
                if (mouseX >= boxLeft && mouseX <= boxLeft + 2.0f * SCALE &&
                    mouseY >= boxTop && mouseY <= boxTop + 2.0f * SCALE)
                {
                    // Set box as selected
                    boxSelected = true;

                    // Calculate offset from box center
                    boxOffset.x = mouseX - (boxLeft + SCALE);
                    boxOffset.y = mouseY - (boxTop + SCALE);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                // Deselect box
                boxSelected = false;
            }
        }

        // Update Box2D world
        if (!boxSelected) // Only update world if box is not selected
        {
            world.Step(1.0f / 60.0f, 6, 2);
        }

        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw ground
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect groundRect = {
            0,
            static_cast<int>(WORLD_HEIGHT * SCALE - (groundBody->GetPosition().y + 0.5f) * SCALE),
            static_cast<int>(WORLD_WIDTH * SCALE),
            static_cast<int>(0.5f * SCALE)
        };
        SDL_RenderFillRect(renderer, &groundRect);

        // Draw box
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        float boxWidth = 2.0f * SCALE;
        float boxHeight = 2.0f * SCALE;
        b2Vec2 boxPosition = boxBody->GetPosition();
        SDL_Rect boxRect = {
            static_cast<int>((boxPosition.x - 1.0f) * SCALE),
            static_cast<int>((WORLD_HEIGHT - boxPosition.y - 1.0f) * SCALE),
            static_cast<int>(boxWidth),
            static_cast<int>(boxHeight)
        };
        SDL_RenderFillRect(renderer, &boxRect);

        // Move box if selected
        if (boxSelected)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Set new box position based on mouse position
            float newBoxX = (mouseX - boxOffset.x) / SCALE + 1.0f;
            float newBoxY = WORLD_HEIGHT - (mouseY - boxOffset.y) / SCALE - 1.0f;
            boxBody->SetTransform(b2Vec2(newBoxX, newBoxY), boxBody->GetAngle());
        }

        // Present renderer
        SDL_RenderPresent(renderer);
    }

    // Destroy renderer
    SDL_DestroyRenderer(renderer);

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();

    return 0;
}