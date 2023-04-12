#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include <box2d.h>
//#include <DebugDraw.h>
#include <SDL.h>

using namespace std;

#define PTM_RATIO 32.0

SDL_Window* GameWin;
SDL_Renderer* GameRen;
SDL_Event event;

int Width = 800, Height = 600;
bool GameRunning = true;

SDL_Rect box;
SDL_Rect ground;
SDL_Surface* Ball;
SDL_Texture* Tex;
SDL_Point center;

int box_x, box_y, box_w, box_h;
int ground_x, ground_y, ground_w, ground_h;

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		exit(0);
	}

	GameWin = SDL_CreateWindow("SDL Physics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN);
	GameRen = SDL_CreateRenderer(GameWin, -1, SDL_RENDERER_PRESENTVSYNC);

	box_x = 0, box_y = 0;
	box_w = 10, box_h = 10;

	box.x = box_x, box.y = box_y;
	box.w = box_w, box.h = box_h;

	ground_x = 0, ground_y = 540;
	ground_w = 800, ground_h = 560;

	ground.x = ground_x, ground.y = ground_y;
	ground.w = ground_w, ground.h = ground_h;

	Ball = SDL_LoadBMP("ball.bmp");
	Tex = SDL_CreateTextureFromSurface(GameRen, Ball);

	center.x = box.w * 0.5f;
	center.y = box.h - (box.w * 0.5);

	b2Vec2 gravity(0, 9.8f);
	bool doSleep = true;

	b2World* World;
	World = new b2World(gravity);
	World->SetGravity(gravity);

	b2Body* groundBody;
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set((float)ground_x, (float)ground_y);
	groundBody = World->CreateBody(&groundBodyDef);

	b2Body* Body;
	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;
	b2Vec2 ballVector;
	ballVector.Set(10, 10);
	ballBodyDef.angularVelocity = 0.0f;
	ballBodyDef.linearVelocity = ballVector;

	ballBodyDef.position.Set(0, 0);
	ballBodyDef.awake = true;
	Body = World->CreateBody(&ballBodyDef);

	b2PolygonShape groundBox;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundBox;
	boxShapeDef.density = 2.0f;
	boxShapeDef.restitution = 0.5f;
	groundBox.SetAsBox(800, 0); //This is what I needed to fix. Orignally it was (0,800)

	groundBody->CreateFixture(&groundBox, 0);

	b2PolygonShape dynamicBox;

	dynamicBox.SetAsBox(10.0f, 10.0f);

	b2FixtureDef fixtureDef;

	fixtureDef.shape = &dynamicBox;

	fixtureDef.density = 2.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.5f;

	Body->CreateFixture(&fixtureDef); //this line

	float timeStep = 1.0f / 100.0f;

	int velIter = 1.0;
	int posIter = 1.0;
	World->Step(timeStep, velIter, posIter);

	b2Vec2 pos = Body->GetPosition();
	float an = Body->GetAngle();

	cout << "X:" << pos.x << endl;
	cout << "Y:" << pos.y << endl;

	while (GameRunning)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				GameRunning = false;
			}

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					GameRunning = false;
				}

				if (event.key.keysym.sym == SDLK_LEFT)
				{
					Body->SetTransform(b2Vec2(-1, 0), 1);
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					Body->SetTransform(b2Vec2(+1, 0), 1);
				}
			}
		}


		World->Step(timeStep, velIter, posIter);

		b2Vec2 pos = Body->GetPosition();
		float angle = Body->GetAngle();

		box.x = pos.x;
		box.y = pos.y;

		SDL_SetRenderDrawColor(GameRen, 0, 255, 255, 0);

		SDL_RenderClear(GameRen);

		//SDL_SetRenderDrawColor(GameRen, 0, 255, 0, 0);
		//SDL_RenderFillRect(GameRen, &box);

		SDL_RendererFlip Flip = SDL_FLIP_NONE;

		SDL_RenderCopyEx(GameRen, Tex, NULL, &box, angle, &center, Flip);

		SDL_SetRenderDrawColor(GameRen, 255, 0, 0, 0);
		SDL_RenderFillRect(GameRen, &ground);

		SDL_RenderPresent(GameRen);
	}

	SDL_DestroyTexture(Tex);
	SDL_FreeSurface(Ball);

	SDL_DestroyWindow(GameWin);
	SDL_DestroyRenderer(GameRen);

	SDL_Quit();

	return 0;
}