#include "utils.h"

SDL_Texture *textureLoad(std::string path, SDL_Renderer *renderer)
{
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image ERROR: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL ERROR: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}