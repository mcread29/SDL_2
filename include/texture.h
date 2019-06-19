#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "Box2D/Box2D.h"

class Texture
{
public:
    Texture(SDL_Texture *texture, float x, float y, float w, float h, int pScale);
    // ~Texture();

    void render(SDL_Renderer *renderer);
    void setPosition(float x, float y);
    void setAngle(double angle);

    b2Vec2 *anchor = new b2Vec2(0.5, 0.5);
    b2Vec2 *position;

private:
    SDL_Texture *mTexture = NULL;
    float width;
    float height;
    double angle = 0;
    int scale;
};