#include "texture.h"

Texture::Texture(SDL_Texture *texture, SDL_Rect rect)
{
    mTexture = texture;
    mRect = rect;
}

// Texture::~Texture()
// {
//     SDL_DestroyTexture(mTexture);
//     mTexture = NULL;
// }

void Texture::render(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, mTexture, NULL, &mRect);
}

void Texture::setPosition(float x, float y)
{
    mRect.x = x;
    mRect.y = y;
}