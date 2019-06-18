/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Box2D/Box2D.h"
#include "textureManager.h"
#include "texture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
TextureManager textureManager;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL ERROR: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not be initialized! SDL_image ERROR: %s\n", IMG_GetError());
					success = false;
				}

				textureManager = TextureManager();
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	success = textureManager.addTexture("texture", "assets/texture.png", gRenderer);
	success = textureManager.addTexture("character", "assets/boxy.png", gRenderer);
	success = textureManager.addTexture("ground", "assets/ground.png", gRenderer);

	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char *args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			// Define the gravity vector.
			b2Vec2 gravity(0.0f, 1.0f);

			// Construct a world object, which will hold and simulate the rigid bodies.
			b2World world(gravity);

			const SDL_Rect groundRect = {0, SCREEN_HEIGHT - 5, SCREEN_WIDTH, 11};
			b2BodyDef groundBodyDef;
			groundBodyDef.position.Set(groundRect.x, groundRect.y);
			b2Body *groundBody = world.CreateBody(&groundBodyDef);
			b2PolygonShape groundBox;
			groundBox.SetAsBox(groundRect.w / 2, groundRect.h / 2);
			groundBody->CreateFixture(&groundBox, 0.0f);

			const SDL_Rect boxRect = {SCREEN_WIDTH / 2, 0, 55, 55};
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(boxRect.x, boxRect.y);
			b2Body *body = world.CreateBody(&bodyDef);
			b2PolygonShape dynamicBox;
			dynamicBox.SetAsBox(boxRect.w / 2, boxRect.h / 2);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			body->CreateFixture(&fixtureDef);

			float32 timeStep = 1.0f / 60.0f;
			int32 velocityIterations = 6;
			int32 positionIterations = 2;

			Texture character = Texture(textureManager.getTexture("character"), {boxRect});
			Texture ground = Texture(textureManager.getTexture("ground"), {groundRect});

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}

				world.Step(timeStep, velocityIterations, positionIterations);

				b2Vec2 position = body->GetPosition();
				float32 angle = body->GetAngle();

				b2Vec2 gPos = groundBody->GetPosition();

				character.setPosition(position.x, position.y - boxRect.h / 2);
				ground.setPosition(gPos.x, gPos.y - groundRect.h / 2);

				SDL_RenderClear(gRenderer);
				ground.render(gRenderer);
				character.render(gRenderer);
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}