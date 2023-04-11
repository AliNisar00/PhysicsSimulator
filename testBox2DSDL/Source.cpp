#include <box2d.h>
#include <SDL.h>

int main(int argc, char* argv[])
{
	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	// create a window
	SDL_Window* window = SDL_CreateWindow("Box2D with SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

	// create a renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	// create a Box2D world
	b2Vec2 gravity(0.0f, -9.81f);
	b2World* world = new b2World(gravity);

	// create Box2D bodies and fixtures
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* groundBody = world->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef dynamicBodyDef;
	dynamicBodyDef.type = b2_dynamicBody;
	dynamicBodyDef.position.Set(0.0f, 4.0f);
	b2Body* dynamicBody = world->CreateBody(&dynamicBodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	dynamicBody->CreateFixture(&fixtureDef);

	// main game loop
	bool quit = false;
	while (!quit)
	{
		// handle events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit == true;
			}
		}

		// update the Box2D world
		world->Step(1.0f / 60.0f, 6, 2);

		// render the Box2D bodies
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		for (b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
		{
			if (body->GetType() == b2_staticBody)
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else if (body->GetType() == b2_dynamicBody)
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}

			b2Fixture* fixture = body->GetFixtureList();
			while (fixture != nullptr)
			{
				b2PolygonShape* shape = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
				if (shape != nullptr)
				{
					b2Vec2 vertices[b2_maxPolygonVertices];
					int count = shape->m_vertices;
					for (int i = 0; i < 4; i++)
					{
						vertices[i] = body->GetWorldPoint(shape->GetVertex(i)); // continue from here...
					}
				}
			}

		}

	}
}