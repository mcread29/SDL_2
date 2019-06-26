/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include "Box2D/Box2D.h"
// #include "camera.h"
#include "textureManager.h"
#include "texture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int WORLD_WIDTH = 192;
const int WORLD_HEIGHT = 108;
const int SCALE = 5;

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
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WORLD_WIDTH * SCALE, WORLD_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
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
			b2Vec2 gravity(0.0f, 6.0f);

			// Construct a world object, which will hold and simulate the rigid bodies.
			b2World world(gravity);

			const float groundX = WORLD_WIDTH / 2;
			const float groundY = WORLD_HEIGHT;
			const float groundW = WORLD_WIDTH * 10;
			const float groundH = 1;

			b2BodyDef groundBodyDef;
			groundBodyDef.type = b2_staticBody;
			groundBodyDef.position.Set(groundX, groundY);

			b2Body *groundBody = world.CreateBody(&groundBodyDef);

			b2PolygonShape groundBox;
			groundBox.SetAsBox(groundW / 2, groundH / 2);

			groundBody->CreateFixture(&groundBox, 0.0f);

			const float boxX = WORLD_WIDTH / 2;
			const float boxY = 0;
			const float boxW = 10.0f;
			const float boxH = boxW;

			b2BodyDef boxBodyDef;
			boxBodyDef.type = b2_dynamicBody;
			boxBodyDef.position.Set(boxX, boxY);
			boxBodyDef.gravityScale = 0.5f;

			b2Body *boxBody = world.CreateBody(&boxBodyDef);

			b2PolygonShape boxBox;
			boxBox.SetAsBox(boxW / 2, boxH / 2);

			b2FixtureDef boxFixtureDef;
			boxFixtureDef.shape = &boxBox;
			boxFixtureDef.density = 1.0f;
			boxFixtureDef.friction = 0.3f;

			boxBody->CreateFixture(&boxFixtureDef);

			float32 timeStep = 1.0f / 60.0f;
			int32 velocityIterations = 6;
			int32 positionIterations = 2;

			Texture character = Texture(textureManager.getTexture("character"), boxX, boxY, boxW, boxH, SCALE);
			Texture ground = Texture(textureManager.getTexture("ground"), groundX, groundY, groundW, groundH, SCALE);

			Camera *camera = new Camera(WORLD_WIDTH, WORLD_HEIGHT);
			camera->setCameraTarget(character.position);

			bool *keyPressStates = new bool[KEY_PRESS_SURFACE_TOTAL];
			for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++)
			{
				keyPressStates[i] = false;
			}

			int remainingJumpSteps = 0;

			Uint32 lastTick = SDL_GetTicks();
			//While application is running
			while (!quit)
			{
				Uint32 deltaTime = 0;
				// float check = 1.0f / 60.0f * 1000.0f;
				// while (deltaTime < check)
				// {
				Uint32 tick = SDL_GetTicks();
				deltaTime += tick - lastTick;
				lastTick = tick;
				// }
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						//Select surfaces based on key press
						float impulse;
						b2Vec2 vel;
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							vel = boxBody->GetLinearVelocity();
							vel.x = 0;
							boxBody->SetLinearVelocity(vel);
							impulse = boxBody->GetMass() * 30.0f;
							boxBody->ApplyLinearImpulse(b2Vec2(0, -impulse), boxBody->GetWorldCenter(), true);
							break;

						case SDLK_LEFT:
							keyPressStates[KEY_PRESS_SURFACE_LEFT] = true;
							break;

						case SDLK_RIGHT:
							keyPressStates[KEY_PRESS_SURFACE_RIGHT] = true;
							break;

						default:
							break;
						}
					}
					else if (e.type == SDL_KEYUP)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_LEFT:
							keyPressStates[KEY_PRESS_SURFACE_LEFT] = false;
							break;

						case SDLK_RIGHT:
							keyPressStates[KEY_PRESS_SURFACE_RIGHT] = false;
							break;

						default:
							break;
						}
					}
				}
				b2Vec2 vel = boxBody->GetLinearVelocity();
				if (keyPressStates[KEY_PRESS_SURFACE_LEFT])
					vel.x = std::max(vel.x - 3, -3.0f);
				if (keyPressStates[KEY_PRESS_SURFACE_RIGHT])
					vel.x = std::min(vel.x + 3, 3.0f);

				printf("%f %f\n", vel.x, vel.y);
				boxBody->SetLinearVelocity(vel);

				world.Step(deltaTime / 60.0f, velocityIterations, positionIterations);

				b2Vec2 position = boxBody->GetPosition();
				float32 angle = boxBody->GetAngle();

				b2Vec2 gPos = groundBody->GetPosition();

				character.setPosition(position.x, position.y);
				character.setAngle(angle);
				ground.setPosition(gPos.x, gPos.y);

				camera->update();
				SDL_RenderClear(gRenderer);
				ground.render(gRenderer, camera);
				character.render(gRenderer, camera);
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}