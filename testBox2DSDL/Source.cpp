// THIS IS THE MAIN .CPP FILE THAT CONTAINS THE MAIN() FUNCTION
#include <iostream>
#include <box2d.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "InitialSetup.h"
#include "objects.h"
#include "Constants.h"
#include "Shape.h"
#include "Square.h"
#include "Triangle.h"
#include "Circle.h"
#include "Car.h"
#include "Borders.h"
#include "display.h"

// enumerators to select which screen to render in the program window
enum class Screen
{
    Welcome,
    Simulator
};

// this function helps select the body that is to be moved by the user mouse movement
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

// initializes the physics world
void init()
{
    world = new b2World(b2Vec2(0.0f, 9.81f));
    Borders b2;
    b2.addBorders();
}

// destroys objects in the physics world
void destroyObjects()
{
    b2Body* body = world->GetBodyList();  // get the first body
    while (body != nullptr) {
        b2Body* next = body->GetNext();   // get the next body
        world->DestroyBody(body);         // destroy the current body
        body = next;                      // move to the next body
    }
}

// the main program function
int main(int argc, char** argv)
{
    // SDL initialization
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Physics Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    Uint32 start;
    SDL_Event event;
    bool running = true;

    Screen currentScreen = Screen::Welcome;

    init();

    // flags
    bool addingTriangle = false;
    bool addingRect = false;
    bool addingCircle = false;
    bool windEnabled = false;
    bool gravityEnabled = true;
    bool leftMouseDown = false; // Flag to keep track of whether left mouse button is down
    bool rotateEnabled = false; // Flag to keep track of whether rotation is enabled or disabled
 
    // dynamic physics objects creation
    Circle* cir = new Circle;
    Square* sq = new Square;
    Triangle* tri = new Triangle;
    Borders* b1 = new Borders;
    Car* car = new Car;


    // WELCOME SCREEN SETUP CODE START:

    // Load the background image
    SDL_Surface* bgSurface = IMG_Load("background.jpeg");
    if (!bgSurface) {
        std::cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
        return 1;
    }

    // Create a texture from the surface
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);

    // Free the surface
    SDL_FreeSurface(bgSurface);

    // Initialize TTF
    TTF_Init();

    // Load the font
    TTF_Font* font = TTF_OpenFont("Machine BT.ttf", 48);
    TTF_Font* instructionfont = TTF_OpenFont("Machine BT.ttf", 18);
    // Create a surface with the message
    SDL_Color TitletextColor = { 160, 32, 240, 255 };
    SDL_Surface* messageSurface = TTF_RenderText_Solid(font,
        "Welcome to our Physics Simulator",
        TitletextColor);

    // Create a texture from the surface
    SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);

    // Get the dimensions of the texture
    int messageWidth = messageSurface->w;
    int messageHeight = messageSurface->h;

    // Free the surface
    SDL_FreeSurface(messageSurface);

    // Create a rectangle to position the message on the screen
    SDL_Rect messageRect;
    messageRect.x = (1265 - messageWidth) / 2;
    messageRect.y = 20;
    messageRect.w = messageWidth;
    messageRect.h = messageHeight;

    // Create a surface with the names of the group members
    SDL_Color nametextColor = { 255, 0, 0, 0 };
    TTF_Font* namefont = TTF_OpenFont("Machine BT.ttf", 24);
    SDL_Surface* groupSurface = TTF_RenderText_Solid(namefont,
        "Anas Bin Yousuf, Syed Ali Nisar Shah, Azhar Ali, Syed Maaz Ullah Shah",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* groupTexture = SDL_CreateTextureFromSurface(renderer, groupSurface);

    // Get the dimensions of the texture
    int groupWidth = groupSurface->w;
    int groupHeight = groupSurface->h;

    // Free the surface
    SDL_FreeSurface(groupSurface);

    // Create a rectangle to position the group names on the screen
    SDL_Rect groupRect;
    groupRect.x = (1265 - groupWidth) / 2;
    groupRect.y = 90;
    groupRect.w = groupWidth;
    groupRect.h = groupHeight;

    // Create a surface with the "Start" button
    SDL_Surface* startSurface = TTF_RenderText_Solid(font,
        "start",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);

    // Get the dimensions of the texture
    int startWidth = startSurface->w;
    int startHeight = startSurface->h;

    // Free the surface
    SDL_FreeSurface(startSurface);

    // Create a rectangle to position the "Start button on the screen
    SDL_Rect startRect;
    startRect.x = (1265 - startWidth) / 2;
    startRect.y = 127;
    startRect.w = startWidth;
    startRect.h = startHeight;

    // Create a surface with the circle instructions
    SDL_Surface* C_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press 'C' and then right click to spawn circle",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* C_instructionsTexture = SDL_CreateTextureFromSurface(renderer, C_instructionsSurface);

    // Get the dimensions of the texture
    int C_instructionsWidth = C_instructionsSurface->w;
    int C_instructionsHeight = C_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(C_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect C_instructionsRect;
    C_instructionsRect.x = (1265 - C_instructionsWidth) / 2;
    C_instructionsRect.y = 240;
    C_instructionsRect.w = C_instructionsWidth;
    C_instructionsRect.h = C_instructionsHeight;

    // Create a surface with the square instructions
    SDL_Surface* S_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press 'S' and then right click to spawn square",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* S_instructionsTexture = SDL_CreateTextureFromSurface(renderer, S_instructionsSurface);

    // Get the dimensions of the texture
    int S_instructionsWidth = S_instructionsSurface->w;
    int S_instructionsHeight = S_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(S_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect S_instructionsRect;
    S_instructionsRect.x = (1265 - S_instructionsWidth) / 2;
    S_instructionsRect.y = 270;
    S_instructionsRect.w = S_instructionsWidth;
    S_instructionsRect.h = S_instructionsHeight;

    // Create a surface with the triangle instructions
    SDL_Surface* T_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press 'T' and then right click to spawn triangle",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* T_instructionsTexture = SDL_CreateTextureFromSurface(renderer, T_instructionsSurface);

    // Get the dimensions of the texture
    int T_instructionsWidth = T_instructionsSurface->w;
    int T_instructionsHeight = T_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(T_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect T_instructionsRect;
    T_instructionsRect.x = (1265 - T_instructionsWidth) / 2;
    T_instructionsRect.y = 300;
    T_instructionsRect.w = T_instructionsWidth;
    T_instructionsRect.h = T_instructionsHeight;

    // Create a surface with the small circle instructions
    SDL_Surface* B_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press 'B' and then right click to spawn small circle",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* B_instructionsTexture = SDL_CreateTextureFromSurface(renderer, B_instructionsSurface);

    // Get the dimensions of the texture
    int B_instructionsWidth = B_instructionsSurface->w;
    int B_instructionsHeight = B_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(B_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect B_instructionsRect;
    B_instructionsRect.x = (1265 - B_instructionsWidth) / 2;
    B_instructionsRect.y = 330;
    B_instructionsRect.w = B_instructionsWidth;
    B_instructionsRect.h = B_instructionsHeight;

    // Create a surface with the car instructions
    SDL_Surface* M_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press 'M' to spawn a car",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* M_instructionsTexture = SDL_CreateTextureFromSurface(renderer, M_instructionsSurface);

    // Get the dimensions of the texture
    int M_instructionsWidth = M_instructionsSurface->w;
    int M_instructionsHeight = M_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(M_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect M_instructionsRect;
    M_instructionsRect.x = (1265 - M_instructionsWidth) / 2;
    M_instructionsRect.y = 360;
    M_instructionsRect.w = M_instructionsWidth;
    M_instructionsRect.h = M_instructionsHeight;

    // Create a surface with the wind instructions
    SDL_Surface* W_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press 'W' to toggle wind affect",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* W_instructionsTexture = SDL_CreateTextureFromSurface(renderer, W_instructionsSurface);

    // Get the dimensions of the texture
    int W_instructionsWidth = W_instructionsSurface->w;
    int W_instructionsHeight = W_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(W_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect W_instructionsRect;
    W_instructionsRect.x = (1265 - W_instructionsWidth) / 2;
    W_instructionsRect.y = 390;
    W_instructionsRect.w = W_instructionsWidth;
    W_instructionsRect.h = W_instructionsHeight;

    // Create a surface with the zero gravity instructions
    SDL_Surface* G_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press 'G' to toggle gravity",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* G_instructionsTexture = SDL_CreateTextureFromSurface(renderer, G_instructionsSurface);

    // Get the dimensions of the texture
    int G_instructionsWidth = G_instructionsSurface->w;
    int G_instructionsHeight = G_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(G_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect G_instructionsRect;
    G_instructionsRect.x = (1265 - G_instructionsWidth) / 2;
    G_instructionsRect.y = 420;
    G_instructionsRect.w = G_instructionsWidth;
    G_instructionsRect.h = G_instructionsHeight;

    // Create a surface with the reverse gravity instructions
    SDL_Surface* U_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press 'U' to enable reverse gravity affect",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* U_instructionsTexture = SDL_CreateTextureFromSurface(renderer, U_instructionsSurface);

    // Get the dimensions of the texture
    int U_instructionsWidth = U_instructionsSurface->w;
    int U_instructionsHeight = U_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(U_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect U_instructionsRect;
    U_instructionsRect.x = (1265 - U_instructionsWidth) / 2;
    U_instructionsRect.y = 450;
    U_instructionsRect.w = U_instructionsWidth;
    U_instructionsRect.h = U_instructionsHeight;

    // Create a surface with the rotate instructions
    SDL_Surface* P_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press 'P' to rotate all objects",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* P_instructionsTexture = SDL_CreateTextureFromSurface(renderer, P_instructionsSurface);

    // Get the dimensions of the texture
    int P_instructionsWidth = P_instructionsSurface->w;
    int P_instructionsHeight = P_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(P_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect P_instructionsRect;
    P_instructionsRect.x = (1265 - P_instructionsWidth) / 2;
    P_instructionsRect.y = 480;
    P_instructionsRect.w = P_instructionsWidth;
    P_instructionsRect.h = P_instructionsHeight;

    // Create a surface with the reset instructions
    SDL_Surface* R_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press 'R' to destroy all the objects",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* R_instructionsTexture = SDL_CreateTextureFromSurface(renderer, R_instructionsSurface);

    // Get the dimensions of the texture
    int R_instructionsWidth = R_instructionsSurface->w;
    int R_instructionsHeight = R_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(R_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect R_instructionsRect;
    R_instructionsRect.x = (1265 - R_instructionsWidth) / 2;
    R_instructionsRect.y = 510;
    R_instructionsRect.w = R_instructionsWidth;
    R_instructionsRect.h = R_instructionsHeight;

    // Create a surface with the move car instructions
    SDL_Surface* LR_instructionsSurface = TTF_RenderText_Solid(instructionfont,
        "Press left/right arrow key to move car",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* LR_instructionsTexture = SDL_CreateTextureFromSurface(renderer, LR_instructionsSurface);

    // Get the dimensions of the texture
    int LR_instructionsWidth = LR_instructionsSurface->w;
    int LR_instructionsHeight = LR_instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(LR_instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect LR_instructionsRect;
    LR_instructionsRect.x = (1265 - LR_instructionsWidth) / 2;
    LR_instructionsRect.y = 540;
    LR_instructionsRect.w = LR_instructionsWidth;
    LR_instructionsRect.h = LR_instructionsHeight;

    // Create a surface with instructions
    SDL_Surface* instructionsSurface = TTF_RenderText_Solid(namefont,
        "Instructions:",
        nametextColor);

    // Create a texture from the surface
    SDL_Texture* instructionsTexture = SDL_CreateTextureFromSurface(renderer, instructionsSurface);

    // Get the dimensions of the texture
    int instructionsWidth = instructionsSurface->w;
    int instructionsHeight = instructionsSurface->h;

    // Free the surface
    SDL_FreeSurface(instructionsSurface);

    // Create a rectangle to position the instructions on the screen
    SDL_Rect instructionsRect;
    instructionsRect.x = (1265 - instructionsWidth) / 2;
    instructionsRect.y = 200;
    instructionsRect.w = instructionsWidth;
    instructionsRect.h = instructionsHeight;

    // Set up the event loop
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Chunk* bgSound = Mix_LoadWAV("stranger-things-theme-song.mp3");
    Mix_PlayChannel(-1, bgSound, -1);

    // WELCOME SCREEN SETUP CODE END


    // main program loop
    while (running)
    {
        start = SDL_GetTicks();

        // clear the screen
        SDL_RenderClear(renderer);

        // checks for events and performs relavent actions
        while (SDL_PollEvent(&event))
        {
            // if the current screen is the Welcome screen
            if (currentScreen == Screen::Welcome)
            {
                // User requests quit
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
                // User clicks the "Start" button
                else if (event.type != SDL_MOUSEBUTTONDOWN)
                {
                    // Clear the screen
                    SDL_RenderClear(renderer);

                    // Render the background texture
                    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

                    // Render the title message texture
                    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);

                    // Render the group names texture
                    SDL_RenderCopy(renderer, groupTexture, NULL, &groupRect);

                    // Render the "Start" button texture
                    SDL_RenderCopy(renderer, startTexture, NULL, &startRect);

                    // render the shape-relavent textures
                    SDL_RenderCopy(renderer, instructionsTexture, NULL, &instructionsRect);

                    SDL_RenderCopy(renderer, C_instructionsTexture, NULL, &C_instructionsRect);

                    SDL_RenderCopy(renderer, S_instructionsTexture, NULL, &S_instructionsRect);

                    SDL_RenderCopy(renderer, T_instructionsTexture, NULL, &T_instructionsRect);

                    SDL_RenderCopy(renderer, B_instructionsTexture, NULL, &B_instructionsRect);

                    SDL_RenderCopy(renderer, M_instructionsTexture, NULL, &M_instructionsRect);

                    SDL_RenderCopy(renderer, W_instructionsTexture, NULL, &W_instructionsRect);

                    SDL_RenderCopy(renderer, G_instructionsTexture, NULL, &G_instructionsRect);

                    SDL_RenderCopy(renderer, U_instructionsTexture, NULL, &U_instructionsRect);

                    SDL_RenderCopy(renderer, P_instructionsTexture, NULL, &P_instructionsRect);

                    SDL_RenderCopy(renderer, R_instructionsTexture, NULL, &R_instructionsRect);

                    SDL_RenderCopy(renderer, LR_instructionsTexture, NULL, &LR_instructionsRect);

                    // Update the screen
                    SDL_RenderPresent(renderer);

                }
                else
                {
                    // this else block destroys all the textures when the "Start" button is clicked
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (mouseX >= startRect.x && mouseX < startRect.x + startRect.w && mouseY >= startRect.y && mouseY < startRect.y + startRect.h)
                    {
                        SDL_DestroyTexture(messageTexture);

                        SDL_DestroyTexture(groupTexture);

                        SDL_DestroyTexture(startTexture);

                        SDL_DestroyTexture(instructionsTexture);

                        SDL_DestroyTexture(C_instructionsTexture);

                        SDL_DestroyTexture(S_instructionsTexture);

                        SDL_DestroyTexture(T_instructionsTexture);

                        SDL_DestroyTexture(B_instructionsTexture);

                        SDL_DestroyTexture(M_instructionsTexture);

                        SDL_DestroyTexture(W_instructionsTexture);

                        SDL_DestroyTexture(G_instructionsTexture);

                        SDL_DestroyTexture(U_instructionsTexture);

                        SDL_DestroyTexture(P_instructionsTexture);

                        SDL_DestroyTexture(R_instructionsTexture);

                        SDL_DestroyTexture(LR_instructionsTexture);

                        SDL_DestroyTexture(bgTexture);

                        // removes the sound
                        Mix_FreeChunk(bgSound);

                        // sets the current screen to the Simulator screen
                        currentScreen = Screen::Simulator;
                    }
                }
            }
            // if the current screen is the Simulator screen
            else if (currentScreen == Screen::Simulator)
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN: // when a mouse button is pressed down
                    if (event.button.button == SDL_BUTTON_RIGHT) // for right click
                    {
                        if (addingRect) {
                            sq->addRect(event.button.x, event.button.y, 15, 15, true);

                        }
                        else if (addingTriangle) {
                            tri->addTriangle(event.button.x, event.button.y, 1, true);
                        }
                        else if (addingCircle) {
                            cir->addCircle(event.button.x, event.button.y, 8, true);
                        }
                    }
                    if (event.button.button == SDL_BUTTON_LEFT) { // for left click
                        selectBody(b2Vec2(event.button.x * P2M, event.button.y * P2M));
                        leftMouseDown = true; // Set flag to indicate left mouse button is down
                    }
                    break;
                case SDL_MOUSEMOTION: // when mouse is moving
                    if (selectedBody && leftMouseDown) // Update selected body position only if left mouse button is down
                    {
                        selectedBody->SetTransform(b2Vec2(event.motion.x * P2M, event.motion.y * P2M), selectedBody->GetAngle());
                    }
                    break;
                case SDL_MOUSEBUTTONUP: // when the mouse button is released
                    selectedBody = nullptr;
                    leftMouseDown = false; // Reset flag when left mouse button is released
                    break;
                case SDL_KEYDOWN: // when specific keyboard keys are clicked
                    if (event.key.keysym.sym == SDLK_s) // for 's' key
                    {
                        addingRect = true;
                        addingTriangle = false;
                        addingCircle = false;
                    }
                    else if (event.key.keysym.sym == SDLK_t) // for 't' key
                    {
                        addingTriangle = true;
                        addingRect = false;
                        addingCircle = false;
                    }
                    else if (event.key.keysym.sym == SDLK_c) // for 'c' key
                    {
                        addingCircle = true;
                        addingRect = false;
                        addingTriangle = false;
                    }
                    else if (event.key.keysym.sym == SDLK_w)  // for 'w' key
                    {
                        windEnabled = !windEnabled;

                        if (windEnabled) {
                            world->SetGravity(b2Vec2(9.81f, 9.81f));
                        }
                        else {
                            world->SetGravity(b2Vec2(0.0f, 9.81f));
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_g) // for 'g' key
                    { 
                        gravityEnabled = !gravityEnabled;

                        if (gravityEnabled) {
                            world->SetGravity(b2Vec2(0.0f, 9.81f));
                        }
                        else {
                            world->SetGravity(b2Vec2(0.0f, 0.0f));
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_u) // for 'u' key
                    {
                        world->SetGravity(b2Vec2(0.0f, -9.81f));
                    }
                    else if (event.key.keysym.sym == SDLK_b) // for 'b' key
                    {
                        cir->addCircle(WIDTH / 7, HEIGHT / 7, 2.5, true);
                    }
                    else if (event.key.keysym.sym == SDLK_r) // for 'r' key
                    {
                        // destroy all Box2D objects
                        destroyObjects();
                        //add borders again
                        b1->addBorders();
                    }
                    else if (event.key.keysym.sym == SDLK_p) // for 'p' key; toggles rotation
                    {
                        rotateEnabled = !rotateEnabled;

                        if (!rotateEnabled)
                        {
                            for (b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
                            {
                                if (body->GetType() == b2_dynamicBody)
                                {
                                    body->SetAngularVelocity(0); // Set the angular velocity of the body to 0
                                }
                            }
                        }

                    }
                    else if (event.key.keysym.sym == SDLK_m) // Toggle rotation
                    {
                        car->addCar(WIDTH - 350, HEIGHT / 2, 90, 30);
                    }
                    else if (event.key.keysym.sym == SDLK_LEFT) // for 'left' arrow key; moves car backwards
                    {
                        car->carBody->ApplyForceToCenter(b2Vec2(-150.0f, 0.0f), true);
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT) // for 'right' arrow key; moves car forwards
                    {
                        car->carBody->ApplyForceToCenter(b2Vec2(150.0f, 0.0f), true);
                    }
                    else if (event.key.keysym.sym == SDLK_UP) // for 'up' arrow key; move car upwards (in zero gravity)
                    {
                        car->carBody->ApplyForceToCenter(b2Vec2(.0f, -150.0f), true);
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN) // for 'down' arrow key; move car downwards (in zero gravity)
                    {
                        car->carBody->ApplyForceToCenter(b2Vec2(0.0f, 150.0f), true);
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) // for 'escape' key
                    {
                        running = false;
                    }
                    break;
                }
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

        // if current screen is Simulator screen
        if (currentScreen == Screen::Simulator)
        {
            // updating the physics simulation
            world->Step(1.0f / 60.0f, 8, 3);

            // clearing the renderer
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            // draws different objects from Box2D in SDL2
            display();

            // updating the screen
            SDL_RenderPresent(renderer);
        }

        // enforcing frame rate
        if (1000.0f / 60.0f > SDL_GetTicks() - start)
            SDL_Delay(1000.0f / 60.0f - (SDL_GetTicks() - start));
    }
    
    // freeing memory
    delete sq;
    delete b1;
    delete cir;
    delete tri;
    delete car;

    // Free the font
    TTF_CloseFont(font);
    // Quit TTF
    TTF_Quit();

    // SDL2 quit code
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}