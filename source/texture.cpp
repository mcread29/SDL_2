#include "texture.h"

Texture::Texture(SDL_Texture *texture, float x, float y, float w, float h, int pScale)
{
    mTexture = texture;
    width = w;
    height = h;
    position = new b2Vec2(x, y);
    scale = pScale;
}

// Texture::~Texture()
// {
//     SDL_DestroyTexture(mTexture);
//     mTexture = NULL;
// }

void Texture::render(SDL_Renderer *renderer)
{
    const int w = width * scale;
    const int h = height * scale;
    const int x = position->x * scale - w * anchor->x;
    const int y = position->y * scale - h * anchor->y;
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(renderer, mTexture, NULL, &rect);
}

void Texture::setPosition(float x, float y)
{
    position->Set(x, y);
}

void Texture::setAngle(double angle)
{
    angle = angle;
}