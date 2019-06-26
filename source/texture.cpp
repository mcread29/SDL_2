#include "texture.h"

Texture::Texture(SDL_Texture *texture, float x, float y, float w, float h, int pScale)
{
    mTexture = texture;
    width = w;
    height = h;
    position = new b2Vec2(x, y);
    renderPosition = new b2Vec2(x * scale - w * anchor->x, y * scale - h * anchor->y);
    scale = pScale;
}

// Texture::~Texture()
// {
//     SDL_DestroyTexture(mTexture);
//     mTexture = NULL;
// }

void Texture::render(SDL_Renderer *renderer, Camera *camera)
{
    const int w = width * scale;
    const int h = height * scale;
    b2Vec2 newPos = camera->transformPoint(position);
    newPos.Set(newPos.x * scale - w * anchor->x, newPos.y * scale - h * anchor->y);
    SDL_Rect rect = {(int)newPos.x, (int)newPos.y, w, h};
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